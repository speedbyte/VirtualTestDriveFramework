MESSAGE("-- create gaze library target")

aux_source_directory(. CORE_SOURCE)
MESSAGE("   source files: ${CORE_SOURCE}")

add_library(gaze SHARED ${CORE_SOURCE})

install(TARGETS gaze DESTINATION lib)