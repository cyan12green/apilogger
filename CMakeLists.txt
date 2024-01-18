cmake_minimum_required(VERSION 3.20)
project(API_LOGGER CXX)

include(FetchContent)
include(FindPkgConfig)

set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(TEST_DIR "${CMAKE_CURRENT_SOURCE_DIR}/tests")
set(CMAKE_BUILD_TYPE Debug)

#list(APPEND CMAKE_PREFIX_PATH "/home/cyangreen/.local")

set(ENABLE_TESTS ON)
set(ENABLE_PYTHON ON)
set(ENABLE_JS ON)
set(BUILD_SERVER ON)

message(STATUS "Building tests: ${ENABLE_TESTS}")
message(STATUS "Building python: ${ENABLE_PYTHON}")
message(STATUS "Building javascript: ${ENABLE_JS}")

#########
set(_PROTOBUF_PROTOC $<TARGET_FILE:protobuf::protoc>)
set(_GRPC_GRPCPP gRPC::grpc++)
set(_PROTOBUF_LIBPROTOBUF protobuf::libprotobuf)
set(_REFLECTION gRPC::grpc++_reflection)

FetchContent_Declare(
  gRPC
   GIT_REPOSITORY https://github.com/grpc/grpc.git
   GIT_TAG v1.60.0
  )

FetchContent_Declare(
  Protobuf
  GIT_REPOSITORY https://github.com/protocolbuffers/protobuf.git
  GIT_TAG v4.25.2
  )

find_package(Protobuf CONFIG)
message(STATUS "Using protobuf ${Protobuf_VERSION}")

if (NOT Protobuf_FOUND)
  FetchContent_MakeAvailable(Protobuf)
endif()

find_package(gRPC CONFIG)
if (NOT gRPC_FOUND)
  FetchContent_MakeAvailable(gRPC)
endif()

message(STATUS "Using gRPC ${gRPC_VERSION}")

set(_GRPC_CPP_PLUGIN_EXECUTABLE $<TARGET_FILE:gRPC::grpc_cpp_plugin>)
set(_GRPC_PYTHON_PLUGIN_EXECUTABLE $<TARGET_FILE:gRPC::grpc_python_plugin>)

if (BUILD_SERVER)
  FetchContent_Declare(
    yaml-cpp
    GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
    GIT_TAG yaml-cpp-0.7.0
    )

  FetchContent_GetProperties(yaml-cpp)

  if(NOT yaml-cpp_POPULATED)
    message(STATUS "Fetching yaml-cpp...")
    FetchContent_Populate(yaml-cpp)

    add_subdirectory(${yaml-cpp_SOURCE_DIR} ${yaml-cpp_BINARY_DIR})
  endif()
endif()

#FetchContent_Declare(
#  CppKafka
#  GIT_REPOSITORY https://github.com/mfontanini/cppkafka.git
#  GIT_TAG 0.4.0
#  )
#
#FetchContent_MakeAvailable(CppKafka)

#set(CppKafka_DIR ${CMAKE_BINARY_DIR}/package)

if (BUILD_SERVER)
  list(APPEND CMAKE_PREFIX_PATH "/home/cyangreen/.local")
  find_package(CppKafka REQUIRED)
endif()

###############################################################################
#Proto and gRPC files

#C++ grpc and protobuf files
get_filename_component(api_logger_proto "${SRC_DIR}/api_logger.proto" ABSOLUTE)
get_filename_component(api_logger_proto_path "${api_logger_proto}" PATH)
set(api_logger_proto_srcs "${CMAKE_CURRENT_BINARY_DIR}/api_logger.pb.cc")
set(api_logger_proto_hdrs "${CMAKE_CURRENT_BINARY_DIR}/api_logger.pb.h")
set(api_logger_grpc_srcs "${CMAKE_CURRENT_BINARY_DIR}/api_logger.grpc.pb.cc")
set(api_logger_grpc_hdrs "${CMAKE_CURRENT_BINARY_DIR}/api_logger.grpc.pb.h")
add_custom_command(
  OUTPUT "${api_logger_proto_srcs}" "${api_logger_proto_hdrs}" "${api_logger_grpc_srcs}" "${api_logger_grpc_hdrs}"
  COMMAND ${_PROTOBUF_PROTOC}
  ARGS
  --grpc_out "${CMAKE_CURRENT_BINARY_DIR}"
  --cpp_out "${CMAKE_CURRENT_BINARY_DIR}"
  --proto_path="${api_logger_proto_path}"
  --plugin=protoc-gen-grpc="${_GRPC_CPP_PLUGIN_EXECUTABLE}"
  "${api_logger_proto}"
  DEPENDS "${api_logger_proto}")

add_library(api_logger_grpc_proto SHARED
  ${api_logger_grpc_srcs}
  ${api_logger_grpc_hdrs}
  ${api_logger_proto_srcs}
  ${api_logger_proto_hdrs})
target_link_libraries(api_logger_grpc_proto
  ${_REFLECTION}
  ${_GRPC_GRPCPP}
  ${_PROTOBUF_LIBPROTOBUF})
set_property(TARGET api_logger_grpc_proto PROPERTY POSITION_INDEPENDENT_CODE ON)

################################################################################
#Python grpc and protobuf files
if (ENABLE_PYTHON)
set(api_logger_py 
  "${CMAKE_CURRENT_BINARY_DIR}/api_logger_pb2.py"
  "${CMAKE_CURRENT_BINARY_DIR}/api_logger_pb2.pyi"
  "${CMAKE_CURRENT_BINARY_DIR}/api_logger_pb2_grpc.py"
  )
add_custom_command(
  OUTPUT ${api_logger_py}
  COMMAND
  python3 -m grpc_tools.protoc
  -I"${CMAKE_CURRENT_BINARY_DIR}"
  --python_out="${SRC_DIR}/python"
  --pyi_out="${SRC_DIR}/python"
  --grpc_python_out="${SRC_DIR}/python"
  --proto_path="${api_logger_proto_path}"
  "${api_logger_proto}")

add_custom_target( py_command ALL
  DEPENDS ${api_logger_py})
endif()
################################################################################
#Javascript grpc and protobuf files
if(ENABLE_JS)
set(api_logger_js
  "${CMAKE_CURRENT_BINARY_DIR}/api_logger_pb.js"
  "${CMAKE_CURRENT_BINARY_DIR}/api_logger_grpc_web_pb.js"
  )

add_custom_command(
  OUTPUT ${api_logger_js}
  COMMAND protoc
  ARGS
  --proto_path="${api_logger_proto_path}" "${api_logger_proto}" 
  --grpc-web_out=import_style=commonjs,mode=grpcweb:${SRC_DIR}/javascript
  --js_out=import_style=commonjs:${SRC_DIR}/javascript
  )
add_custom_target( js_command ALL
  DEPENDS ${api_logger_js})
endif()
################################################################################
#Servers in src as executables
if (BUILD_SERVER)
foreach(_target 
    api_logger_server 
    )
  add_executable(${_target} 
    "${SRC_DIR}/cpp/${_target}.cc" 
    )
  target_link_libraries(${_target}
    api_logger_grpc_proto
    ${_REFLECTION}
    ${_GRPC_GRPCPP}
    ${_PROTOBUF_LIBPROTOBUF}
    yaml-cpp::yaml-cpp
    CppKafka::cppkafka
    )
endforeach()
endif()
################################################################################
#creating the libraries
set(LIB_SOURCE 
  ${SRC_DIR}/cpp/api_logger_client.h
  ${SRC_DIR}/cpp/api_logger_client.cc   

  ${api_logger_grpc_srcs}
  ${api_logger_grpc_hdrs}
  ${api_logger_proto_srcs}
  ${api_logger_proto_hdrs}
  )
  

# .so lib file
add_library(api_logger_module SHARED 
  ${LIB_SOURCE})
target_link_libraries(api_logger_module
  ${_REFLECTION}
  ${_GRPC_GRPCPP}
  ${_PROTOBUF_LIBPROTOBUF}
) 

############################################################################
############################################################################
#tests
if (ENABLE_TESTS)
  add_subdirectory(${TEST_DIR})
endif()
############################################################################

