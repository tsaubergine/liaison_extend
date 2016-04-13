
protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS ${CMAKE_CURRENT_BINARY_DIR} config.proto)

add_executable(echotest echo.cpp ${PROTO_SRCS} ${PROTO_HDRS})
target_link_libraries(echotest ${GOBY_LIBRARIES} extend_protobuf boost_system)

