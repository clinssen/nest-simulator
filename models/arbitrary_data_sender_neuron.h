/*
 *  arbitrary_data_sender_neuron.h
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

#ifndef arbitrary_data_sender_neuron_H
#define arbitrary_data_sender_neuron_H

// nestkernel
#include "connection.h"
#include "archiving_node.h"
#include "event.h"
#include "secondary_event.h"
#include "nest_types.h"
#include "ring_buffer.h"
#include "universal_data_logger.h"

namespace nest
{

void register_arbitrary_data_sender_neuron( const std::string& name );

class arbitrary_data_sender_neuron : public ArchivingNode
{

public:
  //! Default constructor.
  arbitrary_data_sender_neuron();

  //! Copy constructor.
  arbitrary_data_sender_neuron( const arbitrary_data_sender_neuron& );

  using Node::handle;
  using Node::handles_test_event;

  size_t send_test_event( Node&, size_t, synindex, bool ) override;

  using Node::sends_secondary_event;

  void
  sends_secondary_event( ArbitraryDataEvent& ) override
  {
  }

  void handle( SpikeEvent& ) override;
  void handle( DataLoggingRequest& ) override;

  size_t handles_test_event( SpikeEvent&, size_t ) override;
  size_t handles_test_event( DataLoggingRequest&, size_t ) override;

  void get_status( DictionaryDatum& ) const override;
  void set_status( const DictionaryDatum& ) override;

  void pre_run_hook() override;
  void update( Time const&, const long, const long ) override;

protected:
  void init_buffers_() override;

private:
  //! Map for storing a static set of recordables.
  friend class RecordablesMap< arbitrary_data_sender_neuron >;

  //! Logger for universal data supporting the data logging request / reply mechanism. Populated with a recordables map.
  friend class UniversalDataLogger< arbitrary_data_sender_neuron >;

  //! Structure of parameters.
  struct Parameters_
  {
    //! Default constructor.
    Parameters_();

    //! Get the parameters and their values.
    void get( DictionaryDatum& ) const;

    //! Set the parameters and throw errors in case of invalid values.
    void set( const DictionaryDatum&, Node* );
  };

  //! Structure of state variables.
  struct State_
  {
    int i;
    int flag;

    //! Default constructor.
    State_();

    //! Get the state variables and their values.
    void get( DictionaryDatum&, const Parameters_& ) const;

    //! Set the state variables.
    void set( const DictionaryDatum&, const Parameters_&, Node* );
  };

  //! Structure of buffers.
  struct Buffers_
  {
    //! Default constructor.
    Buffers_( arbitrary_data_sender_neuron& );

    //! Copy constructor.
    Buffers_( const Buffers_&, arbitrary_data_sender_neuron& );

    //! Buffer for incoming spikes.
    RingBuffer spikes_;

    //! Logger for universal data.
    UniversalDataLogger< arbitrary_data_sender_neuron > logger_;
  };

  //! Structure of general variables.
  struct Variables_
  {
  };

  //!< Structure of parameters.
  Parameters_ P_;

  //!< Structure of state variables.
  State_ S_;

  //!< Structure of general variables.
  Variables_ V_;

  //!< Structure of buffers.
  Buffers_ B_;

  //! Map storing a static set of recordables.
  static RecordablesMap< arbitrary_data_sender_neuron > recordablesMap_;
};

inline size_t
arbitrary_data_sender_neuron::handles_test_event( SpikeEvent&, size_t receptor_type )
{
  if ( receptor_type != 0 )
  {
    throw UnknownReceptorType( receptor_type, get_name() );
  }

  return 0;
}

inline size_t
arbitrary_data_sender_neuron::handles_test_event( DataLoggingRequest& dlr, size_t receptor_type )
{
  if ( receptor_type != 0 )
  {
    throw UnknownReceptorType( receptor_type, get_name() );
  }

  return B_.logger_.connect_logging_device( dlr, recordablesMap_ );
}

inline void
arbitrary_data_sender_neuron::get_status( DictionaryDatum& d ) const
{
  P_.get( d );
  S_.get( d, P_ );
  ( *d )[ names::recordables ] = recordablesMap_.get_list();
}

inline void
arbitrary_data_sender_neuron::set_status( const DictionaryDatum& d )
{
  // temporary copies in case of errors
  Parameters_ ptmp = P_;
  State_ stmp = S_;

  // make sure that ptmp and stmp consistent - throw BadProperty if not
  ptmp.set( d, this );
  stmp.set( d, ptmp, this );

  P_ = ptmp;
  S_ = stmp;
}

} // namespace nest

#endif // arbitrary_data_sender_neuron_H
