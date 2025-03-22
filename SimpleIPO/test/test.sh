#/bin/bash

source /etc/profile
DIR=$(cd $(dirname $0);pwd)

$LLVM_BUILD_PATH/bin/opt -stats -debug-only="simple-ipo" --load-pass-plugin=$DIR/../build/SimpleIPO.so --passes="simple-ipo" $DIR/test.ll -disable-output -S -o -

