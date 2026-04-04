cmake_minimum_required(VERSION 3.5)

project(cxxnever_bignum)

enable_testing()

find_package(GTest REQUIRED)

set(CMAKE_CXX_STANDARD 20)

file(GLOB HEADERS include/*/*)

install(FILES ${HEADERS} DESTINATION /usr/include/cxxnever_bignum)

file(GLOB TEST_FILES tests/*.cpp)
list(SORT "${TEST_FILES}")

foreach(TEST ${TEST_FILES})
	cmake_path(RELATIVE_PATH TEST
		BASE_DIRECTORY ${CMAKE_SOURCE_DIR}/tests
		OUTPUT_VARIABLE base_name)
	add_executable(${base_name}.bin ${TEST} ${HEADERS})
	target_include_directories(${base_name}.bin PRIVATE ${CMAKE_SOURCE_DIR}/include)
	target_link_libraries(${base_name}.bin GTest::GTest GTest::Main)
	add_test(NAME ${base_name} COMMAND ./${base_name}.bin
		WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
endforeach()
