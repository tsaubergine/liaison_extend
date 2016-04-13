protobuf_include_dirs(${GOBY_INCLUDE_DIRS})
protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS ${CMAKE_CURRENT_BINARY_DIR} config.proto)

add_library(liaison_plugins_extend SHARED container1.cpp ${PROTO_SRCS} ${PROTO_HDRS})
target_link_libraries(liaison_plugins_extend ${PROTOBUF_LIBRARIES} ${GOBY_LIBRARIES} wt extend_protobuf)
install(TARGETS liaison_plugins_extend LIBRARY DESTINATION lib)
