set(LIB_SOURCE ${CMAKE_CURRENT_BINARY_DIR}/mpi_interceptor_rt.c)

mpitracer_make_wrap(mpi_interceptor_tmpl.w ${LIB_SOURCE} ${CMAKE_CURRENT_SOURCE_DIR})

add_library(${MPITRACER_PREFIX}_mpitracer SHARED
  ${LIB_SOURCE}
  InterceptorFunctions.cpp
  System.cpp
  MPIUtil.cpp
)

set_property(TARGET ${MPITRACER_PREFIX}_mpitracer PROPERTY OUTPUT_NAME "mpitracer")
add_library(mpitracer::mpitracer ALIAS ${MPITRACER_PREFIX}_mpitracer)

set_property(TARGET ${MPITRACER_PREFIX}_mpitracer PROPERTY C_STANDARD 11)
set_property(TARGET ${MPITRACER_PREFIX}_mpitracer PROPERTY C_STANDARD_REQUIRED TRUE)

set_property(TARGET ${MPITRACER_PREFIX}_mpitracer PROPERTY CXX_STANDARD 17)
set_property(TARGET ${MPITRACER_PREFIX}_mpitracer PROPERTY CXX_STANDARD_REQUIRED TRUE)

#target_compile_options (${MPITRACER_PREFIX}_mpitracer PUBLIC -fno-rtti)

target_include_directories(${MPITRACER_PREFIX}_mpitracer ${warning_guard}
  PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/lib>
         $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
         $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}>
)

target_link_libraries(${MPITRACER_PREFIX}_mpitracer
  PRIVATE MPI::MPI_CXX
)
