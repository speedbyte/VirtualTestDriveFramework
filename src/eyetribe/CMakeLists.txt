MESSAGE("-- create eyetribe library target")

aux_source_directory(. CORE_SOURCE)
MESSAGE("   source files: ${CORE_SOURCE}")

add_library(eyetribe SHARED EXCLUDE_FROM_ALL ${CORE_SOURCE})
target_link_libraries(eyetribe core gaze plot uvc ${OpenCV_LIBS} ${Boost_LIBRARIES})

install(TARGETS eyetribe DESTINATION lib OPTIONAL)