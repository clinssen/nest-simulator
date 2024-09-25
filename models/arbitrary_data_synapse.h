/*
 *  arbitrary_data_synapse.h
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

#ifndef arbitrary_data_synapse_H
#define arbitrary_data_synapse_H

#include "connection.h"

namespace nest
{

void register_arbitrary_data_synapse( const std::string& name );

template < typename targetidentifierT >
class arbitrary_data_synapse : public Connection< targetidentifierT >
{

public:
  // this line determines which common properties to use
  typedef CommonSynapseProperties CommonPropertiesType;
  typedef Connection< targetidentifierT > ConnectionBase;

  static constexpr ConnectionModelProperties properties =
    ConnectionModelProperties::SUPPORTS_WFR;

  /**
   * Default Constructor.
   * Sets default values for all parameters. Needed by GenericConnectorModel.
   */
  arbitrary_data_synapse()
    : ConnectionBase()
    , weight_( 1.0 )
  {
  }

  SecondaryEvent* get_secondary_event();

  // Explicitly declare all methods inherited from the dependent base
  // ConnectionBase. This avoids explicit name prefixes in all places these
  // functions are used. Since ConnectionBase depends on the template parameter,
  // they are not automatically found in the base class.
  using ConnectionBase::get_delay_steps;
  using ConnectionBase::get_rport;
  using ConnectionBase::get_target;

  void
  check_connection( Node& s, Node& t, size_t receptor_type, const CommonPropertiesType& )
  {
    ArbitraryDataEvent ge;

    s.sends_secondary_event( ge );
    ge.set_sender( s );
    Connection< targetidentifierT >::target_.set_rport( t.handles_test_event( ge, receptor_type ) );
    Connection< targetidentifierT >::target_.set_target( &t );
  }

  /**
   * Send an event to the receiver of this connection.
   * \param e The event to send
   * \param p The port under which this connection is stored in the Connector.
   */
  bool
  send( Event& e, size_t t, const CommonSynapseProperties& )
  {

    std::cout << "arbitrary_data_synapse::send()\n";

    e.set_weight( weight_ );
    e.set_receiver( *get_target( t ) );
    e.set_rport( get_rport() );
    e();
    return true;
  }

  void get_status( DictionaryDatum& d ) const;

  void set_status( const DictionaryDatum& d, ConnectorModel& cm );

  void
  set_weight( double w )
  {
    weight_ = w;
  }

  void
  set_delay( double )
  {
    throw BadProperty( "arbitrary_data_synapse connection has no delay" );
  }

private:
  double weight_; //!< connection weight
};

template < typename targetidentifierT >
constexpr ConnectionModelProperties arbitrary_data_synapse< targetidentifierT >::properties;

template < typename targetidentifierT >
void
arbitrary_data_synapse< targetidentifierT >::get_status( DictionaryDatum& d ) const
{
  // We have to include the delay here to prevent
  // errors due to internal calls of
  // this function in SLI/pyNEST
  ConnectionBase::get_status( d );
  def< double >( d, names::weight, weight_ );
  def< long >( d, names::size_of, sizeof( *this ) );
}

template < typename targetidentifierT >
SecondaryEvent*
arbitrary_data_synapse< targetidentifierT >::get_secondary_event()
{
  return new ArbitraryDataEvent();
}

template < typename targetidentifierT >
void
arbitrary_data_synapse< targetidentifierT >::set_status( const DictionaryDatum& d, ConnectorModel& cm )
{
  // If the delay is set, we throw a BadProperty
  if ( d->known( names::delay ) )
  {
    throw BadProperty( "arbitrary_data_synapse connection has no delay" );
  }

  ConnectionBase::set_status( d, cm );
  updateValue< double >( d, names::weight, weight_ );
}

} // namespace

#endif /* #ifndef arbitrary_data_synapse_H */
