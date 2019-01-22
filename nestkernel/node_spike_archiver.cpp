/*
 *  node_tracer.cpp
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

#include "node_tracer.h"

// Includes from nestkernel:
#include "kernel_manager.h"

// Includes from sli:
#include "dictutils.h"

namespace nest
{

nest::Node_Spike_Archiver::Node_Spike_Archiver()
  : n_incoming_( 0 )
  , last_spike_( -1.0 )
{
}

nest::Node_Spike_Archiver::Node_Spike_Archiver( const Node_Spike_Archiver& n )
  : Node( n )
  , n_incoming_( n.n_incoming_ )
  , last_spike_( n.last_spike_ )
{
}

void
Node_Spike_Archiver::register_stdp_connection( double t_first_read )
{
  // Mark all entries in the deque, which we will not read in future as read by
  // this input input, so that we savely increment the incoming number of
  // connections afterwards without leaving spikes in the history.
  // For details see bug #218. MH 08-04-22

  for ( std::deque< histentry >::iterator runner = history_.begin();
        runner != history_.end()
          and ( t_first_read - runner->t_ > -1.0
                  * kernel().connection_manager.get_stdp_eps() );
        ++runner )
  {
    ( runner->access_counter_ )++;
  }

  n_incoming_++;
}

void
nest::Node_Spike_Archiver::get_history( double t1,
  double t2,
  std::deque< histentry >::iterator* start,
  std::deque< histentry >::iterator* finish )
{
  *finish = history_.end();
  if ( history_.empty() )
  {
    *start = *finish;
    return;
  }
  std::deque< histentry >::reverse_iterator runner = history_.rbegin();
  const double t2_lim = t2 + kernel().connection_manager.get_stdp_eps();
  const double t1_lim = t1 + kernel().connection_manager.get_stdp_eps();
  while ( runner != history_.rend() and runner->t_ >= t2_lim )
  {
    ++runner;
  }
  *finish = runner.base();
  while ( runner != history_.rend() and runner->t_ >= t1_lim )
  {
    runner->access_counter_++;
    ++runner;
  }
  *start = runner.base();
}

void
nest::Node_Spike_Archiver::notifySpikeOccurred( Time const& t_sp, double offset )
{
    const double t_sp_ms = t_sp.get_ms() - offset;
    last_spike_ = t_sp_ms;
    history_.push_back( histentry( last_spike_, Kminus_, triplet_Kminus_, 0 ) );
}

void
nest::Node_Spike_Archiver::get_status( DictionaryDatum& d ) const
{

#ifdef DEBUG_ARCHIVER
  def< int >( d, names::archiver_length, history_.size() );
#endif

}

void
nest::Node_Spike_Archiver::set_status( const DictionaryDatum& d )
{
  // check, if to clear spike history and K_minus
  bool clear = false;
  updateValue< bool >( d, names::clear, clear );
  if ( clear )
  {
    clear_history();
  }
}

void
nest::Node_Spike_Archiver::clear_history()
{
  last_spike_ = -1.0;
  history_.clear();
}


} // of namespace nest
