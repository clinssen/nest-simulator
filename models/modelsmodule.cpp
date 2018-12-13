/*
 *  modelsmodule.cpp
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

/*
    This file is part of NEST.

    modelsmodule.cpp -- sets up the modeldict with all models included
    with the NEST distribution.

    Author(s):
    Marc-Oliver Gewaltig
    R"udiger Kupper
    Hans Ekkehard Plesser

    First Version: June 2006
*/

#include "modelsmodule.h"


namespace nest
{
// At the time when ModelsModule is constructed, the SLI Interpreter
// must already be initialized. ModelsModule relies on the presence of
// the following SLI datastructures: Name, Dictionary
ModelsModule::ModelsModule()
{
}

ModelsModule::~ModelsModule()
{
}

const std::string
ModelsModule::name( void ) const
{
  return std::string(
    "NEST Standard Models Module" ); // Return name of the module
}

const std::string
ModelsModule::commandstring( void ) const
{
  // TODO: Move models-init.sli to sli_neuron....
  return std::string( "(models-init) run" );
}

//-------------------------------------------------------------------------------------

void
ModelsModule::init( SLIInterpreter* )
{
  // rate models with input noise
  kernel().model_manager.register_node_model< gauss_rate_ipn >(
    "gauss_rate_ipn" );
  kernel().model_manager.register_node_model< lin_rate_ipn >( "lin_rate_ipn" );
  kernel().model_manager.register_node_model< sigmoid_rate_ipn >(
    "sigmoid_rate_ipn" );
  kernel().model_manager.register_node_model< sigmoid_rate_gg_1998_ipn >(
    "sigmoid_rate_gg_1998_ipn" );
  kernel().model_manager.register_node_model< tanh_rate_ipn >(
    "tanh_rate_ipn" );
  kernel().model_manager.register_node_model< threshold_lin_rate_ipn >(
    "threshold_lin_rate_ipn" );

  // rate models with output noise
  kernel().model_manager.register_node_model< lin_rate_opn >( "lin_rate_opn" );
  kernel().model_manager.register_node_model< tanh_rate_opn >(
    "tanh_rate_opn" );
  kernel().model_manager.register_node_model< threshold_lin_rate_opn >(
    "threshold_lin_rate_opn" );

  // rate transformer nodes
  kernel().model_manager.register_node_model< rate_transformer_gauss >(
    "rate_transformer_gauss" );
  kernel().model_manager.register_node_model< rate_transformer_lin >(
    "rate_transformer_lin" );
  kernel().model_manager.register_node_model< rate_transformer_sigmoid >(
    "rate_transformer_sigmoid" );
  kernel()
    .model_manager.register_node_model< rate_transformer_sigmoid_gg_1998 >(
      "rate_transformer_sigmoid_gg_1998" );
  kernel().model_manager.register_node_model< rate_transformer_tanh >(
    "rate_transformer_tanh" );
  kernel().model_manager.register_node_model< rate_transformer_threshold_lin >(
    "rate_transformer_threshold_lin" );

  kernel().model_manager.register_node_model< iaf_chs_2007 >( "iaf_chs_2007" );
  kernel().model_manager.register_node_model< iaf_psc_alpha >(
    "iaf_psc_alpha" );
  kernel().model_manager.register_node_model< iaf_psc_alpha_multisynapse >(
    "iaf_psc_alpha_multisynapse" );
  kernel().model_manager.register_node_model< iaf_psc_delta >(
    "iaf_psc_delta" );
  kernel().model_manager.register_node_model< iaf_psc_exp >( "iaf_psc_exp" );
  kernel().model_manager.register_node_model< iaf_psc_exp_multisynapse >(
    "iaf_psc_exp_multisynapse" );
  kernel().model_manager.register_node_model< iaf_tum_2000 >( "iaf_tum_2000" );
  kernel().model_manager.register_node_model< amat2_psc_exp >(
    "amat2_psc_exp" );
  kernel().model_manager.register_node_model< mat2_psc_exp >( "mat2_psc_exp" );
  kernel().model_manager.register_node_model< parrot_neuron >(
    "parrot_neuron" );
  kernel().model_manager.register_node_model< pp_psc_delta >( "pp_psc_delta" );
  kernel().model_manager.register_node_model< pp_pop_psc_delta >(
    "pp_pop_psc_delta" );
  kernel().model_manager.register_node_model< gif_psc_exp >( "gif_psc_exp" );
  kernel().model_manager.register_node_model< gif_psc_exp_multisynapse >(
    "gif_psc_exp_multisynapse" );

  kernel().model_manager.register_node_model< ac_generator >( "ac_generator" );
  kernel().model_manager.register_node_model< dc_generator >( "dc_generator" );
  kernel().model_manager.register_node_model< spike_generator >(
    "spike_generator" );
  kernel().model_manager.register_node_model< inhomogeneous_poisson_generator >(
    "inhomogeneous_poisson_generator" );
  kernel().model_manager.register_node_model< poisson_generator >(
    "poisson_generator" );
  kernel().model_manager.register_node_model< pulsepacket_generator >(
    "pulsepacket_generator" );
  kernel().model_manager.register_node_model< noise_generator >(
    "noise_generator" );
  kernel().model_manager.register_node_model< step_current_generator >(
    "step_current_generator" );
  kernel().model_manager.register_node_model< mip_generator >(
    "mip_generator" );
  kernel().model_manager.register_node_model< sinusoidal_poisson_generator >(
    "sinusoidal_poisson_generator" );
  kernel().model_manager.register_node_model< ppd_sup_generator >(
    "ppd_sup_generator" );
  kernel().model_manager.register_node_model< gamma_sup_generator >(
    "gamma_sup_generator" );
  kernel().model_manager.register_node_model< erfc_neuron >( "erfc_neuron" );
  kernel().model_manager.register_node_model< ginzburg_neuron >(
    "ginzburg_neuron" );
  kernel().model_manager.register_node_model< mcculloch_pitts_neuron >(
    "mcculloch_pitts_neuron" );
  kernel().model_manager.register_node_model< izhikevich >( "izhikevich" );
  kernel().model_manager.register_node_model< spike_dilutor >(
    "spike_dilutor" );

  kernel().model_manager.register_node_model< spike_detector >(
    "spike_detector" );
  kernel().model_manager.register_node_model< weight_recorder >(
    "weight_recorder" );
  kernel().model_manager.register_node_model< spin_detector >(
    "spin_detector" );
  kernel().model_manager.register_node_model< Multimeter >( "multimeter" );
  kernel().model_manager.register_node_model< Voltmeter >( "voltmeter" );
  kernel().model_manager.register_node_model< correlation_detector >(
    "correlation_detector" );
  kernel().model_manager.register_node_model< correlomatrix_detector >(
    "correlomatrix_detector" );
  kernel().model_manager.register_node_model< correlospinmatrix_detector >(
    "correlospinmatrix_detector" );
  kernel().model_manager.register_node_model< volume_transmitter >(
    "volume_transmitter" );

#ifdef HAVE_GSL
  kernel().model_manager.register_node_model< iaf_chxk_2008 >(
    "iaf_chxk_2008" );
  kernel().model_manager.register_node_model< iaf_cond_alpha >(
    "iaf_cond_alpha" );
  kernel().model_manager.register_node_model< iaf_cond_exp >( "iaf_cond_exp" );
  kernel().model_manager.register_node_model< iaf_cond_exp_sfa_rr >(
    "iaf_cond_exp_sfa_rr" );
  kernel().model_manager.register_node_model< iaf_cond_alpha_mc >(
    "iaf_cond_alpha_mc" );
  kernel().model_manager.register_node_model< hh_psc_alpha >( "hh_psc_alpha" );
  kernel().model_manager.register_node_model< hh_psc_alpha_gap >(
    "hh_psc_alpha_gap" );
  kernel().model_manager.register_node_model< hh_cond_exp_traub >(
    "hh_cond_exp_traub" );
  kernel().model_manager.register_node_model< sinusoidal_gamma_generator >(
    "sinusoidal_gamma_generator" );
  kernel().model_manager.register_node_model< gif_cond_exp >( "gif_cond_exp" );
  kernel().model_manager.register_node_model< gif_cond_exp_multisynapse >(
    "gif_cond_exp_multisynapse" );
  kernel().model_manager.register_node_model< gif_pop_psc_exp >(
    "gif_pop_psc_exp" );

  kernel().model_manager.register_node_model< aeif_cond_alpha >(
    "aeif_cond_alpha" );
  kernel().model_manager.register_node_model< aeif_cond_exp >(
    "aeif_cond_exp" );
  kernel().model_manager.register_node_model< aeif_psc_alpha >(
    "aeif_psc_alpha" );
  kernel().model_manager.register_node_model< aeif_psc_exp >( "aeif_psc_exp" );
  kernel().model_manager.register_node_model< aeif_psc_delta >(
    "aeif_psc_delta" );
  kernel().model_manager.register_node_model< ht_neuron >( "ht_neuron" );
  kernel().model_manager.register_node_model< aeif_cond_beta_multisynapse >(
    "aeif_cond_beta_multisynapse" );
  kernel().model_manager.register_node_model< aeif_cond_alpha_multisynapse >(
    "aeif_cond_alpha_multisynapse" );
  kernel().model_manager.register_node_model< siegert_neuron >(
    "siegert_neuron" );
#endif

  // This version of the AdEx model does not depend on GSL.
  kernel().model_manager.register_node_model< aeif_cond_alpha_RK5 >(
    "aeif_cond_alpha_RK5",
    /*private_model*/ false,
    /*deprecation_info*/ "NEST 3.0" );

#ifdef HAVE_MUSIC
  // proxies for inter-application communication using MUSIC
  kernel().model_manager.register_node_model< music_event_in_proxy >(
    "music_event_in_proxy" );
  kernel().model_manager.register_node_model< music_event_out_proxy >(
    "music_event_out_proxy" );
  kernel().model_manager.register_node_model< music_cont_in_proxy >(
    "music_cont_in_proxy" );
  kernel().model_manager.register_node_model< music_cont_out_proxy >(
    "music_cont_out_proxy" );
  kernel().model_manager.register_node_model< music_message_in_proxy >(
    "music_message_in_proxy" );
#endif

  // register all connection models
  register_connection_model< StaticConnection >(std::string("static_synapse"));
/*  register_connection_model< StaticConnectionHomW >("static_synapse_hom_w");
  register_connection_model< STDPConnection >("stdp_synapse");
  register_connection_model< STDPPLConnectionHom("stdp_pl_synapse_hom");
  register_connection_model< STDPTripletConnection >("stdp_triplet_synapse");
  register_connection_model< Quantal_StpConnection >("quantal_stp_synapse");
  register_connection_model< STDPConnectionHom >("stdp_synapse_hom");
  register_connection_model< STDPFACETSHWConnectionHom >("stdp_facetshw_synapse_hom");
  register_connection_model< ContDelayConnection>("cont_delay_synapse");
  register_connection_model< TsodyksConnection >("tsodyks_synapse");
  register_connection_model< TsodyksConnectionHom >("tsodyks_synapse_hom");
  register_connection_model< Tsodyks2Connection >("tsodyks2_synapse");
  register_connection_model< HTConnection >("ht_synapse");
  register_connection_model< STDPDopaConnection >("stdp_dopamine_synapse");
  register_connection_model< VogelsSprekelerConnection >("vogels_sprekeler_synapse");
  register_connection_model< BernoulliConnection>("bernoulli_synapse");*/

  // register secondary connection models
  kernel()
    .model_manager
    .register_secondary_connection_model< GapJunction< TargetIdentifierPtrRport > >(
      "gap_junction",
      /*has_delay=*/false,
      /*requires_symmetric=*/true,
      /*supports_wfr=*/true );
  kernel()
    .model_manager
    .register_secondary_connection_model< RateConnectionInstantaneous< TargetIdentifierPtrRport > >(
      "rate_connection_instantaneous",
      /*has_delay=*/false,
      /*requires_symmetric=*/false,
      /*supports_wfr=*/true );
  kernel()
    .model_manager
    .register_secondary_connection_model< RateConnectionDelayed< TargetIdentifierPtrRport > >(
      "rate_connection_delayed",
      /*has_delay=*/true,
      /*requires_symmetric=*/false,
      /*supports_wfr=*/false );
  kernel()
    .model_manager
    .register_secondary_connection_model< DiffusionConnection< TargetIdentifierPtrRport > >(
      "diffusion_connection",
      /*has_delay=*/false,
      /*requires_symmetric=*/false,
      /*supports_wfr=*/true );
}


/*template < typename ConnectionClassT >
void
ModelsModule::register_connection_model( const std::string name )
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
}*/<

} // namespace nest
