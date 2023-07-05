include(CMakeDependentOption)
include(CMakePackageConfigHelpers)
include(FeatureSummary)
set(FETCHCONTENT_UPDATES_DISCONNECTED ON CACHE STRING "" FORCE)
include(FetchContent)

string(COMPARE EQUAL "${CMAKE_SOURCE_DIR}" "${PROJECT_SOURCE_DIR}"
  PROJECT_IS_TOP_LEVEL
)

find_package(MPI REQUIRED)
find_package(Python3 REQUIRED)

set(MPITRACER_LOG_LEVEL_RT 0 CACHE STRING "Granularity of runtime logger. 3 is most verbose, 0 is least.")

option(MPITRACER_TEST_CONFIGURE_IDE "Add targets for tests to help the IDE with completion etc." ON)
mark_as_advanced(MPITRACER_TEST_CONFIGURE_IDE)
option(MPITRACER_CONFIG_DIR_IS_SHARE "Install to \"share/cmake/\" instead of \"lib/cmake/\"" OFF)
mark_as_advanced(MPITRACER_CONFIG_DIR_IS_SHARE)

set(warning_guard "")
if(NOT PROJECT_IS_TOP_LEVEL)
  option(
      MPITRACER_INCLUDES_WITH_SYSTEM
      "Use SYSTEM modifier for mpitracer includes to disable warnings."
      ON
  )
  mark_as_advanced(MPITRACER_INCLUDES_WITH_SYSTEM)

  if(MPITRACER_INCLUDES_WITH_SYSTEM)
    set(warning_guard SYSTEM)
  endif()
endif()

include(modules/mpitracer-format)
include(modules/mpitracer-wrap)
include(modules/mpitracer-llvm)
include(modules/mpitracer-target-util)

if(PROJECT_IS_TOP_LEVEL)
  if(NOT CMAKE_BUILD_TYPE)
    # set default build type
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "" FORCE)
    set(MPITRACER_LOG_LEVEL_RT 2 CACHE STRING "" FORCE)
    message(STATUS "Building as debug (default)")
  endif()

  if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    # set default install path
    set(CMAKE_INSTALL_PREFIX
        "${mpitracer_SOURCE_DIR}/install/mpitracer"
        CACHE PATH "Default install path" FORCE
    )
    message(STATUS "Installing to (default): ${CMAKE_INSTALL_PREFIX}")
  endif()

    # MPITRACER_DEBUG_POSTFIX is only used for Config
    if(CMAKE_DEBUG_POSTFIX)
      set(MPITRACER_DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX})
    else()
      set(MPITRACER_DEBUG_POSTFIX "-d")
    endif()

  if(NOT CMAKE_DEBUG_POSTFIX AND CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_DEBUG_POSTFIX ${MPITRACER_DEBUG_POSTFIX})
  endif()
else()
  set(MPITRACER_DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX})
endif()

include(GNUInstallDirs)

set(MPITRACER_PREFIX ${PROJECT_NAME})
set(TARGETS_EXPORT_NAME ${MPITRACER_PREFIX}Targets)

if(MPITRACER_CONFIG_DIR_IS_SHARE)
  set(MPITRACER_INSTALL_CONFIGDIR ${CMAKE_INSTALL_DATAROOTDIR}/cmake/${PROJECT_NAME})
else()
  set(MPITRACER_INSTALL_CONFIGDIR ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME})
endif()

