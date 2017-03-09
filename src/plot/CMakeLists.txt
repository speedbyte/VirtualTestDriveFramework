MESSAGE("-- create plot library target")

aux_source_directory(. CORE_SOURCE)
MESSAGE("   source files: ${CORE_SOURCE}")

add_library(plot SHARED ${CORE_SOURCE})
target_link_libraries( plot core io utils ${OpenCV_LIBS} ${Boost_LIBRARIES} )

install(TARGETS plot DESTINATION lib)