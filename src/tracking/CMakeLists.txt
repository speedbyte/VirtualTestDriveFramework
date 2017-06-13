MESSAGE("-- create tracking library target")

aux_source_directory(. CORE_SOURCE)
MESSAGE("   source files: ${CORE_SOURCE}")

add_library(tracking SHARED ${CORE_SOURCE})
target_link_libraries( tracking core io ${OpenCV_LIBS} ${Boost_LIBRARIES} ${EXTERNAL_LIBS} )

install(TARGETS tracking DESTINATION lib)