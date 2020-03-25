# Copyright © 2020, Intel Corporation
#
# Permission is hereby granted, free of charge, to any person obtaining a
# copy of this software and associated documentation files (the "Software"),
# to deal in the Software without restriction, including without limitation
# the rights to use, copy, modify, merge, publish, distribute, sublicense,
# and/or sell copies of the Software, and to permit persons to whom the
# Software is furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included
# in all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
# THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
# FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
# IN THE SOFTWARE.
#################################################################################

#################################################################################
# HELPER FUNCTIONS
#################################################################################

#################################################################################
# clearInputVariables
#   Clears cached Metrics Library input cmake variables. Needed for e.g. the below command scenario:
#   1. cmake -DCMAKE_BUILD_TYPE=release -DML_ARCH=32 -> ML_ARCH specified to 32
#   2. cmake -DCMAKE_BUILD_TYPE=release              -> default ML_ARCH value should be used,
#                                                       but without clearing, it will be still ML_ARCH=32
#################################################################################
function (clearInputVariables)
    unset (CMAKE_BUILD_TYPE CACHE)
    unset (ML_ARCH CACHE)
endfunction ()

#################################################################################
# errorExit
#   Clears cached input variables and displays the given error message.
#################################################################################
function (errorExit message)
    clearInputVariables ()
    message (FATAL_ERROR ${message})
endfunction ()

#################################################################################
# ADAPTER
#################################################################################

if (${CMAKE_VERBOSE_MAKEFILE} STREQUAL ON)
    message ("INFO: Input CMAKE_BUILD_TYPE = ${CMAKE_BUILD_TYPE}")
    message ("INFO: Input ML_ARCH          = ${ML_ARCH}")
endif()

# PLATFORM
if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(PLATFORM "linux")
else()
    errorExit("ERROR: Project isn't targeted for ${CMAKE_SYSTEM_NAME}. Please run CMake on Linux.")
endif()

# BUILD_TYPE
if (NOT CMAKE_BUILD_TYPE)
    set (CMAKE_BUILD_TYPE "release")
else ()
    string (TOLOWER ${CMAKE_BUILD_TYPE} CMAKE_BUILD_TYPE)
    if (NOT (CMAKE_BUILD_TYPE STREQUAL "release" OR
             CMAKE_BUILD_TYPE STREQUAL "release-internal" OR
             CMAKE_BUILD_TYPE STREQUAL "debug"))
        errorExit("ERROR: Specify correct CMAKE_BUILD_TYPE (-DCMAKE_BUILD_TYPE=release|release-internal|debug)")
    endif ()
endif ()

# HOST ARCH
if (CMAKE_HOST_SYSTEM_PROCESSOR STREQUAL "x86_64") # 64 bits Linux
    set (HOST_ARCH "64")
else () # 32 bits Linux
    set (HOST_ARCH "32")
endif ()

# TARGET ARCH
if (ML_ARCH)
# User defines ARCH through custom variable
    if (ML_ARCH STREQUAL "32" OR ML_ARCH STREQUAL "64")
        set (TARGET_ARCH ${ML_ARCH})
    else ()
        errorExit("ERROR: Specify correct ML_ARCH (-DML_ARCH=64|32).")
    endif ()
else ()
# User defines ARCH through compiler flags
    set (TARGET_ARCH "64") # 64 bits by default
    if (CMAKE_SIZEOF_VOID_P EQUAL 4)
        set (TARGET_ARCH "32")
    endif ()
endif ()

message ("-- using TARGET_ARCH = ${TARGET_ARCH}")

if (TARGET_ARCH STREQUAL "32")
    message ("-- !! Compiling 32bit on 64bit host requires gcc (g++) in multilib version !!")
    message ("   e.g. sudo apt-get install gcc-4.8-multilib g++-4.8-multilib")
endif ()

# PATHS
set (BS_DIR_INSTRUMENTATION ${CMAKE_CURRENT_SOURCE_DIR}/instrumentation)
set (BS_DIR_INC ${CMAKE_CURRENT_SOURCE_DIR}/inc)
set (BS_DIR_EXTERNAL ${CMAKE_CURRENT_SOURCE_DIR}/external)
set (DUMP_DIR ${CMAKE_CURRENT_SOURCE_DIR}/dump/${PLATFORM}${TARGET_ARCH}/${CMAKE_BUILD_TYPE}/${PROJECT_NAME})
if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set (CMAKE_INSTALL_PREFIX "/usr" CACHE PATH "..." FORCE)
endif ()

# OUTPUT
set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${DUMP_DIR})
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${DUMP_DIR})
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${DUMP_DIR})

# Clear input variables to have a 'fresh', non-cached values during next cmake execution
clearInputVariables ()
