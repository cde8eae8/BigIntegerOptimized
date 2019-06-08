cmake_minimum_required(VERSION 3.13)
project(BigIntegerOptimized)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic -Wall -Wextra") # -Wcast-align -Wcast-qual -Wdisabled-optimization -Wformat=2 -Winit-self -Wlogical-op -Wmissing-include-dirs -Wnoexcept -Wold-style-cast -Woverloaded-virtual -Wredundant-decls -Wconversion -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=5 -Wswitch-default -Wundef -Wunused")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -ggdb -std=c++11 -fsanitize=address")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

add_executable(BigIntegerOptimized main.cpp big_integer.cpp big_integer.h NumberHolder.cpp NumberHolder.h)

add_executable(tests tests/tets.cpp tests/task_correctness.cpp tests/all_tests.cpp big_integer.cpp NumberHolder.cpp NumberHolder.h)
add_executable(tests2 alltest2.cpp big_integer.cpp NumberHolder.cpp NumberHolder.h)

target_link_libraries(tests gtest gtest_main)
target_link_libraries(tests2 gtest gtest_main)