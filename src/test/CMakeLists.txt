MESSAGE("-- create test tool target")

aux_source_directory(. CORE_SOURCE)
MESSAGE("   source files: ${CORE_SOURCE}")

add_executable(boost_test ${CORE_SOURCE})
target_link_libraries( boost_test core io kitti utils ${OpenCV_LIBS} ${Boost_LIBRARIES} )