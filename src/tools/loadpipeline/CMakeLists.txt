MESSAGE("-- create load pipeline tool target")

aux_source_directory(. CORE_SOURCE)
MESSAGE("   source files: ${CORE_SOURCE}")

add_executable(loadpipeline ${CORE_SOURCE})
target_link_libraries(loadpipeline core utils dl ${OpenCV_LIBS} ${Boost_LIBRARIES} ${EXTERNAL_LIBS})

install(TARGETS loadpipeline DESTINATION bin)