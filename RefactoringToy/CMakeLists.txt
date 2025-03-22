cmake_minimum_required(VERSION 3.13.4)

project(refactory-toy)

find_package(LLVM REQUIRED CONFIG)
find_package(Clang REQUIRED CONFIG)

separate_arguments(LLVM_DEFINITIONS_LIST NATIVE_COMMAND ${LLVM_DEFINITIONS})
add_definitions(${LLVM_DEFINITIONS_LIST})

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -g -rdynamic -fno-rtti")

list(APPEND CMAKE_MODULE_PATH ${LLVM_CMAKE_DIR})
#include(AddLLVM) #introduce add_llvm_library and ...

include_directories(${LLVM_INCLUDE_DIRS})
include_directories(${CLANG_INCLUDE_DIRS})
execute_process(COMMAND llvm-config --libdir 
		OUTPUT_VARIABLE LLVM_LIB_DIR
		OUTPUT_STRIP_TRAILING_WHITESPACE
)

set(CLANG_LIBS 
	clangBasic
	clangRewrite
	clangASTMatchers
	clangTooling
	clangToolingCore
	clangFrontend
	clangSerialization
	clangAST
)

set(SOURCE_FILES
	RefactoryToy.cpp
)

add_executable(RefactoryToy
	${SOURCE_FILES}
)

llvm_map_components_to_libnames(LLVM_LIBS
core
support
frontendopenmp
)

target_link_libraries(RefactoryToy 
	PRIVATE
	${LLVM_LIBS}
	${CLANG_LIBS}
)



