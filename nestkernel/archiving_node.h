/*
 *  archiving_node.h
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

/**
 * \file archiving_node.h
 * Definition of Archiving_Node which is capable of
 * recording and managing a spike history.
 * \author Moritz Helias, Abigail Morrison
 * \date april 2006
 */

#ifndef ARCHIVING_NODE_H
#define ARCHIVING_NODE_H

// C++ includes:
#include <deque>
#include <map>

// Includes from nestkernel:
#include "histentry.h"
#include "nest_time.h"
#include "nest_types.h"
#include "node.h"
//#include "trace.h"
//#include "exponential_all_to_all_trace.h"
#include "synaptic_element.h"
#include "kernel_manager.h"

// Includes from sli:
#include "dictdatum.h"

#define DEBUG_ARCHIVER 1

namespace nest
{

/**
 * \class Archiving_Node
 * a node which archives spike history for the purposes of
 * timing dependent plasticity
 */
class Archiving_Node : public Node
{
  using Node::get_synaptic_elements;

public:
  /**
   * \fn Archiving_Node()
   * Constructor.
   */
  Archiving_Node();

  /**
   * \fn Archiving_Node()
   * Copy Constructor.
   */
  Archiving_Node( const Archiving_Node& );
  /**

   * \fn double get_Ca_minus()
   * return the current value of Ca_minus
   */
  double get_Ca_minus() const;

  /**
   * \fn double get_synaptic_elements(Name n)
   * get the number of synaptic element for the current Node
   * the number of synaptic elements is a double value but the number of
   * actual vacant and connected elements is an integer truncated from this
   * value
   */
  double get_synaptic_elements( Name n ) const;

  /**
   * \fn int get_synaptic_elements_vacant(Name n)
   * get the number of synaptic elements of type n which are available
   * for new synapse creation
   */
  int get_synaptic_elements_vacant( Name n ) const;

  /**
   * \fn int get_synaptic_elements_connected(Name n)
   * get the number of synaptic element of type n which are currently
   * connected
   */
  int get_synaptic_elements_connected( Name n ) const;

  /**
   * \fn std::map<Name, double> get_synaptic_elements()
   * get the number of all synaptic elements for the current Node
   */
  std::map< Name, double > get_synaptic_elements() const;

  /**
   * \fn void update_synaptic_elements()
   * Change the number of synaptic elements in the node depending on the
   * dynamics described by the corresponding growth curve
   */
  void update_synaptic_elements( double t );

  /**
   * \fn void decay_synaptic_elements_vacant()
   * Delete a certain portion of the vacant synaptic elements which are not
   * in use
   */
  void decay_synaptic_elements_vacant();

  /**
   * \fn void connect_synaptic_element()
   * Change the number of connected synaptic elements by n
   */
  void connect_synaptic_element( Name name, int n );

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

  /**
   * Register a new incoming STDP connection.
   *
   * t_first_read: The newly registered synapse will read the history entries
   * with t > t_first_read.
   */
  void register_stdp_connection( double t_first_read );

  void get_status( DictionaryDatum& d ) const;
  void set_status( const DictionaryDatum& d );

  /**
   * retrieve the current value of tau_Ca which defines the exponential decay
   * constant of the intracellular calcium concentration
   */
  double get_tau_Ca() const;

  size_t register_trace(const Name &n, double tau, bool reset);
/*
    template <typename T>
    size_t register_trace(const Name &n, const DictionaryDatum &parms)
    {
        // XXX: TODO: check if name, parms combination has already occurred
        T *tr = new T(parms);
        std::cout << "* In Archiving_Node::register_trace(): emplacing " << n << std::endl;
    //    traces.emplace(n, std::ref<T>(t));
        traces.emplace(n, tr);
        return traces.size() - 1;
    }
*/
    /**
     * update traces of this node due to a spike at t = t_sp
    **/
    //void update_traces(Time const& t_sp);

    void get_trace_values(const Time& t, double *tr) {
        std::cout << "* In Archiving_Node::get_trace_values(t = " << t.get_ms() << ")" << std::endl;
        if ( history_.empty() )
        {
            std::cout << "\thistory is empty" << std::endl;
            for (size_t i = 0; i < n_traces; ++i) {
                tr[i] = this->traces[i];
            }
        }
        else
        {
            int spike_idx = history_.size() - 1;
            while ( spike_idx >= 0 )
            {
              if ( t.get_ms() - history_[ spike_idx ].t_ > kernel().connection_manager.get_stdp_eps() )
              {
                  for (size_t i = 0; i < n_traces; ++i) {
                    tr[i] = ( history_[ spike_idx ].Kminus_
                            * std::exp( ( history_[ spike_idx ].t_ - t.get_ms() ) * traces_tau_inv[i] ) );
                    std::cout << "\ttr[" << i << "] = " << tr[i] << ")" << std::endl;

                  }
                  return;
              }
              --spike_idx;
            }
            std::cout << "\tfall-through" << std::endl;
            for (size_t i = 0; i < n_traces; ++i) {
                tr[i] = 0.;
            }
        }
    }

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

  std::vector<double> trace_property_tau;
  std::vector<double> traces_tau_inv;
  std::vector <bool> trace_types;
  std::vector<double> traces;
  size_t n_traces;

  // Map of the synaptic elements
  std::map< Name, SynapticElement > synaptic_elements_map_;



    //std::map< Name, std::reference_wrapper< Trace > > traces;
//    std::map< Name, Trace* > traces;


};

inline double
Archiving_Node::get_spiketime_ms() const
{
  return last_spike_;
}

inline double
Archiving_Node::get_tau_Ca() const
{
  return tau_Ca_;
}

inline double
Archiving_Node::get_Ca_minus() const
{
  return Ca_minus_;
}

} // of namespace
#endif
