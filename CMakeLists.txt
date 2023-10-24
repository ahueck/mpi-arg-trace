cmake_minimum_required(VERSION 3.20)

project(
    mpitracer
    VERSION 0.2.0
    DESCRIPTION "MPI argument tracer"
    HOMEPAGE_URL "https://github.com/ahueck/mpi-arg-trace"
    LANGUAGES CXX C Fortran
)

list(
  APPEND
  CMAKE_MODULE_PATH
  ${CMAKE_CURRENT_SOURCE_DIR}/cmake
)

include(mpitracerToolchain)

mpitracer_target_format(
  format-mpitracer-sources "Formats project source files"
  TARGETS lib/*.cpp
          lib/*.c
          lib/*.h
)

#add_subdirectory(scripts)
add_subdirectory(lib)

if(PROJECT_IS_TOP_LEVEL)
  if(MPITRACER_TEST)
    enable_testing()
    add_subdirectory(test)
  endif()
endif()

if(PROJECT_IS_TOP_LEVEL)
    feature_summary(WHAT ENABLED_FEATURES PACKAGES_FOUND PACKAGES_NOT_FOUND
      DESCRIPTION "mpitracer ${PROJECT_VERSION} package and feature info:"
      INCLUDE_QUIET_PACKAGES
      QUIET_ON_EMPTY
    )
endif()

