MESSAGE("-- create eyetribe library target")

aux_source_directory(. CORE_SOURCE)
MESSAGE("   source files: ${CORE_SOURCE}")

add_library(eyetribe SHARED ${CORE_SOURCE})
target_link_libraries(eyetribe core gaze uvc ${OpenCV_LIBS} ${Boost_LIBRARIES})

install(TARGETS eyetribe DESTINATION lib)