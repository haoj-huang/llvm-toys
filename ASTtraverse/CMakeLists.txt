cmake_minimum_required(VERSION 3.13.4)

project(ast-traverse)

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
execute_process(COMMAND llvm-config --libdir 
		OUTPUT_VARIABLE LLVM_LIB_DIR
		OUTPUT_STRIP_TRAILING_WHITESPACE
)

set(CLANG_LIBS 
	${LLVM_LIB_DIR}/libclang.so
)

set(SOURCE_FILES
	ASTtraverse.cpp
)

add_executable(ASTtraverse
	${SOURCE_FILES}
)

llvm_map_components_to_libnames(LLVM_LIBS
core
support
)

target_link_libraries(ASTtraverse 
	PRIVATE
	${LLVM_LIBS}
	${CLANG_LIBS}
)



