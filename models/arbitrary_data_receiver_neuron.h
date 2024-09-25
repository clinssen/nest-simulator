/*
 *  arbitrary_data_receiver_neuron.h
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

#ifndef arbitrary_data_receiver_neuron_H
#define arbitrary_data_receiver_neuron_H

// Generated includes:
#include "config.h"

// C includes:
#include <gsl/gsl_errno.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_odeiv.h>
#include <gsl/gsl_sf_exp.h>

// Includes from nestkernel:
#include "archiving_node.h"
#include "connection.h"
#include "event.h"
#include "nest_types.h"
#include "node.h"
#include "recordables_map.h"
#include "ring_buffer.h"
#include "universal_data_logger.h"

namespace nest
{

void register_arbitrary_data_receiver_neuron( const std::string& name );

class arbitrary_data_receiver_neuron : public ArchivingNode
{

public:
  typedef Node base;

  arbitrary_data_receiver_neuron();
  arbitrary_data_receiver_neuron( const arbitrary_data_receiver_neuron& );

  /**
   * Import sets of overloaded virtual functions.
   * @see Technical Issues / Virtual Functions: Overriding, Overloading, and
   * Hiding
   */
  using Node::handle;
  using Node::handles_test_event;
  using Node::sends_secondary_event;

  size_t send_test_event( Node& target, size_t receptor_type, synindex, bool ) override;

  void handle( SpikeEvent& ) override;
  void handle( DataLoggingRequest& ) override;
  void handle( ArbitraryDataEvent& ) override;

  size_t handles_test_event( SpikeEvent&, size_t ) override;
  size_t handles_test_event( DataLoggingRequest&, size_t ) override;
  size_t handles_test_event( ArbitraryDataEvent&, size_t ) override;

  void get_status( DictionaryDatum& ) const override;
  void set_status( const DictionaryDatum& ) override;

private:
  void init_buffers_() override;
  void pre_run_hook() override;
  void update( Time const&, const long, const long ) override;

  // Friends --------------------------------------------------------

  // make dynamics function quasi-member
  friend int arbitrary_data_receiver_neuron_dynamics( double, const double*, double*, void* );

  // The next two classes need to be friends to access the State_ class/member
  friend class RecordablesMap< arbitrary_data_receiver_neuron >;
  friend class UniversalDataLogger< arbitrary_data_receiver_neuron >;

private:
  // ----------------------------------------------------------------

  /**
   * Independent parameters of the model.
   */
  struct Parameters_
  {
    Parameters_();

    void get( DictionaryDatum& ) const;        //!< Store current values in dictionary
    void set( const DictionaryDatum&, Node* ); //!< Set values from dictionary
  };

public:
  // ----------------------------------------------------------------

  /**
   * State variables of the model.
   */
  struct State_
  {
    State_();

    void get( DictionaryDatum& ) const;
    void set( const DictionaryDatum&, const Parameters_&, Node* );
  };

  // Variables class -------------------------------------------------------

  /**
   * Internal variables of the model.
   * Variables are re-initialized upon each call to Simulate.
   */
  struct Variables_
  {
  };

  // ----------------------------------------------------------------

  /**
   * Buffers of the model.
   */
  struct Buffers_
  {
    Buffers_( arbitrary_data_receiver_neuron& ); //!< Sets buffer pointers to 0
    //! Sets buffer pointers to 0
    Buffers_( const Buffers_&, arbitrary_data_receiver_neuron& );

    std::vector< RingBuffer > spikes_;

    //! Logger for all analog data
    UniversalDataLogger< arbitrary_data_receiver_neuron > logger_;
  };

  // Access functions for UniversalDataLogger -------------------------------

  Parameters_ P_;
  State_ S_;
  Variables_ V_;
  Buffers_ B_;

  //! Mapping of recordables names to access functions
  static RecordablesMap< arbitrary_data_receiver_neuron > recordablesMap_;
};


inline size_t
arbitrary_data_receiver_neuron::send_test_event( Node& target, size_t receptor_type, synindex, bool )
{
  SpikeEvent e;
  e.set_sender( *this );

  return target.handles_test_event( e, receptor_type );
}


inline size_t
arbitrary_data_receiver_neuron::handles_test_event( SpikeEvent&, size_t receptor_type )
{
  if ( receptor_type != 0 )
  {
    throw UnknownReceptorType( receptor_type, get_name() );
  }
  return 0;
}

inline size_t
arbitrary_data_receiver_neuron::handles_test_event( DataLoggingRequest& dlr, size_t receptor_type )
{
  if ( receptor_type != 0 )
  {
    throw UnknownReceptorType( receptor_type, get_name() );
  }
  return B_.logger_.connect_logging_device( dlr, recordablesMap_ );
}

inline size_t
arbitrary_data_receiver_neuron::handles_test_event( ArbitraryDataEvent&, size_t receptor_type )
{
  if ( receptor_type != 0 )
  {
    throw UnknownReceptorType( receptor_type, get_name() );
  }
  return 0;
}

inline void
arbitrary_data_receiver_neuron::get_status( DictionaryDatum& d ) const
{
  P_.get( d );
  S_.get( d );
  ArchivingNode::get_status( d );

  ( *d )[ names::recordables ] = recordablesMap_.get_list();

  def< double >( d, names::t_spike, get_spiketime_ms() );
}

inline void
arbitrary_data_receiver_neuron::set_status( const DictionaryDatum& d )
{
  Parameters_ ptmp = P_;     // temporary copy in case of errors
  ptmp.set( d, this );       // throws if BadProperty
  State_ stmp = S_;          // temporary copy in case of errors
  stmp.set( d, ptmp, this ); // throws if BadProperty

  // We now know that (ptmp, stmp) are consistent. We do not
  // write them back to (P_, S_) before we are also sure that
  // the properties to be set in the parent class are internally
  // consistent.
  ArchivingNode::set_status( d );

  // if we get here, temporaries contain consistent set of properties
  P_ = ptmp;
  S_ = stmp;

  pre_run_hook();
}

} // namespace


#endif // arbitrary_data_receiver_neuron_H
