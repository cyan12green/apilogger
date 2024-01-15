cmake_minimum_required(VERSION 3.20)

find_package(PythonLibs REQUIRED)


file(GLOB SRC "*.cc")
list(TRANSFORM SRC REPLACE "${CMAKE_CURRENT_SOURCE_DIR}/(.*).cc" "\\1")
foreach(_target
    ${SRC})
  add_executable(${_target} "${_target}.cc")
  target_link_libraries(${_target}
    api_logger_module
    yaml-cpp::yaml-cpp
    )
endforeach()

