function(mpitracer_make_wrap input output workdir)
  set(WRAP_SCRIPT ${PROJECT_SOURCE_DIR}/scripts/wrap.py)

  set(MPITRACER_WRAP_FLAGS "")
  set(MPITRACER_WRAP_COMMENT "Generate PMPI wrapper")
  if(MPITRACER_ENABLE_FORTRAN)
    set(MPITRACER_WRAP_FLAGS "-f")
    set(MPITRACER_WRAP_COMMENT "Generate PMPI wrapper with Fortran")
  endif()

  add_custom_command(
    OUTPUT ${output}
    COMMAND ${Python3_EXECUTABLE} ${WRAP_SCRIPT} ${MPITRACER_WRAP_FLAGS} ${input} -o
            ${output}
    MAIN_DEPENDENCY ${input}
    WORKING_DIRECTORY ${workdir}
    COMMENT ${MPITRACER_WRAP_COMMENT}
  )
endfunction()
