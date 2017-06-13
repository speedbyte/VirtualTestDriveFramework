MESSAGE("-- create spectral whitening tool target")

aux_source_directory(. CORE_SOURCE)
MESSAGE("   source files: ${CORE_SOURCE}")

add_executable(spectralwhitening EXCLUDE_FROM_ALL ${CORE_SOURCE})
target_link_libraries(spectralwhitening kitti saliency plot ${OpenCV_LIBS} ${Boost_LIBRARIES} ${EXTERNAL_LIBS} )