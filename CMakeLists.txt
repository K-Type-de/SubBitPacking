if(COMMAND idf_component_register)
# If the project is a submodule of an ESP-IDF project, register it as component
idf_component_register(INCLUDE_DIRS "src")
return()
endif()

cmake_minimum_required(VERSION 3.14)
project(SubBitPackedArrayLib)

add_library(SubBitPackedArray INTERFACE)
target_include_directories(SubBitPackedArray INTERFACE src)
