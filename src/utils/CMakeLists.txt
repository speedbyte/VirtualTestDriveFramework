MESSAGE("-- create utils library target")

aux_source_directory(. CORE_SOURCE)
MESSAGE("   source files: ${CORE_SOURCE}")

add_library(utils SHARED ${CORE_SOURCE})
target_link_libraries(utils ${OpenCV_LIBS} ${Boost_LIBRARIES} ${EXTERNAL_LIBS} ${GRAPHVIZ_LIBRARY})

install(TARGETS utils DESTINATION lib)