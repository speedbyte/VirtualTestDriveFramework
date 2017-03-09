MESSAGE("-- create tracklet plot tool target")

aux_source_directory(. CORE_SOURCE)
MESSAGE("   source files: ${CORE_SOURCE}")

add_executable(plottracklets ${CORE_SOURCE})
target_link_libraries(plottracklets kitti plot ${OpenCV_LIBS} ${Boost_LIBRARIES} )