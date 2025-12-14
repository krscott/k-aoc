add_library(aoc aoc.c
  day01.c
  day02.c
  day03.c
  day04.c
  day05.c
  day06.c
  day07.c
  # day08.c
  # day09.c
  # day10.c
  # day11.c
  # day12.c
  # day13.c
  # day14.c
  # day15.c
  # day16.c
  # day17.c
  # day18.c
  # day19.c
  # day20.c
  # day21.c
  # day22.c
  # day23.c
  # day24.c
  # day25.c
)

target_include_directories(aoc PUBLIC ${PROJECT_SOURCE_DIR}/include)
# target_sources(aoc PRIVATE file.c)

install(TARGETS aoc)
