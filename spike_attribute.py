import nest



pre, post = nest.Create("iaf_psc_exp", 2)
nest.Connect(pre, post)

pre.I_e = 1E3    # [pA]

nest.Simulate(20.)

