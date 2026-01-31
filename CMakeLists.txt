cmake_minimum_required(VERSION 3.5)

project(cxxnever_bignbum)

enable_testing()

find_package(GTest REQUIRED)

file(GLOB HEADERS include/*/*)

install(FILES ${HEADERS} DESTINATION /usr/include/cxxnever_bignum)

add_executable(tester tests/test.cpp ${HEADERS})

target_link_libraries(tester GTest::GTest GTest::Main)

target_include_directories(tester PRIVATE ${CMAKE_SOURCE_DIR}/include)

add_test(NAME tester COMMAND tester)
