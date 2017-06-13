MESSAGE("-- create velodyne plot tool target")

aux_source_directory(. CORE_SOURCE)
MESSAGE("   source files: ${CORE_SOURCE}")

add_executable(plotvelodyne EXCLUDE_FROM_ALL ${CORE_SOURCE})
target_link_libraries(plotvelodyne kitti utils ${OpenCV_LIBS} ${Boost_LIBRARIES} ${EXTERNAL_LIBS} )