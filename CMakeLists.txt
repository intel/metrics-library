#=========================== begin_copyright_notice ============================
#
# Copyright (C) 2020-2023 Intel Corporation
#
# SPDX-License-Identifier: MIT
#
#============================ end_copyright_notice =============================

cmake_minimum_required (VERSION 3.5)

#################################################################################
# PROJECT
#################################################################################
set (PROJECT_NAME metrics_library)
project (${PROJECT_NAME})

set (LIB igdml)
set (LIB_NAME libigdml)

set (SOME_COMMON_INCLUDES FALSE)

# uncomment for logs during build (during 'make' command)
# set (CMAKE_VERBOSE_MAKEFILE ON)

# instrumentation common settings & macros
if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    include (ml_cmake_adapter.cmake NO_POLICY_SCOPE)
else()
    include (../instr.cmake NO_POLICY_SCOPE) # windows and others
endif()

# version
set (API_HEADER_LOCATION "${BS_DIR_INC}/common/instrumentation/api/metrics_library_api_1_0.h")
set (MAJOR_NUMBER_REGEX "^#define METRICS_LIBRARY_MAJOR_NUMBER ([0-9]+).*$")
set (MINOR_NUMBER_REGEX "^#define METRICS_LIBRARY_MINOR_NUMBER ([0-9]+).*$")
set (BUILD_NUMBER_REGEX "^#define METRICS_LIBRARY_BUILD_NUMBER ([0-9]+).*$")

set (LIB_MAJOR_VERSION 1)
set (LIB_MINOR_VERSION 0)
set (LIB_BUILD_VERSION 0)

file (STRINGS ${API_HEADER_LOCATION} MAJOR_NUMBER_LINE REGEX ${MAJOR_NUMBER_REGEX})
file (STRINGS ${API_HEADER_LOCATION} MINOR_NUMBER_LINE REGEX ${MINOR_NUMBER_REGEX})
file (STRINGS ${API_HEADER_LOCATION} BUILD_NUMBER_LINE REGEX ${BUILD_NUMBER_REGEX})

if (MAJOR_NUMBER_LINE)
    string (REGEX REPLACE ${MAJOR_NUMBER_REGEX} "\\1" LIB_MAJOR_VERSION ${MAJOR_NUMBER_LINE})
else ()
    message ("Major number has not been found, so the default one has been used.")
endif ()

if (MINOR_NUMBER_LINE)
    string (REGEX REPLACE ${MINOR_NUMBER_REGEX} "\\1" LIB_MINOR_VERSION ${MINOR_NUMBER_LINE})
else ()
    message ("Minor number has not been found, so the default one has been used.")
endif ()

if (BUILD_NUMBER_LINE)
    string (REGEX REPLACE ${BUILD_NUMBER_REGEX} "\\1" LIB_BUILD_VERSION ${BUILD_NUMBER_LINE})
else ()
    message ("Build number has not been found, so the default one has been used.")
endif ()

set (LIB_VERSION ${LIB_MAJOR_VERSION}.${LIB_MINOR_VERSION}.${LIB_BUILD_VERSION})

#################################################################################
# CONFIGURE C++ STANDARD
#################################################################################
set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

#################################################################################
# MACROS
#################################################################################
macro (ml_add_headers _platform)
    # library headers
    file (GLOB _LIBRARY_HEADERS
        ${BS_DIR_INSTRUMENTATION}/metrics_library/library/inc/${_platform}/*.h
        )
    set (_HEADERS ${_HEADERS} ${_LIBRARY_HEADERS})

    # generated traits headers
    file (GLOB _TRAITS_HEADERS
        ${BS_DIR_INSTRUMENTATION}/metrics_library/codegen/traits/${_platform}/*.h
        )
    set (_HEADERS ${_HEADERS} ${_TRAITS_HEADERS})

    # configuration traits headers
    file (GLOB _TRAITS_HEADERS
        ${BS_DIR_INSTRUMENTATION}/metrics_library/traits/configuration/${_platform}/*.h
        )
    set (_HEADERS ${_HEADERS} ${_TRAITS_HEADERS})

    # constants traits headers
    file (GLOB _TRAITS_HEADERS
        ${BS_DIR_INSTRUMENTATION}/metrics_library/traits/constants/${_platform}/*.h
        )
    set (_HEADERS ${_HEADERS} ${_TRAITS_HEADERS})

    # device traits headers
    file (GLOB _TRAITS_HEADERS
        ${BS_DIR_INSTRUMENTATION}/metrics_library/traits/device/${_platform}/*.h
        )
    set (_HEADERS ${_HEADERS} ${_TRAITS_HEADERS})

    # internal feature headers
    file (GLOB _TRAITS_HEADERS
        ${BS_DIR_INSTRUMENTATION}/metrics_library/traits/internal/${_platform}/*.h
        )
    set (_HEADERS ${_HEADERS} ${_TRAITS_HEADERS})

    # kernel interface traits headers
    file (GLOB _TRAITS_HEADERS
        ${BS_DIR_INSTRUMENTATION}/metrics_library/traits/kernel_interface/${_platform}/*.h
        )
    set (_HEADERS ${_HEADERS} ${_TRAITS_HEADERS})

    # marker traits headers
    file (GLOB _TRAITS_HEADERS
        ${BS_DIR_INSTRUMENTATION}/metrics_library/traits/marker/${_platform}/*.h
        )
    set (_HEADERS ${_HEADERS} ${_TRAITS_HEADERS})

    # override traits headers
    file (GLOB _TRAITS_HEADERS
        ${BS_DIR_INSTRUMENTATION}/metrics_library/traits/override/${_platform}/*.h
        )
    set (_HEADERS ${_HEADERS} ${_TRAITS_HEADERS})

    # policy traits headers
    file (GLOB _TRAITS_HEADERS
        ${BS_DIR_INSTRUMENTATION}/metrics_library/traits/policy/${_platform}/*.h
        )
    set (_HEADERS ${_HEADERS} ${_TRAITS_HEADERS})

    # tools traits headers
    file (GLOB _TRAITS_HEADERS
        ${BS_DIR_INSTRUMENTATION}/metrics_library/traits/tools/${_platform}/*.h
        )
    set (_HEADERS ${_HEADERS} ${_TRAITS_HEADERS})

    # validation traits headers
    file (GLOB _TRAITS_HEADERS
        ${BS_DIR_INSTRUMENTATION}/metrics_library/traits/validation/${_platform}/*.h
        )
    set (_HEADERS ${_HEADERS} ${_TRAITS_HEADERS})
endmacro ()

#################################################################################
# SOURCES
#################################################################################
# common sources
set (_SOURCES ${_SOURCES}
    # metrics library core
    ${BS_DIR_INSTRUMENTATION}/metrics_library/.clang-format
    ${BS_DIR_INSTRUMENTATION}/metrics_library/ddi/ml_ddi_command_buffer.hpp
    ${BS_DIR_INSTRUMENTATION}/metrics_library/ddi/ml_ddi_configuration.hpp
    ${BS_DIR_INSTRUMENTATION}/metrics_library/ddi/ml_ddi_context.hpp
    ${BS_DIR_INSTRUMENTATION}/metrics_library/ddi/ml_ddi_marker.hpp
    ${BS_DIR_INSTRUMENTATION}/metrics_library/ddi/ml_ddi_query.hpp
    ${BS_DIR_INSTRUMENTATION}/metrics_library/ddi/ml_ddi_report.hpp
    ${BS_DIR_INSTRUMENTATION}/metrics_library/ddi/ml_ddi_override.hpp
    ${BS_DIR_INSTRUMENTATION}/metrics_library/codegen/ddi/ml_ddi_command_buffer.cpp
    ${BS_DIR_INSTRUMENTATION}/metrics_library/codegen/ddi/ml_ddi_configuration.cpp
    ${BS_DIR_INSTRUMENTATION}/metrics_library/codegen/ddi/ml_ddi_context.cpp
    ${BS_DIR_INSTRUMENTATION}/metrics_library/codegen/ddi/ml_ddi_marker.cpp
    ${BS_DIR_INSTRUMENTATION}/metrics_library/codegen/ddi/ml_ddi_query.cpp
    ${BS_DIR_INSTRUMENTATION}/metrics_library/codegen/ddi/ml_ddi_report.cpp
    ${BS_DIR_INSTRUMENTATION}/metrics_library/codegen/ddi/ml_ddi_override.cpp
    ${BS_DIR_INSTRUMENTATION}/metrics_library/codegen/library/ml_export.def
    ${BS_DIR_INSTRUMENTATION}/metrics_library/codegen/library/ml_export.cpp
    # debug logs framework
    ${BS_DIR_INSTRUMENTATION}/utils/common/iu_debug.c
    )

# platform dependent
if (${PLATFORM} STREQUAL Windows) # windows
    set (_SOURCES ${_SOURCES}
        # debug logs framework
        ${BS_DIR_INSTRUMENTATION}/utils/windows/umd/iu_os.cpp
        ${BS_DIR_INSTRUMENTATION}/utils/windows/umd/iu_std.cpp
        )

    # linux files in solution explorer
    set (_LINUX_SOURCES_ON_WINDOWS
        # debug logs framework
        ${BS_DIR_INSTRUMENTATION}/utils/linux/iu_os.cpp
        ${BS_DIR_INSTRUMENTATION}/utils/linux/iu_std.cpp
        )

    # linux files will not be being built
    set_source_files_properties (
        ${_LINUX_SOURCES_ON_WINDOWS} PROPERTIES HEADER_FILE_ONLY ON
        )

    set (_SOURCES ${_SOURCES} ${_LINUX_SOURCES_ON_WINDOWS})
elseif (${PLATFORM} STREQUAL linux)
    set (_SOURCES ${_SOURCES}
        # debug logs framework
        ${BS_DIR_INSTRUMENTATION}/utils/linux/iu_os.cpp
        ${BS_DIR_INSTRUMENTATION}/utils/linux/iu_std.cpp
        )
else()
    message("-- Using platform is ${PLATFORM}")
endif ()

set (SOURCES ${_SOURCES})

#################################################################################
# HEADERS
#################################################################################
# metrics library api headers
set (_HEADERS ${_HEADERS}
    ${API_HEADER_LOCATION}
    )

# instrumentation utils common headers
file (GLOB _IU_COMMON_HEADERS
    ${BS_DIR_INSTRUMENTATION}/utils/common/inc/*.h
    )
set (_HEADERS ${_HEADERS} ${_IU_COMMON_HEADERS})

# library headers
file (GLOB _LIBRARY_HEADERS
    ${BS_DIR_INSTRUMENTATION}/metrics_library/library/inc/*.h
    )
set (_HEADERS ${_HEADERS} ${_LIBRARY_HEADERS})

# generated ddi headers
file (GLOB_RECURSE _DDI_HEADERS
    ${BS_DIR_INSTRUMENTATION}/metrics_library/codegen/ddi/inc/*.h
    )
set (_HEADERS ${_HEADERS} ${_DDI_HEADERS})

# generated library headers
file (GLOB_RECURSE _DDI_LIBRARY_HEADERS
    ${BS_DIR_INSTRUMENTATION}/metrics_library/codegen/library/inc/*.h
    )
set (_HEADERS ${_HEADERS} ${_DDI_LIBRARY_HEADERS})

# generated gpu commands headers
file (GLOB_RECURSE _COMMANDS_HEADERS
    ${BS_DIR_INSTRUMENTATION}/metrics_library/codegen/gpu/*.h
    )
set (_HEADERS ${_HEADERS} ${_COMMANDS_HEADERS})

# generated traits headers
file (GLOB _TRAITS_HEADERS
    ${BS_DIR_INSTRUMENTATION}/metrics_library/codegen/traits/*.h
    )
set (_HEADERS ${_HEADERS} ${_TRAITS_HEADERS})

# command buffer traits headers
file (GLOB _TRAITS_HEADERS
    ${BS_DIR_INSTRUMENTATION}/metrics_library/traits/command_buffer/*.h
    )
set (_HEADERS ${_HEADERS} ${_TRAITS_HEADERS})

# configuration traits headers
file (GLOB _TRAITS_HEADERS
    ${BS_DIR_INSTRUMENTATION}/metrics_library/traits/configuration/*.h
    )
set (_HEADERS ${_HEADERS} ${_TRAITS_HEADERS})

# constants traits headers
file (GLOB _TRAITS_HEADERS
    ${BS_DIR_INSTRUMENTATION}/metrics_library/traits/constants/*.h
    )
set (_HEADERS ${_HEADERS} ${_TRAITS_HEADERS})

# context traits headers
file (GLOB _TRAITS_HEADERS
    ${BS_DIR_INSTRUMENTATION}/metrics_library/traits/context/*.h
    )
set (_HEADERS ${_HEADERS} ${_TRAITS_HEADERS})

# device traits headers
file (GLOB _TRAITS_HEADERS
    ${BS_DIR_INSTRUMENTATION}/metrics_library/traits/device/*.h
    )
set (_HEADERS ${_HEADERS} ${_TRAITS_HEADERS})

# example traits headers
file (GLOB _TRAITS_HEADERS
    ${BS_DIR_INSTRUMENTATION}/metrics_library/traits/example/*.h
    )
set (_HEADERS ${_HEADERS} ${_TRAITS_HEADERS})

# function log traits headers
file (GLOB _TRAITS_HEADERS
    ${BS_DIR_INSTRUMENTATION}/metrics_library/traits/function_log/*.h
    )
set (_HEADERS ${_HEADERS} ${_TRAITS_HEADERS})

# internal feature headers
file (GLOB _TRAITS_HEADERS
    ${BS_DIR_INSTRUMENTATION}/metrics_library/traits/internal/*.h
    )
set (_HEADERS ${_HEADERS} ${_TRAITS_HEADERS})

# kernel interface traits headers
file (GLOB _TRAITS_HEADERS
    ${BS_DIR_INSTRUMENTATION}/metrics_library/traits/kernel_interface/*.h
    )
set (_HEADERS ${_HEADERS} ${_TRAITS_HEADERS})

# marker traits headers
file (GLOB _TRAITS_HEADERS
    ${BS_DIR_INSTRUMENTATION}/metrics_library/traits/marker/*.h
    )
set (_HEADERS ${_HEADERS} ${_TRAITS_HEADERS})

# oa buffer traits headers
file (GLOB _TRAITS_HEADERS
    ${BS_DIR_INSTRUMENTATION}/metrics_library/traits/oa_buffer/*.h
    )
set (_HEADERS ${_HEADERS} ${_TRAITS_HEADERS})

# object traits headers
file (GLOB _TRAITS_HEADERS
    ${BS_DIR_INSTRUMENTATION}/metrics_library/traits/object/*.h
    )
set (_HEADERS ${_HEADERS} ${_TRAITS_HEADERS})

# override traits headers
file (GLOB _TRAITS_HEADERS
    ${BS_DIR_INSTRUMENTATION}/metrics_library/traits/override/*.h
    )
set (_HEADERS ${_HEADERS} ${_TRAITS_HEADERS})

# policy traits headers
file (GLOB _TRAITS_HEADERS
    ${BS_DIR_INSTRUMENTATION}/metrics_library/traits/policy/*.h
    )
set (_HEADERS ${_HEADERS} ${_TRAITS_HEADERS})

# query hw counters traits headers
file (GLOB _TRAITS_HEADERS
    ${BS_DIR_INSTRUMENTATION}/metrics_library/traits/query/*.h
    )
set (_HEADERS ${_HEADERS} ${_TRAITS_HEADERS})

# tools traits headers
file (GLOB _TRAITS_HEADERS
    ${BS_DIR_INSTRUMENTATION}/metrics_library/traits/tools/*.h
    )
set (_HEADERS ${_HEADERS} ${_TRAITS_HEADERS})

# validation traits headers
file (GLOB _TRAITS_HEADERS
    ${BS_DIR_INSTRUMENTATION}/metrics_library/traits/validation/*.h
    )
set (_HEADERS ${_HEADERS} ${_TRAITS_HEADERS})

# platform dependent
if (${PLATFORM} STREQUAL Windows) # windows
    # instrumentation utils headers
    file (GLOB _IU_OS_HEADERS
        ${BS_DIR_INSTRUMENTATION}/utils/windows/inc/*.h
        )
    set (_HEADERS ${_HEADERS} ${_IU_OS_HEADERS})

    ml_add_headers("windows")

    # add linux headers to solution explorer
    ml_add_headers("linux")

    # linux instrumentation utils headers
    file (GLOB _IU_LINUX_HEADERS
        ${BS_DIR_INSTRUMENTATION}/utils/linux/inc/*.h
        )

    # external
    file (GLOB_RECURSE _EXTERNAL_HEADERS
        ${BS_DIR_INSTRUMENTATION}/metrics_library/library/inc/linux/external/drm/*.h
        )
    set (_HEADERS ${_HEADERS} ${_IU_LINUX_HEADERS} ${_EXTERNAL_HEADERS})
elseif (${PLATFORM} STREQUAL "linux") # linux
    # instrumentation utils headers
    file (GLOB _IU_OS_HEADERS
        ${BS_DIR_INSTRUMENTATION}/utils/linux/inc/*.h
        )
    set (_HEADERS ${_HEADERS} ${_IU_OS_HEADERS})

    # external
    file (GLOB_RECURSE _EXTERNAL_HEADERS
        ${BS_DIR_INSTRUMENTATION}/metrics_library/library/inc/external/*.h
        )
    set (_HEADERS ${_HEADERS} ${_EXTERNAL_HEADERS})

    ml_add_headers("linux")
else ()
    message("-- Using platform is ${PLATFORM}")
endif ()

set (HEADERS ${_HEADERS})

#################################################################################
# SOURCE GROUPS
#################################################################################
# common
source_group (api                           ${BS_DIR_INC}/common/instrumentation/api/.*h)
source_group (ddi                           ${BS_DIR_INSTRUMENTATION}/metrics_library/ddi/.*hpp)
source_group (codegen\\ddi                  ${BS_DIR_INSTRUMENTATION}/metrics_library/codegen/ddi/.*cpp)
source_group (codegen\\ddi\\inc             ${BS_DIR_INSTRUMENTATION}/metrics_library/codegen/ddi/inc/.*h)
source_group (codegen\\gpu                  ${BS_DIR_INSTRUMENTATION}/metrics_library/codegen/gpu/.*h)
source_group (codegen\\traits               ${BS_DIR_INSTRUMENTATION}/metrics_library/codegen/traits/.*h)
source_group (codegen\\library              ${BS_DIR_INSTRUMENTATION}/metrics_library/codegen/library/.*)
source_group (codegen\\library\\inc         ${BS_DIR_INSTRUMENTATION}/metrics_library/codegen/library/inc/.*h)
source_group (library                       ${BS_DIR_INSTRUMENTATION}/metrics_library/ml.rc)
source_group (library\\inc                  ${BS_DIR_INSTRUMENTATION}/metrics_library/library/inc/.*h)
source_group (traits\\command_buffer        ${BS_DIR_INSTRUMENTATION}/metrics_library/traits/command_buffer/.*h)
source_group (traits\\configuration         ${BS_DIR_INSTRUMENTATION}/metrics_library/traits/configuration/.*h)
source_group (traits\\constants             ${BS_DIR_INSTRUMENTATION}/metrics_library/traits/constants/.*h)
source_group (traits\\context               ${BS_DIR_INSTRUMENTATION}/metrics_library/traits/context/.*h)
source_group (traits\\device                ${BS_DIR_INSTRUMENTATION}/metrics_library/traits/device/.*h)
source_group (traits\\example               ${BS_DIR_INSTRUMENTATION}/metrics_library/traits/example/.*h)
source_group (traits\\function_log          ${BS_DIR_INSTRUMENTATION}/metrics_library/traits/function_log/.*h)
source_group (traits\\internal              ${BS_DIR_INSTRUMENTATION}/metrics_library/traits/internal/.*h)
source_group (traits\\kernel_interface      ${BS_DIR_INSTRUMENTATION}/metrics_library/traits/kernel_interface/.*h)
source_group (traits\\marker                ${BS_DIR_INSTRUMENTATION}/metrics_library/traits/marker/.*h)
source_group (traits\\oa_buffer             ${BS_DIR_INSTRUMENTATION}/metrics_library/traits/oa_buffer/.*h)
source_group (traits\\objects               ${BS_DIR_INSTRUMENTATION}/metrics_library/traits/object/.*h)
source_group (traits\\override              ${BS_DIR_INSTRUMENTATION}/metrics_library/traits/override/.*h)
source_group (traits\\policy                ${BS_DIR_INSTRUMENTATION}/metrics_library/traits/policy/.*h)
source_group (traits\\query                 ${BS_DIR_INSTRUMENTATION}/metrics_library/traits/query/.*h)
source_group (traits\\tools                 ${BS_DIR_INSTRUMENTATION}/metrics_library/traits/tools/.*h)
source_group (traits\\validation            ${BS_DIR_INSTRUMENTATION}/metrics_library/traits/validation/.*h)
# debug logs framework
source_group (iu FILES
    ${BS_DIR_INSTRUMENTATION}/utils/common/iu_debug.c
    ${BS_DIR_INSTRUMENTATION}/utils/common/inc/iu_debug.h
    ${BS_DIR_INSTRUMENTATION}/utils/common/inc/iu_std.h
)

# platform dependent
if (${PLATFORM} STREQUAL Windows) # windows
    source_group (codegen\\traits\\windows          ${BS_DIR_INSTRUMENTATION}/metrics_library/codegen/traits/windows/.*h)
    source_group (library\\inc\\windows             ${BS_DIR_INSTRUMENTATION}/metrics_library/library/inc/windows/.*h)
    source_group (traits\\configuration\\windows    ${BS_DIR_INSTRUMENTATION}/metrics_library/traits/configuration/windows/.*h)
    source_group (traits\\constants\\windows        ${BS_DIR_INSTRUMENTATION}/metrics_library/traits/constants/windows/.*h)
    source_group (traits\\internal\\windows         ${BS_DIR_INSTRUMENTATION}/metrics_library/traits/internal/windows/.*h)
    source_group (traits\\kernel_interface\\windows ${BS_DIR_INSTRUMENTATION}/metrics_library/traits/kernel_interface/windows/.*h)
    source_group (traits\\marker\\windows           ${BS_DIR_INSTRUMENTATION}/metrics_library/traits/marker/windows/.*h)
    source_group (traits\\override\\windows         ${BS_DIR_INSTRUMENTATION}/metrics_library/traits/override/windows/.*h)
    source_group (traits\\policy\\windows           ${BS_DIR_INSTRUMENTATION}/metrics_library/traits/policy/windows/.*h)
    source_group (traits\\tools\\windows            ${BS_DIR_INSTRUMENTATION}/metrics_library/traits/tools/windows/.*h)
    source_group (traits\\validation\\windows       ${BS_DIR_INSTRUMENTATION}/metrics_library/traits/validation/windows/.*h)
    # debug logs framework
    source_group (iu\\windows FILES
        ${BS_DIR_INSTRUMENTATION}/utils/windows/umd/iu_os.cpp
        ${BS_DIR_INSTRUMENTATION}/utils/windows/umd/iu_std.cpp
        ${BS_DIR_INSTRUMENTATION}/utils/windows/inc/iu_os.h
    )
    # add linux headers to solution explorer
    source_group (codegen\\traits\\linux             ${BS_DIR_INSTRUMENTATION}/metrics_library/codegen/traits/linux/.*h)
    source_group (library\\inc\\linux                ${BS_DIR_INSTRUMENTATION}/metrics_library/library/inc/linux/.*)
    source_group (library\\inc\\linux\\external\\drm ${BS_DIR_INSTRUMENTATION}/metrics_library/library/inc/linux/external/drm.*)
    source_group (traits\\configuration\\linux       ${BS_DIR_INSTRUMENTATION}/metrics_library/traits/configuration/linux/.*h)
    source_group (traits\\constants\\linux           ${BS_DIR_INSTRUMENTATION}/metrics_library/traits/constants/linux/.*h)
    source_group (traits\\device\\linux              ${BS_DIR_INSTRUMENTATION}/metrics_library/traits/device/linux/.*h)
    source_group (traits\\kernel_interface\\linux    ${BS_DIR_INSTRUMENTATION}/metrics_library/traits/kernel_interface/linux/.*h)
    source_group (traits\\marker\\linux              ${BS_DIR_INSTRUMENTATION}/metrics_library/traits/marker/linux/.*h)
    source_group (traits\\override\\linux            ${BS_DIR_INSTRUMENTATION}/metrics_library/traits/override/linux/.*h)
    source_group (traits\\policy\\linux              ${BS_DIR_INSTRUMENTATION}/metrics_library/traits/policy/linux/.*h)
    source_group (traits\\tools\\linux               ${BS_DIR_INSTRUMENTATION}/metrics_library/traits/tools/linux/.*h)
    source_group (traits\\validation\\linux          ${BS_DIR_INSTRUMENTATION}/metrics_library/traits/validation/linux/.*h)
    source_group (iu\\linux FILES
        ${BS_DIR_INSTRUMENTATION}/utils/linux/iu_os.cpp
        ${BS_DIR_INSTRUMENTATION}/utils/linux/iu_std.cpp
        ${BS_DIR_INSTRUMENTATION}/utils/linux/inc/iu_os.h
        ${BS_DIR_INSTRUMENTATION}/utils/linux/inc/iu_i915_perf.h
    )
elseif (${PLATFORM} STREQUAL linux) # linux
    source_group (codegen\\traits\\linux             ${BS_DIR_INSTRUMENTATION}/metrics_library/codegen/traits/linux/.*h)
    source_group (library\\inc\\linux                ${BS_DIR_INSTRUMENTATION}/metrics_library/library/inc/linux/.*)
    source_group (library\\inc\\linux\\external\\drm ${BS_DIR_INSTRUMENTATION}/metrics_library/library/inc/linux/external/drm/.*)
    source_group (traits\\configuration\\linux       ${BS_DIR_INSTRUMENTATION}/metrics_library/traits/configuration/linux/.*h)
    source_group (traits\\constants\\linux           ${BS_DIR_INSTRUMENTATION}/metrics_library/traits/constants/linux/.*h)
    source_group (traits\\device\\linux              ${BS_DIR_INSTRUMENTATION}/metrics_library/traits/device/linux/.*h)
    source_group (traits\\kernel_interface\\linux    ${BS_DIR_INSTRUMENTATION}/metrics_library/traits/kernel_interface/linux/.*h)
    source_group (traits\\marker\\linux              ${BS_DIR_INSTRUMENTATION}/metrics_library/traits/marker/linux/.*h)
    source_group (traits\\override\\linux            ${BS_DIR_INSTRUMENTATION}/metrics_library/traits/override/linux/.*h)
    source_group (traits\\policy\\linux              ${BS_DIR_INSTRUMENTATION}/metrics_library/traits/policy/linux/.*h)
    source_group (traits\\tools\\linux               ${BS_DIR_INSTRUMENTATION}/metrics_library/traits/tools/linux/.*h)
    source_group (traits\\validation\\linux          ${BS_DIR_INSTRUMENTATION}/metrics_library/traits/validation/linux/.*h)
    # debug logs framework
    source_group (iu\\linux FILES
        ${BS_DIR_INSTRUMENTATION}/utils/linux/iu_os.cpp
        ${BS_DIR_INSTRUMENTATION}/utils/linux/iu_std.cpp
        ${BS_DIR_INSTRUMENTATION}/utils/linux/inc/iu_os.h
        ${BS_DIR_INSTRUMENTATION}/utils/linux/inc/iu_i915_perf.h
    )
else()
    message("-- Using platform is ${PLATFORM}")
endif ()

#################################################################################
# TARGET FLAGS (add_library & link_libraries)
#################################################################################
# add library
add_library (${PROJECT_NAME} SHARED ${HEADERS} ${SOURCES})

# version
set_target_properties (${PROJECT_NAME} PROPERTIES VERSION ${LIB_VERSION} SOVERSION ${LIB_MAJOR_VERSION})

# project specific target settings
if (${PLATFORM} STREQUAL Windows) # windows
    instr_target_settings (${PROJECT_NAME})
    set_property (TARGET ${PROJECT_NAME} PROPERTY OUTPUT_NAME ${LIB}${ARCH})
    set (CMAKE_CXX_STANDARD_LIBRARIES "")
    set (CMAKE_C_STANDARD_LIBRARIES "")
    set_target_properties (${PROJECT_NAME} PROPERTIES VS_GLOBAL_DriverTargetPlatform Universal)
    set_target_properties (${PROJECT_NAME} PROPERTIES VS_PLATFORM_TOOLSET "WindowsApplicationForDrivers10.0")
    target_link_libraries (${PROJECT_NAME}
            onecoreuap.lib
            )
    windows_apps_props_universal (${PROJECT_NAME})
elseif (${PLATFORM} STREQUAL linux) # linux
    set_property (TARGET ${PROJECT_NAME} PROPERTY OUTPUT_NAME ${LIB})
else()
    message("-- Using platform is ${PLATFORM}")
endif ()

#################################################################################
# INCLUDES
#################################################################################
set (COMMON_TARGET_INCLUDE_DIRECTORIES
    ${BS_DIR_INC}/common/instrumentation/api
    ${BS_DIR_INSTRUMENTATION}/utils/inc
    ${BS_DIR_INSTRUMENTATION}/utils/common/inc
    ${BS_DIR_INSTRUMENTATION}/metrics_library/ddi
    ${BS_DIR_INSTRUMENTATION}/metrics_library/codegen/ddi/inc
    ${BS_DIR_INSTRUMENTATION}/metrics_library/codegen/gpu
    ${BS_DIR_INSTRUMENTATION}/metrics_library/codegen/traits
    ${BS_DIR_INSTRUMENTATION}/metrics_library/codegen/library/inc
    ${BS_DIR_INSTRUMENTATION}/metrics_library/library/inc
    ${BS_DIR_INSTRUMENTATION}/metrics_library/traits/command_buffer
    ${BS_DIR_INSTRUMENTATION}/metrics_library/traits/configuration
    ${BS_DIR_INSTRUMENTATION}/metrics_library/traits/constants
    ${BS_DIR_INSTRUMENTATION}/metrics_library/traits/context
    ${BS_DIR_INSTRUMENTATION}/metrics_library/traits/device
    ${BS_DIR_INSTRUMENTATION}/metrics_library/traits/example
    ${BS_DIR_INSTRUMENTATION}/metrics_library/traits/function_log
    ${BS_DIR_INSTRUMENTATION}/metrics_library/traits/internal
    ${BS_DIR_INSTRUMENTATION}/metrics_library/traits/kernel_interface
    ${BS_DIR_INSTRUMENTATION}/metrics_library/traits/marker
    ${BS_DIR_INSTRUMENTATION}/metrics_library/traits/oa_buffer
    ${BS_DIR_INSTRUMENTATION}/metrics_library/traits/object
    ${BS_DIR_INSTRUMENTATION}/metrics_library/traits/override
    ${BS_DIR_INSTRUMENTATION}/metrics_library/traits/policy
    ${BS_DIR_INSTRUMENTATION}/metrics_library/traits/query
    ${BS_DIR_INSTRUMENTATION}/metrics_library/traits/tools
    ${BS_DIR_INSTRUMENTATION}/metrics_library/traits/validation
    )

if (${PLATFORM} STREQUAL Windows) # windows
    target_include_directories (${PROJECT_NAME} PUBLIC
        ${COMMON_TARGET_INCLUDE_DIRECTORIES}
        ${BS_DIR_INSTRUMENTATION}/utils/windows/inc
        ${BS_DIR_INSTRUMENTATION}/metrics_library/library/inc/windows
        )
elseif (${PLATFORM} STREQUAL linux) # linux
    target_include_directories (${PROJECT_NAME} PUBLIC
        ${COMMON_TARGET_INCLUDE_DIRECTORIES}
        ${BS_DIR_INSTRUMENTATION}/utils/linux/inc
        ${BS_DIR_INSTRUMENTATION}/metrics_library/library/inc/linux
        ${BS_DIR_INSTRUMENTATION}/metrics_library/library/inc/linux/external/drm
        )
else()
    message("-- Using platform is ${PLATFORM}")
endif ()

#################################################################################
# COMPILER FLAGS
#################################################################################
if (${PLATFORM} STREQUAL Windows) # windows
    bs_set_extra_target_properties (${PROJECT_NAME}
        USERMODE_DRIVER
        )
    set (COMMON_COMPILE_OPTIONS
        /W4         # display warnings level 4
        /WX         # treat warnings as errors
        /Zp8        # struct member alignment - 8 bytes
        /bigobj     # increase number of sections in .obj
        /GR-        # run-time type information disabled
        /wd4127     # conditional expression is constant
        /wd4201     # nonstandard extension used : nameless struct/union
        /wd4324     # structure was padded due to alignment specifier
        /wd4714     # marked as __forceinline not inlined
        )
    target_compile_options (
        ${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Release>:
            ${COMMON_COMPILE_OPTIONS}
            /Ot     # favor fast code
            /O2     # favor speed
            /GL     # whole program optimization
        >
        $<$<CONFIG:ReleaseInternal>:
            ${COMMON_COMPILE_OPTIONS}
            /Ot     # favor fast code
            /O2     # favor speed
            /GL     # whole program optimization
        >
        $<$<CONFIG:Debug>:
            ${COMMON_COMPILE_OPTIONS}
        >
        )
elseif (${PLATFORM} STREQUAL linux) # linux
    target_compile_options (
        ${PROJECT_NAME} PRIVATE
        -Wall                    # enables all compiler's warning messages
        -Werror                  # treat warning as errors
        #-Wfatal-errors          # abort compilation on the first error occurred
        -Wno-unknown-pragmas     # allow unknown pragmas (such as #pragma region)
        -fpack-struct=8          # struct member alignment - 8 bytes
        -fvisibility=hidden      # hide non-exported symbols
        -Wno-sequence-point      # disable undefined behavior warning
        -Wno-comment             # allow multi-line comments
        -Ofast                   # optimize code in the favor of speed
        -Wno-extern-c-compat     # disable "empty struct has size 0 in C, size 1 in C++" warning
        -fstack-protector-strong # enables stack protection for vulnerable functions that contain: An array of any size and type
        )
    if ("${TARGET_ARCH}" STREQUAL "32")
        add_definitions (-m32)
    endif ()
    if ("${CMAKE_BUILD_TYPE}" STREQUAL "debug")
        add_definitions (-D_DEBUG)
    elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "release-internal")
        add_definitions (-D_RELEASE_INTERNAL)
    elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "releaseinternal")
        add_definitions (-D_RELEASE_INTERNAL)
    endif ()
else()
    message("-- Using platform is ${PLATFORM}")
endif ()
#################################################################################
# GENERATOR
#################################################################################
if (MSVC AND NOT "$ENV{skipPyProj}" STREQUAL "True")
    include_external_msproject ("metrics_library_generator" ${GFX_DEVELOPMENT_DIR}/Tools/MetricsLibraryGenerator/metrics_library_generator.pyproj)
    set_target_properties ("metrics_library_generator" PROPERTIES EXCLUDE_FROM_DEFAULT_BUILD TRUE)
    set_target_properties ("metrics_library_generator" PROPERTIES EXCLUDE_FROM_ALL TRUE)
endif ()

#################################################################################
# LINKER FLAGS
#################################################################################
if (${PLATFORM} STREQUAL Windows) # windows
    # dummy linker flags to override the need to forcefully add them this way
    set (CMAKE_SHARED_LINKER_FLAGS_RELEASEINTERNAL "/LTCG /INCREMENTAL:NO /OPT:ICF")
    set (CMAKE_SHARED_LINKER_FLAGS_RELEASE         "/LTCG /INCREMENTAL:NO /OPT:ICF")
    set (CMAKE_SHARED_LINKER_FLAGS_DEBUG           "")
    set (CMAKE_SHARED_LINKER_FLAGS                 "")
elseif (${PLATFORM} STREQUAL linux) # linux
    set (CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--no-undefined")
    set (CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,-z,defs")
    set (CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,-z,noexecstack")
    set (CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,-z,relro")
    set (CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,-z,now")
    # to enable x86 build on x64 host
    if ("${TARGET_ARCH}" STREQUAL "32")
        set (CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -m32")
    endif ()
else()
    message("-- Using platform is ${PLATFORM}")
endif ()

#################################################################################
# LINK LIBRARIES
#################################################################################
if (${PLATFORM} STREQUAL linux)
    set (DRM_LIB_PATH drm)
    target_link_libraries (
        ${PROJECT_NAME}     # metrics_library
        ${DRM_LIB_PATH}     # drm
    )
endif ()

#################################################################################
# INSTALLER
#################################################################################
if (${PLATFORM} STREQUAL Windows) # windows
    install (TARGETS ${PROJECT_NAME} RUNTIME
        DESTINATION Release/lh64
        CONFIGURATIONS Release
    )

    install (TARGETS ${PROJECT_NAME} RUNTIME
        DESTINATION Release-Internal/lh64
        CONFIGURATIONS ReleaseInternal
    )

    install (TARGETS ${PROJECT_NAME} RUNTIME
        DESTINATION Debug/lh64
        CONFIGURATIONS Debug
    )

    install (FILES $<TARGET_PDB_FILE:${PROJECT_NAME}>
        DESTINATION Release/lh64/pdb
        CONFIGURATIONS Release
    )

    install (FILES $<TARGET_PDB_FILE:${PROJECT_NAME}>
        DESTINATION Release-Internal/lh64/pdb
        CONFIGURATIONS ReleaseInternal
    )

    install (FILES $<TARGET_PDB_FILE:${PROJECT_NAME}>
        DESTINATION Debug/lh64/pdb
        CONFIGURATIONS Debug
    )
elseif (${PLATFORM} STREQUAL linux) # linux
    include (GNUInstallDirs)

    if (${TARGET_ARCH} STREQUAL 64)
        set (CPACK_PACKAGE_ARCHITECTURE        "x86_64")
        set (CPACK_DEBIAN_PACKAGE_ARCHITECTURE "amd64")
        set (CPACK_RPM_PACKAGE_ARCHITECTURE    "x86_64")
    else ()
        set (CPACK_PACKAGE_ARCHITECTURE        "i386")
        set (CPACK_DEBIAN_PACKAGE_ARCHITECTURE "i386")
        set (CPACK_RPM_PACKAGE_ARCHITECTURE    "i386")
    endif ()

    # pkg-config
    set (PC_FILE ${LIB_NAME}.pc)
    set (PC_IN ${PC_FILE}.in)

    configure_file (${PC_IN} ${CMAKE_BINARY_DIR}/${PC_FILE} @ONLY)

    install (FILES ${CMAKE_BINARY_DIR}/${PC_FILE}
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig
        COMPONENT metrics-library-pkgconfig
    )

    # header
    install (FILES ${API_HEADER_LOCATION}
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
        COMPONENT metrics-library-devel
    )

    # library
    install (TARGETS ${PROJECT_NAME}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        COMPONENT metrics-library
        NAMELINK_SKIP
    )

    install (TARGETS ${PROJECT_NAME}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        COMPONENT metrics-library-devel
        NAMELINK_ONLY
    )

    # workaround with 2 symlinks (e.g. libigdml64.so, libigdml.so) in devel package refering to libigdml.so.1 in regular package
    ADD_CUSTOM_TARGET(link_target ALL COMMAND ${CMAKE_COMMAND} -E create_symlink "${LIB_NAME}.so.${LIB_MAJOR_VERSION}" "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/${LIB_NAME}${TARGET_ARCH}.so")
    install(FILES "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/${LIB_NAME}${TARGET_ARCH}.so"
        DESTINATION ${CMAKE_INSTALL_LIBDIR}
        COMPONENT metrics-library-devel)

    # software package generator
    if (LIB_CPACK_GENERATOR)
        set (CPACK_GENERATOR ${LIB_CPACK_GENERATOR})
    else ()
        if (EXISTS "/etc/debian_version")
            set (CPACK_GENERATOR "DEB")
        elseif (EXISTS "/etc/redhat-release")
            set (CPACK_GENERATOR "RPM")
        else ()
            set (CPACK_GENERATOR   "TXZ")
            set (CPACK_SET_DESTDIR ON)
        endif ()
    endif ()

    set (CPACK_DEBIAN_PACKAGE_MAINTAINER   "intel")
    set (CPACK_PACKAGE_NAME                "intel")
    set (CPACK_PACKAGE_VERSION             ${LIB_MAJOR_VERSION}.${LIB_MINOR_VERSION}.${LIB_BUILD_VERSION})
    set (CPACK_PACKAGE_INSTALL_DIRECTORY   ${CMAKE_INSTALL_PREFIX})
    set (CPACK_DEBIAN_PACKAGE_DESCRIPTION  "Metrics Library for Intel(R) Graphics Technology")
    set (CPACK_DEBIAN_PACKAGE_SHLIBDEPS    ON)
    set (CPACK_RPM_PACKAGE_RELOCATABLE     TRUE)

    if (DEFINED LIB_PACKAGE_RELEASE)
        set (CPACK_DEBIAN_PACKAGE_RELEASE ${LIB_PACKAGE_RELEASE})
        set (CPACK_RPM_PACKAGE_RELEASE    ${LIB_PACKAGE_RELEASE})
    else ()
        set (CPACK_DEBIAN_PACKAGE_RELEASE 1)
        set (CPACK_RPM_PACKAGE_RELEASE    1)
    endif ()

    set (CPACK_DEBIAN_METRICS-LIBRARY-PKGCONFIG_FILE_NAME  "intel-metrics-library-pkgconfig-${CPACK_PACKAGE_VERSION}-${CPACK_DEBIAN_PACKAGE_RELEASE}_${CPACK_DEBIAN_PACKAGE_ARCHITECTURE}.deb")
    set (CPACK_RPM_METRICS-LIBRARY-PKGCONFIG_FILE_NAME     "intel-metrics-library-pkgconfig-${CPACK_PACKAGE_VERSION}-${CPACK_RPM_PACKAGE_RELEASE}.${CPACK_RPM_PACKAGE_ARCHITECTURE}.rpm")
    set (CPACK_ARCHIVE_METRICS-LIBRARY-PKGCONFIG_FILE_NAME "intel-metrics-library-pkgconfig-${CPACK_PACKAGE_VERSION}-${CPACK_DEBIAN_PACKAGE_RELEASE}.${CPACK_PACKAGE_ARCHITECTURE}")

    set (CPACK_DEBIAN_METRICS-LIBRARY-DEVEL_FILE_NAME  "intel-metrics-library-devel-${CPACK_PACKAGE_VERSION}-${CPACK_DEBIAN_PACKAGE_RELEASE}_${CPACK_DEBIAN_PACKAGE_ARCHITECTURE}.deb")
    set (CPACK_RPM_METRICS-LIBRARY-DEVEL_FILE_NAME     "intel-metrics-library-devel-${CPACK_PACKAGE_VERSION}-${CPACK_RPM_PACKAGE_RELEASE}.${CPACK_RPM_PACKAGE_ARCHITECTURE}.rpm")
    set (CPACK_ARCHIVE_METRICS-LIBRARY-DEVEL_FILE_NAME "intel-metrics-library-devel-${CPACK_PACKAGE_VERSION}-${CPACK_DEBIAN_PACKAGE_RELEASE}.${CPACK_PACKAGE_ARCHITECTURE}")

    set (CPACK_DEBIAN_METRICS-LIBRARY_FILE_NAME  "intel-metrics-library-${CPACK_PACKAGE_VERSION}-${CPACK_DEBIAN_PACKAGE_RELEASE}_${CPACK_DEBIAN_PACKAGE_ARCHITECTURE}.deb")
    set (CPACK_RPM_METRICS-LIBRARY_FILE_NAME     "intel-metrics-library-${CPACK_PACKAGE_VERSION}-${CPACK_RPM_PACKAGE_RELEASE}.${CPACK_RPM_PACKAGE_ARCHITECTURE}.rpm")
    set (CPACK_ARCHIVE_METRICS-LIBRARY_FILE_NAME "intel-metrics-library-${CPACK_PACKAGE_VERSION}-${CPACK_DEBIAN_PACKAGE_RELEASE}.${CPACK_PACKAGE_ARCHITECTURE}")

    set(CPACK_DEBIAN_METRICS-LIBRARY-DEVEL_PACKAGE_DEPENDS "intel-metrics-library(=${CPACK_PACKAGE_VERSION}-${CPACK_DEBIAN_PACKAGE_RELEASE})")
    set(CPACK_RPM_METRICS-LIBRARY-DEVEL_PACKAGE_REQUIRES "intel-metrics-library = ${CPACK_PACKAGE_VERSION}-${CPACK_RPM_PACKAGE_RELEASE}")

    set (CPACK_DEB_COMPONENT_INSTALL     ON)
    set (CPACK_RPM_COMPONENT_INSTALL     ON)
    set (CPACK_ARCHIVE_COMPONENT_INSTALL ON)

    set (CPACK_COMPONENTS_ALL metrics-library-pkgconfig metrics-library-devel metrics-library)

    include (CPack)
else()
    message("-- Using platform is ${PLATFORM}")
endif ()

#################################################################################
# CODING STANDARD
#################################################################################
#if (MSVC)
#    add_custom_command (TARGET ${PROJECT_NAME}
#                       PRE_BUILD
#                       COMMAND ${BS_DIR_INSTRUMENTATION}/metrics_library/CodingStandard.bat NoEdit --path ${BS_DIR_EXTERNAL_COMPONENTS}
#                       COMMENT "Checking coding standards..."
#                      )
#endif ()

#################################################################################
# DEBUG
#################################################################################
if (${PLATFORM} STREQUAL Windows) # windows
    instr_debug (${PROJECT_NAME})
elseif (${PLATFORM} STREQUAL linux) # linux
    message ("INFO: PROJECT_NAME           = ${PROJECT_NAME}")
    message ("INFO: VERSION                = ${LIB_VERSION}")
    message ("INFO: BUILD_TYPE             = ${CMAKE_BUILD_TYPE}")
    message ("INFO: PLATFORM               = ${PLATFORM}")
    message ("INFO: HOST_ARCH              = ${HOST_ARCH}")
    message ("INFO: TARGET_ARCH            = ${TARGET_ARCH}")
    message ("INFO: DUMP_DIR               = ${DUMP_DIR}")
    message ("INFO: LIB_INSTALL_DIR        = ${CMAKE_INSTALL_FULL_LIBDIR}")
    message ("INFO: LIBDRM_SRC             = ${LIBDRM_SRC}")
    message ("INFO: DRM_LIB_PATH           = ${DRM_LIB_PATH}")
    message ("INFO: BS_DIR_INSTRUMENTATION = ${BS_DIR_INSTRUMENTATION}")
    message ("INFO: BS_DIR_INC             = ${BS_DIR_INC}")
    message ("INFO: BS_DIR_EXTERNAL        = ${BS_DIR_EXTERNAL}")
    message ("INFO: CMAKE_C_FLAGS          = ${CMAKE_C_FLAGS}")
    message ("INFO: CMAKE_CXX_FLAGS        = ${CMAKE_CXX_FLAGS}")
else()
    message("-- Using platform is ${PLATFORM}")
endif ()
