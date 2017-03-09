MESSAGE("-- create core library target")

MESSAGE("   generate protobuf sources:")
PROTOBUF_GENERATE_CPP(PROTO_SRCS PROTO_HDRS ./node.proto)
MESSAGE("       headers: ${PROTO_SRCS}")
MESSAGE("       headers: ${PROTO_HDRS}")

aux_source_directory(. CORE_SOURCE)
LIST(APPEND CORE_SOURCE ${PROTO_SRCS})
MESSAGE("   source files: ${CORE_SOURCE}")

add_library(core SHARED ${CORE_SOURCE})
target_link_libraries( core pthread ${OpenCV_LIBS} ${Boost_LIBRARIES} ${PROTOBUF_LIBRARIES} ${GRAPHVIZ_LIBRARY})

install(TARGETS core DESTINATION lib)