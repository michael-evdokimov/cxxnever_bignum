cmake_minimum_required(VERSION 3.5)

project(cxxnever_bignbum)

enable_testing()

find_package(GTest REQUIRED)

set(CMAKE_CXX_FLAGS "-ggdb3")

add_executable(tester tests/test.cpp)

target_link_libraries(tester GTest::GTest GTest::Main)

target_include_directories(tester PRIVATE ${CMAKE_SOURCE_DIR}/src)
