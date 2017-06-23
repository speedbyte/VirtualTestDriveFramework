MESSAGE("-- create quick_tests target")

aux_source_directory(. CORE_SOURCE)
MESSAGE("   source files: ${CORE_SOURCE}")

add_executable(quick_tests EXCLUDE_FROM_ALL ${CORE_SOURCE})
target_link_libraries(quick_tests kitti saliency plot utils ${OpenCV_LIBS} ${Boost_LIBRARIES} ${EXTERNAL_LIBS} )

install(TARGETS quick_tests DESTINATION bin OPTIONAL)