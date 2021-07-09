cmake_minimum_required(VERSION 2.8)

set(pluginName XBraidIntegrator) #[[todo]]
project(UG_PLUGIN_${pluginName})

set(UG4_XBraidIntegrator_plugin_VERSION_MAJOR 0.1)
set(UG4_XBraidIntegrator_plugin_VERSION_MINOR 0.0)

# ----------  ----------  ---------- ---------- Compiling libraries ---------- ----------  ----------  ----------

set(CMAKE_CXX_FLAGS " ${CMAKE_CXX_FLAGS_INIT} -fPIC")

set(SOURCES
        src/xbraidintegrator_plugin.cpp
        src/simple_integrator_factory.h
        src/limex_integrator_factory.h src/linear_time_integrator_factory.h src/const_step_linear_time_integrator_factory.h src/time_integrator_linear_adaptive_factory.h src/discontinuity_integrator_factory.h)


if(UG_ROOT_CMAKE_PATH)
    MESSAGE("XBraidIntegrator UG_ROOT was found")
else(UG_ROOT_CMAKE_PATH)
    set(UG_ROOT_CMAKE_PATH /pro/ug/ugcore/cmake)
endif(UG_ROOT_CMAKE_PATH)

include(${UG_ROOT_CMAKE_PATH}/ug_plugin_includes.cmake)

if (buildEmbeddedPlugins)
    EXPORTSOURCES(${CMAKE_CURRENT_SOURCE_DIR} ${SOURCES})
else (buildEmbeddedPlugins)
    add_library(${pluginName} SHARED ${SOURCES})
    #add_library(${pluginName} SHARED ${CMAKE_CURRENT_SOURCE_DIR}/libs/xbraid/braid/libbraid.a)
    target_link_libraries(${pluginName} ug4)
    LINK_DIRECTORIES(${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
    #MESSAGE(${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
    # add_dependencies(${pluginName} UG_PLUGIN_Limex)
    target_link_libraries(${pluginName} XBraidForUG4)
    target_link_libraries(${pluginName} Limex)
endif (buildEmbeddedPlugins)

