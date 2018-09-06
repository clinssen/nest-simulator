# Copyright (c) 2014, Felix Schmitt, Axel Huebl, Rene Widera
# All rights reserved.
# 
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#     * Redistributions of source code must retain the above copyright
#       notice, this list of conditions and the following disclaimer.
#     * Redistributions in binary form must reproduce the above copyright
#       notice, this list of conditions and the following disclaimer in the
#       documentation and/or other materials provided with the distribution.
#     * Neither the name of the PIConGPU nor the
#       names of its contributors may be used to endorse or promote products
#       derived from this software without specific prior written permission.
# 
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
# ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
# WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
# DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
# (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
# LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
# ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
# SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

################################################################################
# Score-P
################################################################################

option(SCOREP_ENABLE "Create with Score-P support" OFF)

if (SCOREP_ENABLE)
	message(STATUS "Building with Score-P support")
	set(SCOREP_ROOT "$ENV{SCOREP_ROOT}")
	if(NOT SCOREP_ROOT)
		message(FATAL_ERROR "Environment variable SCOREP_ROOT not set!")
	endif(NOT SCOREP_ROOT)

	# compile flags
	execute_process(COMMAND $ENV{SCOREP_ROOT}/bin/scorep-config --nocompiler --cflags
					OUTPUT_VARIABLE SCOREP_COMPILEFLAGS
					RESULT_VARIABLE SCOREP_CONFIG_RETURN)
	if(NOT SCOREP_CONFIG_RETURN EQUAL 0)
		message(FATAL_ERROR "Can NOT execute 'scorep-config' at $ENV{SCOREP_ROOT}/bin/scorep-config - check file permissions")
	endif()

	# link flags
	execute_process(COMMAND $ENV{SCOREP_ROOT}/bin/scorep-config --mpp=mpi --ldflags
					OUTPUT_VARIABLE SCOREP_LINKFLAGS)
	# libraries
	execute_process(COMMAND $ENV{SCOREP_ROOT}/bin/scorep-config --mpp=mpi --libs
					OUTPUT_VARIABLE SCOREP_LIBFLAGS)
	string(STRIP "${SCOREP_LIBFLAGS}" SCOREP_LIBFLAGS)

	# subsystem iniialization file
	execute_process(COMMAND $ENV{SCOREP_ROOT}/bin/scorep-config --mpp=mpi --adapter-init
					OUTPUT_VARIABLE SCOREP_INIT_FILE)
	file(WRITE ${CMAKE_BINARY_DIR}/scorep_init.c "${SCOREP_INIT_FILE}")

	if(SCOREP_ENABLE)
		set(SCOREP_SRCFILES "${CMAKE_BINARY_DIR}/scorep_init.c")
	endif(SCOREP_ENABLE)

	# modify our flags
	set(CMAKE_CXX_LINK_FLAGS "${CMAKE_CXX_LINK_FLAGS} ${SCOREP_LINKFLAGS}")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${SCOREP_COMPILEFLAGS}")
endif(SCOREP_ENABLE)


