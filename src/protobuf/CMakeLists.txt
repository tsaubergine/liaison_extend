file(GLOB PROTOS *.proto)

protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS "${project_INC_DIR}/liaison_extend/protobuf" ${PROTOS})


add_library(extend_protobuf SHARED ${PROTO_SRCS} ${PROTO_HDRS})

target_link_libraries(extend_protobuf 
  ${PROTOBUF_LIBRARIES}
  ${GOBY_ACOMMS_LIBRARY}
  ${GOBY_COMMON_LIBRARY}
)

