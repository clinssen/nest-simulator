/*
 *  modelsmodule.h
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

#ifndef MODELSMODULE_H
#define MODELSMODULE_H

// C++ includes:
#include <string>

// Includes from sli:
#include "connector_model.h"
#include "genericmodel.h"
#include "slimodule.h"

// Includes from nestkernel
#include "genericmodel_impl.h"

// Generated includes:
#include "config.h"

// Neuron models
#include "aeif_cond_alpha.h"
#include "aeif_cond_alpha_RK5.h"
#include "aeif_cond_alpha_multisynapse.h"
#include "aeif_cond_beta_multisynapse.h"
#include "aeif_cond_exp.h"
#include "aeif_psc_alpha.h"
#include "aeif_psc_delta.h"
#include "aeif_psc_exp.h"
#include "amat2_psc_exp.h"
#include "erfc_neuron.h"
#include "gauss_rate.h"
#include "gif_cond_exp.h"
#include "gif_cond_exp_multisynapse.h"
#include "gif_pop_psc_exp.h"
#include "gif_psc_exp.h"
#include "gif_psc_exp_multisynapse.h"
#include "ginzburg_neuron.h"
#include "hh_cond_exp_traub.h"
#include "hh_psc_alpha.h"
#include "hh_psc_alpha_gap.h"
#include "ht_neuron.h"
#include "iaf_chs_2007.h"
#include "iaf_chxk_2008.h"
#include "iaf_cond_alpha.h"
#include "iaf_cond_alpha_mc.h"
#include "iaf_cond_exp.h"
#include "iaf_cond_exp_sfa_rr.h"
#include "iaf_psc_alpha.h"
#include "iaf_psc_alpha_multisynapse.h"
#include "iaf_psc_delta.h"
#include "iaf_psc_exp.h"
#include "iaf_psc_exp_multisynapse.h"
#include "iaf_tum_2000.h"
#include "izhikevich.h"
#include "lin_rate.h"
#include "mat2_psc_exp.h"
#include "mcculloch_pitts_neuron.h"
#include "parrot_neuron.h"
#include "pp_pop_psc_delta.h"
#include "pp_psc_delta.h"
#include "siegert_neuron.h"
#include "sigmoid_rate.h"
#include "sigmoid_rate_gg_1998.h"
#include "tanh_rate.h"
#include "threshold_lin_rate.h"

// Stimulation devices
#include "ac_generator.h"
#include "dc_generator.h"
#include "gamma_sup_generator.h"
#include "inhomogeneous_poisson_generator.h"
#include "mip_generator.h"
#include "noise_generator.h"
#include "poisson_generator.h"
#include "ppd_sup_generator.h"
#include "pulsepacket_generator.h"
#include "sinusoidal_gamma_generator.h"
#include "sinusoidal_poisson_generator.h"
#include "spike_generator.h"
#include "step_current_generator.h"

// Recording devices
#include "correlation_detector.h"
#include "correlomatrix_detector.h"
#include "correlospinmatrix_detector.h"
#include "multimeter.h"
#include "voltmeter.h"
#include "spike_detector.h"
#include "spin_detector.h"
#include "weight_recorder.h"

#include "volume_transmitter.h"

// Prototypes for synapses
#include "bernoulli_connection.h"
#include "common_synapse_properties.h"
#include "cont_delay_connection.h"
#include "cont_delay_connection_impl.h"
#include "diffusion_connection.h"
#include "gap_junction.h"
#include "ht_connection.h"
#include "quantal_stp_connection.h"
#include "quantal_stp_connection_impl.h"
#include "rate_connection_delayed.h"
#include "rate_connection_instantaneous.h"
#include "spike_dilutor.h"
#include "static_connection.h"
#include "static_connection_hom_w.h"
#include "stdp_connection.h"
#include "stdp_connection_facetshw_hom.h"
#include "stdp_connection_facetshw_hom_impl.h"
#include "stdp_connection_hom.h"
#include "stdp_dopa_connection.h"
#include "stdp_pl_connection_hom.h"
#include "stdp_triplet_connection.h"
#include "tsodyks2_connection.h"
#include "tsodyks_connection.h"
#include "tsodyks_connection_hom.h"
#include "vogels_sprekeler_connection.h"

// Includes from nestkernel:
#include "common_synapse_properties.h"
#include "connector_model_impl.h"
#include "genericmodel.h"
#include "kernel_manager.h"
#include "model.h"
#include "model_manager_impl.h"
#include "target_identifier.h"

#ifdef HAVE_MUSIC
#include "music_cont_in_proxy.h"
#include "music_cont_out_proxy.h"
#include "music_event_in_proxy.h"
#include "music_event_out_proxy.h"
#include "music_message_in_proxy.h"
#endif


namespace nest
{
/**
 * Module supplying all models that are included in the NEST release.
 * @todo Should this be a dynamic module?
 */
class ModelsModule : public SLIModule
{
public:
  ModelsModule();
  ~ModelsModule();

  /**
   * Initialize module by registering models with the network.
   * @param SLIInterpreter* SLI interpreter
   */
  void init( SLIInterpreter* );

  const std::string name( void ) const;
  const std::string commandstring( void ) const;

private:

  /**
   * Registers a particular synapse model with the kernel. The type of the
   * added synapse is `ConnectionClassT`. The name of the synapse should be
   * passed as the `name` parameter.
   *
   * The synapse model is added three times:
   * - once for the normal indexing type (TargetIdentifierPtrRport)
   * - once for the ConnectionLabel-wrapped type (ConnectionLabel<
   *   ConnectionClassT < TargetIdentifierPtrRport > > >); the name is
   *   suffixed with "_lbl"
   * - once for the high-performance indexing type
   *   (TargetIdentifierType); the name is suffixed with "_hpc"
   *
   * This is a helper function for internal use in the ModelsModule. It
   * is called during construction of the ModelsModule and is not
   * intended to be used outside of this.
  **/
/*  template < typename ConnectionClassT >
  void register_connection_model( const std::string name );*/



};





template < class ConnectionClassT < Q >  >
void
register_connection_model( const std::string name )
{
  kernel()
    .model_manager.register_connection_model< ConnectionClassT < TargetIdentifierPtrRport > >(
      name );
  kernel()
    .model_manager.register_connection_model< ConnectionLabel < ConnectionClassT < TargetIdentifierPtrRport > > >(
      name + "_lbl" );
  kernel()
    .model_manager.register_connection_model< ConnectionClassT < TargetIdentifierIndex > >(
      name + "_hpc" );
}



} // namespace

#endif
