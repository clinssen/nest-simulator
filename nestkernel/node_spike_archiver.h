/*
 *  node_tracer.h
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


#ifndef NODE_TRACER_H
#define NODE_TRACER_H

// C++ includes:
#include <deque>

// Includes from nestkernel:
#include "histentry.h"
#include "nest_time.h"
#include "nest_types.h"
#include "node.h"
#include "synaptic_element.h"

// Includes from sli:
#include "dictdatum.h"

#define DEBUG_ARCHIVER 1

namespace nest
{

class Node_Spike_Archiver : public Node_Tracer
{

public:
  /**
   * \fn Node_Spike_Archiver()
   * Constructor.
   */
  Node_Spike_Archiver();

  /**
   * \fn Node_Spike_Archiver()
   * Copy Constructor.
   */
  Node_Spike_Archiver( const Node_Spike_Archiver& );

    void
    notifySpikeOccurred( Time const& t_sp, double offset );



  /**
   * \fn double get_Ca_minus()
   * return the current value of Ca_minus
   */
  double get_Ca_minus() const;

  /**
   * \fn double get_K_value(long t)
   * return the Kminus value at t (in ms).
   */
  double get_K_value( double t );

  /**
   * write the Kminus and triplet_Kminus values at t (in ms) to
   * the provided locations.
   * @throws UnexpectedEvent
   */

  void get_K_values( double t, double& Kminus, double& triplet_Kminus );

  /**
   * \fn double get_triplet_K_value(std::deque<histentry>::iterator &iter)
   * return the triplet Kminus value for the associated iterator.
   */

  double get_triplet_K_value( const std::deque< histentry >::iterator& iter );

  /**
   * \fn void get_history(long t1, long t2,
   * std::deque<Archiver::histentry>::iterator* start,
   * std::deque<Archiver::histentry>::iterator* finish)
   * return the spike times (in steps) of spikes which occurred in the range
   * (t1,t2].
   */
  void get_history( double t1,
    double t2,
    std::deque< histentry >::iterator* start,
    std::deque< histentry >::iterator* finish );

  void get_status( DictionaryDatum& d ) const;
  void set_status( const DictionaryDatum& d );

  /**
   * retrieve the current value of tau_Ca which defines the exponential decay
   * constant of the intracellular calcium concentration
   */
  double get_tau_Ca() const;

protected:
  /**
   * \fn void set_spiketime(Time const & t_sp, double offset)
   * record spike history
   */
  void set_spiketime( Time const& t_sp, double offset = 0.0 );

  /**
   * \fn double get_spiketime()
   * return most recent spike time in ms
   */
  inline double get_spiketime_ms() const;

  /**
   * \fn void clear_history()
   * clear spike history
   */
  void clear_history();

private:
  // number of incoming connections from stdp connectors.
  // needed to determine, if every incoming connection has
  // read the spikehistory for a given point in time
  size_t n_incoming_;

  // sum exp(-(t-ti)/tau_minus)
  double Kminus_;

  // sum exp(-(t-ti)/tau_minus_triplet)
  double triplet_Kminus_;

  double tau_minus_;
  double tau_minus_inv_;

  // time constant for triplet low pass filtering of "post" spike train
  double tau_minus_triplet_;
  double tau_minus_triplet_inv_;

  double last_spike_;

  // spiking history needed by stdp synapses
  std::deque< histentry > history_;

  /*
   * Structural plasticity
   */

  // Time of the last update of the Calcium concentration in ms
  double Ca_t_;

  // Value of the calcium concentration [Ca2+] at Ca_t_. Intracellular calcium
  // concentration has a linear factor to mean electrical activity of 10^2,
  // this means, for example, that a [Ca2+] of 0.2 is equivalent to a mean
  // activity of 20Hz.
  double Ca_minus_;

  // Time constant for exponential decay of the intracellular calcium
  // concentration
  double tau_Ca_;

  // Increase in calcium concentration [Ca2+] for each spike of the neuron
  double beta_Ca_;

  // Map of the synaptic elements
  std::map< Name, SynapticElement > synaptic_elements_map_;
};

inline double
Node_Spike_Archiver::get_spiketime_ms() const
{
  return last_spike_;
}

inline double
Node_Spike_Archiver::get_tau_Ca() const
{
  return tau_Ca_;
}

inline double
Node_Spike_Archiver::get_Ca_minus() const
{
  return Ca_minus_;
}

} // of namespace
#endif
