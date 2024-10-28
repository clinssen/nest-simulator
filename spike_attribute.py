import nest

pre, post = nest.Create("iaf_psc_exp", 2)
nest.Connect(pre, post, syn_spec={"synapse_model": "stdp_synapse"})

pre.I_e = 1E3    # [pA]

nest.Simulate(20.)

