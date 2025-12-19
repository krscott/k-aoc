add_library(aoc aoc.c)
target_include_directories(aoc PUBLIC ${PROJECT_SOURCE_DIR}/include)


file(
  GLOB MAIN_SRCS
  RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
  "aoc2*.c")

foreach(src IN LISTS MAIN_SRCS)
  get_filename_component(name ${src} NAME_WE) # filename without extension

  add_executable(${name}a ${src} aoc.c)
  target_compile_options(${name}a PUBLIC -DPART_1=1)
  target_include_directories(${name}a PUBLIC ${PROJECT_SOURCE_DIR}/include)
  install(TARGETS ${name}a)

  add_executable(${name}b ${src} aoc.c)
  target_compile_options(${name}b PUBLIC -DPART_2=1)
  target_include_directories(${name}b PUBLIC ${PROJECT_SOURCE_DIR}/include)
  install(TARGETS ${name}b)
endforeach()

