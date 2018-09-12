# -*- coding: utf-8 -*-
#
# test_stdp_multiplicity.py
#
# This file is part of NEST.
#
# Copyright (C) 2004 The NEST Initiative
#
# NEST is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 2 of the License, or
# (at your option) any later version.
#
# NEST is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with NEST.  If not, see <http://www.gnu.org/licenses/>.

# This script tests the parrot_neuron in NEST.

import nest
import unittest
import math
import numpy as np


@nest.check_stack
class StdpSynapse(unittest.TestCase):
    """
    ...
    """

    def run_protocol(self, pre_post_shift):
        """
        """

        resolution = .1     # [ms]

        delay = 1.  # [ms]

        # pre_spike_times = [100., 200., 300.]      # [ms]
        # post_spike_times = [150., 250., 350.]      # [ms]

        # pre_spike_times = [220., 300.]      # [ms]
        # post_spike_times = [150., 250., 350.]      # [ms]

        # pre_spike_times = [301., 302.]      # [ms]
        # post_spike_times = [100., 300.]      # [ms]

        # pre_spike_times = [   4.,   6.  , 6.]    # [ms]
        # post_spike_times = [  2.,   6. ]      # [ms]

        pre_spike_times = 1 + np.round(100 * np.sort(np.abs(np.random.randn(100))))      # [ms]
        post_spike_times =  np.sort(np.unique(1 + np.round(100 * np.sort(np.abs(np.random.randn(100))))))     # [ms]

        print("Pre spike times: " + str(pre_spike_times))
        print("Post spike times: " + str(post_spike_times))

        nest.set_verbosity("M_WARNING")

        post_weights = {'parrot': [], 'parrot_ps': []}

        nest.ResetKernel()
        nest.SetKernelStatus({'resolution': resolution})

        wr = nest.Create('weight_recorder')
        nest.CopyModel("stdp_synapse", "stdp_synapse_rec",
                       {"weight_recorder": wr[0], "weight": 1.})

        # create spike_generators with these times
        pre_sg = nest.Create("spike_generator",
                             params={"spike_times": pre_spike_times,
                                     'allow_offgrid_spikes': True})
        post_sg = nest.Create("spike_generator",
                              params={"spike_times": post_spike_times,
                                      'allow_offgrid_spikes': True})
        pre_sg_ps = nest.Create("spike_generator",
                                params={"spike_times": pre_spike_times,
                                        'precise_times': True})
        post_sg_ps = nest.Create("spike_generator",
                                 params={"spike_times": post_spike_times,
                                         'precise_times': True})

        # create parrot neurons and connect spike_generators
        pre_parrot = nest.Create("parrot_neuron")
        post_parrot = nest.Create("parrot_neuron")
        pre_parrot_ps = nest.Create("parrot_neuron_ps")
        post_parrot_ps = nest.Create("parrot_neuron_ps")

        nest.Connect(pre_sg, pre_parrot,
                     syn_spec={"delay": delay})
        nest.Connect(post_sg, post_parrot,
                     syn_spec={"delay": delay})
        nest.Connect(pre_sg_ps, pre_parrot_ps,
                     syn_spec={"delay": delay})
        nest.Connect(post_sg_ps, post_parrot_ps,
                     syn_spec={"delay": delay})

        # create spike detector --- debugging only
        spikes = nest.Create("spike_detector",
                             params={'precise_times': True})
        nest.Connect(
            pre_parrot + post_parrot +
            pre_parrot_ps + post_parrot_ps,
            spikes
        )

        # connect both parrot neurons with a stdp synapse onto port 1
        # thereby spikes transmitted through the stdp connection are
        # not repeated postsynaptically.
        nest.Connect(
            pre_parrot, post_parrot,
            syn_spec={'model': 'stdp_synapse_rec', 'receptor_type': 1})
        nest.Connect(
            pre_parrot_ps, post_parrot_ps,
            syn_spec={'model': 'stdp_synapse_rec', 'receptor_type': 1})

        # get STDP synapse and weight before protocol
        syn = nest.GetConnections(source=pre_parrot,
                                  synapse_model="stdp_synapse_rec")
        w_pre = nest.GetStatus(syn)[0]['weight']
        syn_ps = nest.GetConnections(source=pre_parrot_ps,
                                     synapse_model="stdp_synapse_rec")
        w_pre_ps = nest.GetStatus(syn)[0]['weight']

        print()
        print("[py] w_pre = " + str(w_pre))
        print("[py] w_pre_ps = " + str(w_pre_ps))
        print()

        sim_time = np.amax(np.concatenate((pre_spike_times, post_spike_times))) + 5 * delay
        nest.Simulate(sim_time)

        # get weight post protocol
        w_post = nest.GetStatus(syn)[0]['weight']
        w_post_ps = nest.GetStatus(syn_ps)[0]['weight']

        import pdb;pdb.set_trace()

        print()
        print("[py] w_post = " + str(w_post))
        print("[py] w_post_ps = " + str(w_post_ps))
        print()

        wr_weights = nest.GetStatus(wr, "events")[0]["weights"]
        print("[py] wr_weights = " + str(wr_weights))


        assert w_post != w_pre, "Plain parrot weight did not change."
        assert w_post_ps != w_pre_ps, "Precise parrot \
            weight did not change."

        post_weights['parrot'].append(w_post)
        post_weights['parrot_ps'].append(w_post_ps)

        return post_weights

    def test_ParrotNeuronSTDPProtocolPotentiation(self):
        """Check weight convergence on potentiation."""

        post_weights = self.run_protocol(pre_post_shift=10.0)
        w_plain = np.array(post_weights['parrot'])
        w_precise = np.array(post_weights['parrot_ps'])

        assert all(w_plain == w_plain[0]), 'Plain weights differ'
        dw = w_precise - w_plain
        dwrel = dw[1:] / dw[:-1]
        assert all(np.round(dwrel, decimals=3) ==
                   0.5), 'Precise weights do not converge.'

    # def test_ParrotNeuronSTDPProtocolDepression(self):
    #     """Check weight convergence on depression."""

    #     post_weights = self.run_protocol(pre_post_shift=-10.0)
    #     w_plain = np.array(post_weights['parrot'])
    #     w_precise = np.array(post_weights['parrot_ps'])

    #     assert all(w_plain == w_plain[0]), 'Plain weights differ'
    #     dw = w_precise - w_plain
    #     dwrel = dw[1:] / dw[:-1]
    #     assert all(np.round(dwrel, decimals=3) ==
    #                0.5), 'Precise weights do not converge.'


def suite():

    # makeSuite is sort of obsolete http://bugs.python.org/issue2721
    # using loadTestsFromTestCase instead.
    suite = unittest.TestLoader().loadTestsFromTestCase(StdpSynapse)
    return unittest.TestSuite([suite])


def run():
    runner = unittest.TextTestRunner(verbosity=99)
    runner.run(suite())


if __name__ == "__main__":
    #unittest.findTestCases(__main__).debug()
    run()
