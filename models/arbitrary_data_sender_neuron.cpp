/*
 *  arbitrary_data_sender_neuron.cpp
 *
 *  This file is part of NEST.
 *
 *  Copyright (C) 2004 The NEST Initiative
 *
 *  NEST is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  NEST is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with NEST.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

// nest models
#include "arbitrary_data_sender_neuron.h"

// C++
#include <limits>

// libnestutil
#include "dict_util.h"
#include "numerics.h"

// nestkernel
#include "exceptions.h"
#include "kernel_manager.h"
#include "nest_impl.h"
#include "universal_data_logger_impl.h"

// sli
#include "dictutils.h"

namespace nest
{

void
register_arbitrary_data_sender_neuron( const std::string& name )
{
  register_node_model< arbitrary_data_sender_neuron >( name );
}

/* ----------------------------------------------------------------
 * Recordables map
 * ---------------------------------------------------------------- */

RecordablesMap< arbitrary_data_sender_neuron > arbitrary_data_sender_neuron::recordablesMap_;

template <>
void
RecordablesMap< arbitrary_data_sender_neuron >::create()
{
}

/* ----------------------------------------------------------------
 * Default constructors for parameters, state, and buffers
 * ---------------------------------------------------------------- */

arbitrary_data_sender_neuron::Parameters_::Parameters_()
{
}

arbitrary_data_sender_neuron::State_::State_()
{
}

arbitrary_data_sender_neuron::Buffers_::Buffers_( arbitrary_data_sender_neuron& n )
  : logger_( n )
{
}

arbitrary_data_sender_neuron::Buffers_::Buffers_( const Buffers_&, arbitrary_data_sender_neuron& n )
  : logger_( n )
{
}

/* ----------------------------------------------------------------
 * Getter and setter functions for parameters and state
 * ---------------------------------------------------------------- */

void
arbitrary_data_sender_neuron::Parameters_::get( DictionaryDatum& d ) const
{
}

void
arbitrary_data_sender_neuron::Parameters_::set( const DictionaryDatum& d, Node* node )
{
}

void
arbitrary_data_sender_neuron::State_::get( DictionaryDatum& d, const Parameters_& p ) const
{
}

void
arbitrary_data_sender_neuron::State_::set( const DictionaryDatum& d, const Parameters_& p, Node* node )
{
}

/* ----------------------------------------------------------------
 * Default and copy constructor for node
 * ---------------------------------------------------------------- */

arbitrary_data_sender_neuron::arbitrary_data_sender_neuron()
  : ArchivingNode()
  , P_()
  , S_()
  , B_( *this )
{
  recordablesMap_.create();
}

arbitrary_data_sender_neuron::arbitrary_data_sender_neuron( const arbitrary_data_sender_neuron& n )
  : ArchivingNode( n )
  , P_( n.P_ )
  , S_( n.S_ )
  , B_( n.B_, *this )
{
}

/* ----------------------------------------------------------------
 * Node initialization functions
 * ---------------------------------------------------------------- */

void
arbitrary_data_sender_neuron::init_buffers_()
{
  B_.spikes_.clear();   // includes resize
  B_.logger_.reset();   // includes resize
}

void
arbitrary_data_sender_neuron::pre_run_hook()
{
  B_.logger_.init(); // ensures initialization in case multimeter connected after Simulate
}


inline size_t
arbitrary_data_sender_neuron::send_test_event( Node& target, size_t receptor_type, synindex, bool )
{
  SpikeEvent e;
  e.set_sender( *this );
  return target.handles_test_event( e, receptor_type );
}

/* ----------------------------------------------------------------
 * Update function
 * ---------------------------------------------------------------- */

void
arbitrary_data_sender_neuron::update( Time const& origin, const long from, const long to )
{
  for ( long lag = from; lag < to; ++lag )
  {
    const long t = origin.get_steps() + lag;

    ++S_.i;
    if (S_.i % 100 == 0)
    {
      S_.flag = !S_.flag;
    }

    if (S_.i % 50 == 0)
    {
      std::cout << "arbitrary_data_sender_neuron : sending a spike\n";
      const size_t buffer_size = 3;
      std::vector< double > coeffs( buffer_size, 0.0 );
      coeffs[0] = S_.i;
      coeffs[1] = S_.flag ? 1 : 0;
      coeffs[2] = 42;

      ArbitraryDataEvent ev;
      ev.set_coeff_length(coeffs.size());
      ev.set_coeffarray( coeffs );
      kernel().event_delivery_manager.send_secondary( *this, ev );

      set_spiketime( Time::step( origin.get_steps() + lag + 1 ) );
      SpikeEvent se;
      kernel().event_delivery_manager.send( *this, se, lag );
    }

    B_.logger_.record_data( t );
  }
}

/* ----------------------------------------------------------------
 * Event handling functions
 * ---------------------------------------------------------------- */

void
arbitrary_data_sender_neuron::handle( SpikeEvent& e )
{
  assert( e.get_delay_steps() > 0 );

  B_.spikes_.add_value(
    e.get_rel_delivery_steps( kernel().simulation_manager.get_slice_origin() ), e.get_weight() * e.get_multiplicity() );
}

void
arbitrary_data_sender_neuron::handle( DataLoggingRequest& e )
{
  B_.logger_.handle( e );
}

} // namespace nest
