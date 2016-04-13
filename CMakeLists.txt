cmake_minimum_required(VERSION 2.6.0 FATAL_ERROR)
project(liaison_extend)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake_modules/")

find_package(Goby REQUIRED)
include_directories(${GOBY_INCLUDE_DIRS})

find_package(ZeroMQ REQUIRED)
include_directories(${ZeroMQ_INCLUDE_DIRS})

# protocol buffers - http://code.google.com/apis/protocolbuffers/
find_package(ProtobufLocal REQUIRED)
include_directories(${PROTOBUF_INCLUDE_DIRS})

get_filename_component(project_LIB_DIR lib ABSOLUTE)
get_filename_component(project_BIN_DIR bin ABSOLUTE)
get_filename_component(project_INC_DIR include ABSOLUTE)
get_filename_component(project_SRC_DIR src ABSOLUTE)
get_filename_component(project_SHARE_DIR share ABSOLUTE)
get_filename_component(project_BUILD_DIR build ABSOLUTE)

## set the cmake defaults for libraries and binaries
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${project_LIB_DIR} CACHE PATH 
  "Output directory for the libraries" )
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${project_BIN_DIR} CACHE PATH
  "Output directory for the binaries" )

add_subdirectory(src)
