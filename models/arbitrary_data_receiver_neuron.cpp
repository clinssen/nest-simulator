/*
 *  arbitrary_data_receiver_neuron.cpp
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
#include "arbitrary_data_receiver_neuron.h"

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
register_arbitrary_data_receiver_neuron( const std::string& name )
{
  register_node_model< arbitrary_data_receiver_neuron >( name );
}

/* ----------------------------------------------------------------
 * Recordables map
 * ---------------------------------------------------------------- */

RecordablesMap< arbitrary_data_receiver_neuron > arbitrary_data_receiver_neuron::recordablesMap_;

template <>
void
RecordablesMap< arbitrary_data_receiver_neuron >::create()
{
}

/* ----------------------------------------------------------------
 * Default constructors for parameters, state, and buffers
 * ---------------------------------------------------------------- */

arbitrary_data_receiver_neuron::Parameters_::Parameters_()
{
}

arbitrary_data_receiver_neuron::State_::State_()
{
}

arbitrary_data_receiver_neuron::Buffers_::Buffers_( arbitrary_data_receiver_neuron& n )
  : logger_( n )
{
}

arbitrary_data_receiver_neuron::Buffers_::Buffers_( const Buffers_&, arbitrary_data_receiver_neuron& n )
  : logger_( n )
{
}

/* ----------------------------------------------------------------
 * Getter and setter functions for parameters and state
 * ---------------------------------------------------------------- */

void
arbitrary_data_receiver_neuron::Parameters_::get( DictionaryDatum& d ) const
{
}

void
arbitrary_data_receiver_neuron::Parameters_::set( const DictionaryDatum& d, Node* node )
{
}

void
arbitrary_data_receiver_neuron::State_::get( DictionaryDatum& d ) const
{
}

void
arbitrary_data_receiver_neuron::State_::set( const DictionaryDatum&, const Parameters_&, Node* )
{
}

/* ----------------------------------------------------------------
 * Default and copy constructor for node
 * ---------------------------------------------------------------- */

arbitrary_data_receiver_neuron::arbitrary_data_receiver_neuron()
  : ArchivingNode()
  , P_()
  , S_()
  , B_( *this )
{
  recordablesMap_.create();
}

arbitrary_data_receiver_neuron::arbitrary_data_receiver_neuron( const arbitrary_data_receiver_neuron& n )
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
arbitrary_data_receiver_neuron::init_buffers_()
{
  B_.spikes_.clear();   // includes resize
  B_.logger_.reset();   // includes resize
}

void
arbitrary_data_receiver_neuron::pre_run_hook()
{
  B_.logger_.init(); // ensures initialization in case multimeter connected after Simulate
}


/* ----------------------------------------------------------------
 * Update function
 * ---------------------------------------------------------------- */

void
arbitrary_data_receiver_neuron::update( Time const& origin, const long from, const long to )
{
  std::cout << "arbitrary_data_receiver_neuron::update()\n";

  for ( long lag = from; lag < to; ++lag )
  {
    const long t = origin.get_steps() + lag;

    // std::cout << "arbitrary_data_receiver_neuron::actually handling spike\n";

    B_.logger_.record_data( t );
  }
}

/* ----------------------------------------------------------------
 * Event handling functions
 * ---------------------------------------------------------------- */

void
arbitrary_data_receiver_neuron::handle( SpikeEvent& e )
{
  assert( e.get_delay_steps() > 0 );
  std::cout << "arbitrary_data_receiver_neuron::handle( SpikeEvent& e )\n";

  // B_.spikes_.add_value(
  //   e.get_rel_delivery_steps( kernel().simulation_manager.get_slice_origin() ), e.get_weight() * e.get_multiplicity() );
}

void
arbitrary_data_receiver_neuron::handle( DataLoggingRequest& e )
{
  B_.logger_.handle( e );
}
void
arbitrary_data_receiver_neuron::handle( ArbitraryDataEvent& e )
{
  std::cout << "arbitrary_data_receiver_neuron::handle( ArbitraryDataEvent& e )\n";

  for ( auto it_event = e.begin(); it_event != e.end(); )
  {
    std::cout << "-> " << e.get_coeffvalue( it_event ) << "\n"; // get_coeffvalue advances iterator
    // const long time_step = e.get_stamp().get_steps();
    // const double weight = e.get_weight();
    // const double error_signal = e.get_coeffvalue( it_event ); // get_coeffvalue advances iterator
    // const double learning_signal = weight * error_signal;

    // write_learning_signal_to_history( time_step, learning_signal );
  }


}

} // namespace nest
