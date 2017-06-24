MESSAGE("-- create protobuild tool target")

list(APPEND SaliencySandbox_LIBS core io kitti plot saliency utils gaze adtf tracking run_configurations)

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

message("installing graphviz ${GRAPHVIZ_LIBRARY} --output-- ${Graphviz_LIB_N} ${Graphviz_LIB_P}     ")

#pkg_boost_LIBRARIES should be the complete path of the .so including the library name.
# for example,  /usr/lib/libgvc.so;/usr/lib/libcdt.so;/usr/lib/libpathplan.so
# pkg_boost_LIBRARIES only gives the names that are written in pc file, normally without lib and without extension
# for example boost_python

foreach(Boost_LIB ${pkg_boost_LIBRARIES})
    get_filename_component(Boost_LIB_P ${Boost_LIB} REALPATH)
# we take the files directly from our own build
    #install(FILES ${Boost_LIB_P} DESTINATION lib/third/boost)
    get_filename_component(Boost_LIB_N ${Boost_LIB_P} NAME)
    list(APPEND PROTOBUILD_JITC_THIRD_DEP ../../../../libs/boost-install/lib/lib${Boost_LIB_N}.so)
endforeach()
message("installing Boost ${pkg_boost_LIBRARIES} --output--  ${Boost_LIB_N} ${Boost_LIB_P}  ")


foreach(OpenCV_LIB ${pkg_opencv_LIBRARIES})
#    get_property(OpenCV_LIB_P TARGET ${OpenCV_LIB} PROPERTY IMPORTED_LOCATION_RELEASE)
    get_filename_component(OpenCV_LIB_P ${OpenCV_LIB} REALPATH)
# we take the files directly from our own build
#install(FILES ${OpenCV_LIB_P} DESTINATION lib/third/opencv)
    get_filename_component(OpenCV_LIB_N ${OpenCV_LIB_P} NAME)
    list(APPEND PROTOBUILD_JITC_THIRD_DEP ../../../../libs/opencv-install/lib/lib${OpenCV_LIB_N}.so)
endforeach()

foreach(PROTOBUF_LIB ${PROTOBUF_LIBRARIES})
    get_filename_component(PROTOBUF_LIB_P ${PROTOBUF_LIB} REALPATH)
    if(EXISTS ${PROTOBUF_LIB_P})
        install(FILES ${PROTOBUF_LIB_P} DESTINATION lib/third/protobuf)
        get_filename_component(PROTOBUF_LIB_N ${PROTOBUF_LIB_P} NAME)
        list(APPEND PROTOBUILD_JITC_THIRD_DEP /third/protobuf/${PROTOBUF_LIB_N})
    else()
        message("skip lib: ${PROTOBUF_LIB}")
    endif()
endforeach()

message("FINAL PROTOBUILD_JITC_THIRD_DEP ${PROTOBUILD_JITC_THIRD_DEP} ")

configure_file(build.h.in build.h)

aux_source_directory(. CORE_SOURCE)

add_executable(protobuild ${CORE_SOURCE} ${PROTO_SRCS} ${PROTO_HDRS})
target_link_libraries(protobuild ${SaliencySandbox_LIBS} ${OpenCV_LIBS} ${Boost_LIBRARIES} ${EXTERNAL_LIBS} ${PROTOBUF_LIBRARIES} ${GRAPHVIZ_LIBRARY} dl )

install(TARGETS protobuild DESTINATION bin)
