file(READ version.txt ULTRASONIC_VERSION)
message("ULTRASONIC_VERSION : ${ULTRASONIC_VERSION}")

# sources
file(GLOB_RECURSE ULTRASONIC_SOURCES ultrasonic/*.*)
add_library(ultrasonic ${ULTRASONIC_SOURCES})

# include dirs
target_include_directories(ultrasonic PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# defines
target_compile_definitions(ultrasonic PUBLIC -DULTRASONIC_VERSION="${ULTRASONIC_VERSION}")

# depends
target_link_libraries(ultrasonic etl)
target_link_libraries(ultrasonic periph)