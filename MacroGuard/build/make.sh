#/bin/bash
#LLVM_BUILD_PATH defined in /etc/profile

cmake -DLLVM_DIR=$LLVM_BUILD_PATH/lib/cmake/llvm -DClang_DIR=$LLVM_BUILD_PATH/lib/cmake/clang ..


