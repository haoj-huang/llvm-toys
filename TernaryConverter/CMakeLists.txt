cmake_minimum_required(VERSION 3.13.4)

project(ternary-converter)

find_package(LLVM REQUIRED CONFIG)
find_package(Clang REQUIRED CONFIG)

separate_arguments(LLVM_DEFINITIONS_LIST NATIVE_COMMAND ${LLVM_DEFINITIONS})
add_definitions(${LLVM_DEFINITIONS_LIST})

set(CMAKE_CXX_STANDARD 14)

list(APPEND CMAKE_MODULE_PATH ${LLVM_CMAKE_DIR})
include(AddLLVM) #introduce add_llvm_library and ...

include_directories(${LLVM_INCLUDE_DIRS})
include_directories(${CLANG_INCLUDE_DIRS})

set(SOURCE_FILES
	TernaryConverter.cpp
	TernaryConverterCallback.cpp
)

add_llvm_library(TernaryConverter MODULE
	${SOURCE_FILES}
	PLUGIN_TOOL clang
)

