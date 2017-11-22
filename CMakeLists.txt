cmake_minimum_required(VERSION 3.8)

set(PROJECT_NAME JWT)

project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 17)

find_package(PkgConfig)
pkg_check_modules(GTEST REQUIRED gtest>=1.7.0)

include_directories(
		${GTEST_INCLUDE_DIRS}
		Jwt_Token/src/include
)

file(GLOB LIB_FILES
		Jwt_Token/src/include/*.h
		Jwt_Token/src/*/*.cpp)

set(SOURCE_FILES Jwt_Token/Source.cpp ${LIB_FILES})

file(GLOB TEST_FILES Jwt_Token/test/*.cpp)

add_executable(test_${PROJECT_NAME} ${TEST_FILES} ${LIB_FILES})

target_link_libraries(test_${PROJECT_NAME} gtest_main gtest)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})