add_subdirectory(lib)
add_subdirectory(cli)

if(WITH_GUI)
  add_subdirectory(gui)
endif(WITH_GUI)
