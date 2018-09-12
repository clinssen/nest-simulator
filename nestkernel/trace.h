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

/**
 * \file archiving_node.h
 * Definition of Archiving_Node which is capable of
 * recording and managing a spike history.
 * \author Moritz Helias, Abigail Morrison
 * \date april 2006
 */

#ifndef TRACE_H
#define TRACE_H

#include "nest_time.h"

namespace nest
{

/**
 * \class Trace
 */
class Trace
{

public:
	virtual void updateTrace(const Time& t_sp) = 0;
	virtual double getTraceValue(const Time& t) = 0;
	virtual void reset() = 0;

protected:

private:

};

} // of namespace
#endif
