add_library(aoc day01.c)

target_include_directories(aoc PUBLIC ${PROJECT_SOURCE_DIR}/include)
# target_sources(aoc PRIVATE file.c)

install(TARGETS aoc)
