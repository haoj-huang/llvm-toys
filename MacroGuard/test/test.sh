#/bin/bash

source /etc/profile
DIR=$(cd $(dirname $0);pwd)

$LLVM_BUILD_PATH/bin/clang -fplugin=$DIR/../build/MacroGuardPlugin.so $DIR/s2.c

echo "--------------------------------------------------"

$LLVM_BUILD_PATH/bin/clang -fplugin=$DIR/../build/MacroGuardPlugin.so $DIR/s3.c


