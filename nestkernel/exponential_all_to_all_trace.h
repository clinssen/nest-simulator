/*
 *  exponential_all_to_all_trace.h
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

#ifndef EXPONENTIAL_ALL_TO_ALL_H
#define EXPONENTIAL_ALL_TO_ALL_H

// C++ includes:
#include <deque>

// Includes from nestkernel:
#include "trace.h"
#include "dict.h"
#include "dictdatum.h"
#include "dictutils.h"
#include "nest_names.h"


namespace nest
{


/**
 * \class ExponentialAllToAllTrace
 */
class ExponentialAllToAllTrace : public Trace
{

public:
  ExponentialAllToAllTrace(const DictionaryDatum &parms);
  // ExponentialAllToAllTrace( const Archiving_Node& );
  void updateTrace(const Time& t_sp) {
      std::cout << "\t* Updating ExponentialAllToAllTrace to register spike at t = " << t_sp.get_ms() << std::endl;
      tr += 1.;
  };
  double getTraceValue(const Time& t) { return 0.;};
  void reset() {
      std::cout << "\t* Resetting ExponentialAllToAllTrace" << std::endl;
      tr = 0.;
  }
protected:

private:
	double tr;
	double tau;
};

} // of namespace
#endif
