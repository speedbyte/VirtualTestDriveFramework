MESSAGE("-- create protobuild tool target")


list(APPEND SaliencySandbox_LIBS core io kitti plot saliency utils)

foreach(SaliencySandbox_LIB ${SaliencySandbox_LIBS})
    list(APPEND PROTOBUILD_JITC_THIRD_DEP /lib${SaliencySandbox_LIB}.so)
endforeach()

message("graph: ${GRAPHVIZ_LIBRARY}")

foreach(Graphviz_LIB ${GRAPHVIZ_LIBRARY})
    get_filename_component(Graphviz_LIB_P ${Graphviz_LIB} REALPATH)
    install(FILES ${Graphviz_LIB_P} DESTINATION lib/third/graphviz)
    get_filename_component(Graphviz_LIB_N ${Graphviz_LIB_P} NAME)
    list(APPEND PROTOBUILD_JITC_THIRD_DEP /third/graphviz/${Graphviz_LIB_N})
endforeach()

foreach(Boost_LIB ${Boost_LIBRARIES})
    get_filename_component(Boost_LIB_P ${Boost_LIB} REALPATH)
    install(FILES ${Boost_LIB_P} DESTINATION lib/third/boost)
    get_filename_component(Boost_LIB_N ${Boost_LIB_P} NAME)
    list(APPEND PROTOBUILD_JITC_THIRD_DEP /third/boost/${Boost_LIB_N})
endforeach()

foreach(OpenCV_LIB ${OpenCV_LIBS})
    get_property(OpenCV_LIB_P TARGET ${OpenCV_LIB} PROPERTY IMPORTED_LOCATION_RELEASE)
    get_filename_component(OpenCV_LIB_P ${OpenCV_LIB_P} REALPATH)
    install(FILES ${OpenCV_LIB_P} DESTINATION lib/third/opencv)
    get_filename_component(OpenCV_LIB_N ${OpenCV_LIB_P} NAME)
    list(APPEND PROTOBUILD_JITC_THIRD_DEP /third/opencv/${OpenCV_LIB_N})
endforeach()





configure_file(build.h.in build.h)

aux_source_directory(. CORE_SOURCE)

add_executable(protobuild ${CORE_SOURCE} ${PROTO_SRCS} ${PROTO_HDRS})
target_link_libraries(protobuild ${SaliencySandbox_LIBS} ${OpenCV_LIBS} ${Boost_LIBRARIES} ${PROTOBUF_LIBRARIES} ${GRAPHVIZ_LIBRARY} dl )

install(TARGETS protobuild DESTINATION bin)
