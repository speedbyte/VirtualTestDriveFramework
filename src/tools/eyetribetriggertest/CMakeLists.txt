MESSAGE("-- create eyetribtriggertest tool target")

aux_source_directory(. CORE_SOURCE)
MESSAGE("   source files: ${CORE_SOURCE}")

add_executable(eyetribtriggertest ${CORE_SOURCE})
target_link_libraries(eyetribtriggertest core utils dl io eyetribe ${OpenCV_LIBS} ${Boost_LIBRARIES} ${GRAPHVIZ_LIBRARY} )

install(TARGETS eyetribtriggertest DESTINATION bin)