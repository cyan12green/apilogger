cmake_minimum_required(VERSION 3.22)

project(test)

add_definitions(-std=c++17)
set (CMAKE_CXX_STANDARD 17)

#include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../cpp")
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE BOTH)
set(CMAKE_PREFIX_PATH "/home/cyangreen/.local/lib")

include_directories("/home/cyangreen/.local/include")
list(APPEND CMAKE_MODULE_PATH "/home/cyangreen/.local")
#set(wolfssl_DIR "/home/cyangreen/.local/lib/cmake/wolfssl")
find_package(wolfssl CONFIG REQUIRED
  #HINTS "/home/cyangreen/.local/lib/cmake/wolfssl"
  )
message(STATUS "Used config file: ${wolfssl_CONFIG}")

#find_package(gRPC CONFIG REQUIRED)

#message(STATUS "Using gRPC ${gRPC_VERSION}")
#set(_GRPC_GRPCPP gRPC::grpc++)

add_library(keyex_module
  SHARED
  IMPORTED)

set_target_properties(keyex_module
  PROPERTIES IMPORTED_LOCATION
  ${CMAKE_CURRENT_SOURCE_DIR}/../../../cmake/build/libkeyex_module.so)

	add_executable(calc calc.cpp)
  target_link_libraries(calc 
    #${_GRPC_GRPCPP}
    keyex_module)
