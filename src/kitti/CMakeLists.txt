MESSAGE("-- create kitti library target")

aux_source_directory(. CORE_SOURCE)
MESSAGE("   source files: ${CORE_SOURCE}")

add_library(kitti SHARED ${CORE_SOURCE})
target_link_libraries( kitti core io ${OpenCV_LIBS} ${Boost_LIBRARIES} )

install(TARGETS kitti DESTINATION lib)