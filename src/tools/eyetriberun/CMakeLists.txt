MESSAGE("-- create eyetriberun tool target")

aux_source_directory(. CORE_SOURCE)
MESSAGE("   source files: ${CORE_SOURCE}")

add_executable(eyetriberun EXCLUDE_FROM_ALL ${CORE_SOURCE})
target_link_libraries(eyetriberun core utils dl io eyetribe ${OpenCV_LIBS} ${Boost_LIBRARIES} )