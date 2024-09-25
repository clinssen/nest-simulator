import nest

nest.resolution = 1.

src = nest.Create("arbitrary_data_sender_neuron")

tgt = nest.Create("arbitrary_data_receiver_neuron")

nest.Connect(src, tgt, syn_spec={"synapse_model": "arbitrary_data_synapse"})

nest.Simulate(100)

