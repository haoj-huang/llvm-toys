#/bin/bash

source /etc/profile
DIR=$(cd $(dirname $0);pwd)

echo "$DIR/test.ll(original):"
cat $DIR/test.ll
echo "===-------------------------------==="

$LLVM_BUILD_PATH/bin/opt -O2 --enable-new-pm --load-pass-plugin=$DIR/../build/StrictOpt.so  $DIR/test.ll -S -o -

