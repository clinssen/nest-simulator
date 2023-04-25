# -*- coding: utf-8 -*-
#
# test_gif_exp.py
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

import nest
import numpy as np
import pytest


@pytest.mark.skipif_missing_gsl
@pytest.mark.parametrize("model", ["gif_psc_exp", "gif_cond_exp"])
class TestGifCondExp:
    r"""
    Test of gif_cond_exp with external DC current and spike generators

    This testscript checks the correctness of gif_cond_exp neuron model. It
    injects external DC current and also three spikes to neuron and measures its
    spike times. The spike times should match the expected values. The expected
    spike times are computed by the Python code used for the publication
    (Code/GIF.py in http://wiki.epfl.ch/giftoolbox/documents/GIF_Toolbox.zip).
    In order to use the Python code, the adaptation parameters (q_stc/sfa) should
    be converted using the formula described in the model documentation.
    """

    def run_simulation(self, model, params=None):
        nest.ResetKernel()
        nest.resolution = .1
        nest.rng_seed = 1

        neuron = nest.Create(model)
        if params:
            neuron.set(params)

        sg = nest.Create("spike_generator", params={"spike_times": [10.0, 20.0, 30.0]})
        dc_gen = nest.Create("dc_generator", params={"amplitude": 170.})
        sr = nest.Create("spike_recorder", params={"time_in_steps": True})
        nest.Connect(dc_gen, neuron)
        nest.Connect(neuron, sr)
        nest.Connect(sg, neuron)

        nest.Simulate(150.0)

        return sr.get("events")["times"]

    def test_gif_cond_exp_wrong_params(self, model):
        """Test for wrong parameters (negative lambda)"""
        params = {"lambda_0": -10.0}
        with pytest.raises(nest.kernel.NESTError):
            self.run_simulation(model, params)

    def test_gif_cond_exp_wrong_params2(self, model):
        """Test for wrong parameters (unequal size of arrays)"""
        params = {"tau_sfa": 120.0,
                  "q_sfa": [10.0, 25.0]}
        with pytest.raises(nest.kernel.NESTError):
            self.run_simulation(model, params)

    def test_gif_cond_exp_default_params(self, model):
        """Test default parameters"""
        times = self.run_simulation(model)
        if model == "gif_cond_exp":
            np.testing.assert_allclose(times, [398, 740, 1101, 1500])
        else:
            assert model == "gif_psc_exp"
            np.testing.assert_allclose(times, [462, 850, 1288])

    def test_gif_cond_exp_defined_params(self, model):
        """Test defined parameters"""
        params = {"C_m": 40.0,
                  "Delta_V": 0.2,
                  "tau_syn_ex": 8.0,
                  "tau_sfa": [120.0, 10.0],
                  "q_sfa": [10.0, 25.0],
                  "tau_stc": [10.0, 20.0],
                  "q_stc": [20.0, -5.0]}

        times = self.run_simulation(model, params)

        if model == "gif_cond_exp":
            np.testing.assert_allclose(times, [171, 676])
        else:
            assert model == "gif_psc_exp"
            np.testing.assert_allclose(times, [207, 740])
