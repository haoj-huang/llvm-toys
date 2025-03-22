#bin/bash

source /etc/profile
DIR=$(cd $(dirname $0);pwd)

$LLVM_BUILD_PATH/bin/clang -fsyntax-only -fplugin=$DIR/../build/TernaryConverter.so -Xclang -plugin -Xclang ternary-converter $DIR/test.c

