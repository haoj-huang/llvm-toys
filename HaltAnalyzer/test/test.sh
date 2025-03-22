#/bin/bash

source /etc/profile
DIR=$(cd $(dirname $0);pwd)

$LLVM_BUILD_PATH/bin/opt -O0 --enable-new-pm --load-pass-plugin=$DIR/../build/HaltAnalyzer.so $DIR/test.ll --disable-output

