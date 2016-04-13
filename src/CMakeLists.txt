file(GLOB_RECURSE INCLUDE_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.h *.proto)
foreach(I ${INCLUDE_FILES})
  configure_file(${I} ${project_INC_DIR}/liaison_extend/${I} @ONLY)
endforeach()

include_directories(${project_INC_DIR})
protobuf_include_dirs(${project_INC_DIR})

add_subdirectory(liaison)
add_subdirectory(protobuf)
add_subdirectory(echo)

