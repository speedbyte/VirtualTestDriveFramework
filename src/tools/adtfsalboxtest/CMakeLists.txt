MESSAGE("-- create adtfsalboxtest tool target")

aux_source_directory(. CORE_SOURCE)
MESSAGE("   source files: ${CORE_SOURCE}")

add_executable(adtfsalboxtest ${CORE_SOURCE})
#target_link_libraries(adtfsalboxtest kitti saliency plot ${OpenCV_LIBS} ${Boost_LIBRARIES} )