MESSAGE("-- create eyetriberun tool target")

aux_source_directory(. CORE_SOURCE)
MESSAGE("   source files: ${CORE_SOURCE}")

add_executable(eyetriberun ${CORE_SOURCE})
target_link_libraries(eyetriberun core utils dl io eyetribe ${OpenCV_LIBS} ${Boost_LIBRARIES} )

install(TARGETS eyetriberun DESTINATION bin)