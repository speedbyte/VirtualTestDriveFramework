MESSAGE("-- create adtf library target")

aux_source_directory(. CORE_SOURCE)
MESSAGE("   source files: ${CORE_SOURCE}")

add_library(adtf SHARED ${CORE_SOURCE})
target_link_libraries( adtf core io gaze saliency tracking utils ${OpenCV_LIBS} ${Boost_LIBRARIES} ${EXTERNAL_LIBS} )

install(TARGETS adtf DESTINATION lib)