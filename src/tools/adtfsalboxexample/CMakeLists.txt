MESSAGE("-- create adtfsalboxexample tool target")

aux_source_directory(. CORE_SOURCE)
MESSAGE("   source files: ${CORE_SOURCE}")

add_executable(adtfsalboxexample ${CORE_SOURCE})
target_link_libraries(adtfsalboxexample core utils kitti dl ${OpenCV_LIBS} ${Boost_LIBRARIES} ${EXTERNAL_LIBS} )