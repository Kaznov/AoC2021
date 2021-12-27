cmake_minimum_required(VERSION 3.20)

project(AoC2021)

include(cmake/StandardProjectSettings.cmake)
include(cmake/PreventInSourceBuilds.cmake)
# Link this 'library' to use the warnings specified in CompilerWarnings.cmake
add_library(project_warnings INTERFACE)

include(cmake/CompilerWarnings.cmake)
set_project_warnings(project_warnings)

# allow for static analysis options
include(cmake/StaticAnalyzers.cmake)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB SOURCE_FILES *.cpp)
foreach(SOURCE_PATH ${SOURCE_FILES})
  get_filename_component(EXECUTABLE_NAME ${SOURCE_PATH} NAME_WE)
  add_executable(${EXECUTABLE_NAME} ${SOURCE_PATH})
  target_link_libraries(${EXECUTABLE_NAME} project_warnings)
endforeach(SOURCE_PATH ${SOURCE_FILES})
