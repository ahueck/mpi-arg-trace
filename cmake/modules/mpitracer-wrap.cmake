function(mpitracer_make_wrap input output workdir)
  set(WRAP_SCRIPT ${PROJECT_SOURCE_DIR}/scripts/wrap.py)
  
  add_custom_command(
    OUTPUT ${output}
    COMMAND ${Python3_EXECUTABLE} ${WRAP_SCRIPT} ${input} -o
            ${output}
    MAIN_DEPENDENCY ${input}
    WORKING_DIRECTORY ${workdir}
    COMMENT "Generate PMPI wrapper"
  )
endfunction()
