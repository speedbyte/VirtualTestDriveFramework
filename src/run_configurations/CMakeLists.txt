MESSAGE("-- helper class for general run commands")

aux_source_directory(. CORE_SOURCE)
MESSAGE("   source files: ${CORE_SOURCE}")

add_library(run_configurations SHARED ${CORE_SOURCE})
target_link_libraries(run_configurations kitti saliency plot utils ${OpenCV_LIBS} ${Boost_LIBRARIES} ${EXTERNAL_LIBS} )

install(TARGETS run_configurations DESTINATION lib)