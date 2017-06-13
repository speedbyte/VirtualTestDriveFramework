MESSAGE("-- create booblean maps tool target")

aux_source_directory(. CORE_SOURCE)
MESSAGE("   source files: ${CORE_SOURCE}")

add_executable(booleanmaps ${CORE_SOURCE})
target_link_libraries(booleanmaps kitti saliency plot ${OpenCV_LIBS} ${Boost_LIBRARIES} ${EXTERNAL_LIBS} )