MESSAGE("-- create io library target")

aux_source_directory(. CORE_SOURCE)
MESSAGE("   source files: ${CORE_SOURCE}")

add_library(io SHARED ${CORE_SOURCE})

install(TARGETS io DESTINATION lib)