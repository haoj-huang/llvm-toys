cmake_minimum_required(VERSION 3.13.4)

project(jit-toy)

find_package(LLVM REQUIRED CONFIG)
find_package(Clang REQUIRED CONFIG)

separate_arguments(LLVM_DEFINITIONS_LIST NATIVE_COMMAND ${LLVM_DEFINITIONS})
add_definitions(${LLVM_DEFINITIONS_LIST})

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -rdynamic -fno-rtti")

list(APPEND CMAKE_MODULE_PATH ${LLVM_CMAKE_DIR})
#include(AddLLVM) #introduce add_llvm_library and ...

include_directories(${LLVM_INCLUDE_DIRS})
include_directories(${CLANG_INCLUDE_DIRS})

set(SOURCE_FILES
	JITtoy.cpp
)

add_executable(JITtoy
	${SOURCE_FILES}
)

llvm_map_components_to_libnames(LLVM_LIBS
core
native
mcjit
irreader
support
bitreader
executionengine
)

target_link_libraries(JITtoy ${LLVM_LIBS})


