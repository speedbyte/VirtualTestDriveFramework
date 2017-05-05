MESSAGE("-- create oxts plot tool target")

aux_source_directory(. CORE_SOURCE)
MESSAGE("   source files: ${CORE_SOURCE}")

add_executable(plotoxts EXCLUDE_FROM_ALL ${CORE_SOURCE})
target_link_libraries(plotoxts kitti plot ${OpenCV_LIBS} ${Boost_LIBRARIES} )