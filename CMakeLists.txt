cmake_minimum_required(VERSION 3.8)

set(PROJECT_NAME JWT)

project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 17)

set(SOURCE_FILES Jwt_Token/Source.cpp Jwt_Token Jwt_Token/Base64/Base64Encode.cpp Jwt_Token/Json/Json.cpp Jwt_Token/RSA/RSA.cpp)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})