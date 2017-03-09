MESSAGE("-- create saliency library target")

aux_source_directory(. SALIENCY_SOURCE)
aux_source_directory(./activation ACTIVATION_SOURCE)
MESSAGE("   source files: ${SALIENCY_SOURCE} ${ACTIVATION_SOURCE}")

add_library(saliency SHARED ${SALIENCY_SOURCE} ${ACTIVATION_SOURCE})
target_link_libraries( saliency core io utils ${OpenCV_LIBS} ${Boost_LIBRARIES} )

install(TARGETS saliency DESTINATION lib)