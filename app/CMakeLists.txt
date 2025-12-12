add_executable(aoc-app main.c)

target_link_libraries(aoc-app aoc)

install(TARGETS aoc-app)
