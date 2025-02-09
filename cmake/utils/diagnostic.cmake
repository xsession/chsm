include(color_print)
include(code_refactor)

macro(diagnostic)
  get_filename_component(libname ${CMAKE_CURRENT_SOURCE_DIR} NAME)
  if(NOT TARGET ${libname})
    set(libname ${ARGN})
  else()
    if(code_format)
      clang_format(${libname})
    endif()
  endif()

  if(CMAKE_DEBUG_MSG)
    color_print(cyan "--> ${libname}: ")
    get_target_property(list ${libname} INCLUDE_DIRECTORIES)
    message(STATUS "${libname} target INCLUDE_DIRECTORIES list: -> ${list}")
    get_target_property(list ${libname} SOURCE_DIR)
    message(STATUS "${libname} target SOURCE_DIR list: -> ${list}")
    get_target_property(list ${libname} SOURCES)
    message(STATUS "${libname} target SOURCES list: -> ${list}")
    get_target_property(list ${libname} INTERFACE_SOURCES)
    message(STATUS "${libname} target INTERFACE_SOURCES list: -> ${list}")
    get_target_property(list ${libname} COMPILE_DEFINITIONS )
    message(STATUS "${libname} target COMPILE_DEFINITIONS list: -> ${list}")
    get_target_property(list ${libname} LINK_LIBRARIES)
    message(STATUS "${libname} target LINK_LIBRARIES list: -> ${list}")
    get_target_property(list ${libname} COMPILE_OPTIONS)
    message(STATUS "${libname} target COMPILE_OPTIONS list: -> ${list}")
  else()
    color_print(cyan "--> ${libname}: ")
  endif()
endmacro()

