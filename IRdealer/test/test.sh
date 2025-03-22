#/bin/bash

source /etc/profile
DIR=$(cd $(dirname $0);pwd)

$LLVM_BUILD_PATH/bin/opt --load-pass-plugin=$DIR/../build/IRdealer.so --passes="function(ir-dealer)" $DIR/test.ll -disable-output -S -o -


