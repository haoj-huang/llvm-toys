#/bin/bash

LLVM_CODE_PATH=/opt/llvm_project
LLVM_BUILD_PATH=/opt/build_llvm
mkdir $LLVM_BUILD_PATH

cd $LLVM_BUILD_PATH

cmake -G "Ninja" -DCMAKE_EXPORT_COMPILE_COMMANDS=1 -DLLVM_USE_LINKER=gold -DCMAKE_BUILD_TYPE=Debug -DLLVM_PARALLEL_LINK_JOBS=2 -DLLVM_TARGETS_TO_BUILD="X86" -DBUILD_SHARED_LIBS=ON -DLLVM_OPTIMIZED_TABLEGEN=ON -DLLVM_ENABLE_PROJECTS="clang;clang-tools-extra;compiler-rt;lld" $LLVM_CODE_PATH/llvm


