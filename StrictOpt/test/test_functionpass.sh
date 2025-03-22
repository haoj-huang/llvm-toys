#/bin/bash

source /etc/profile
DIR=$(cd $(dirname $0);pwd)

echo "/opt/test/s7.ll(original):"
cat /opt/test/s7.ll
echo "===-------------------------------==="

$LLVM_BUILD_PATH/bin/opt --load-pass-plugin=$DIR/../build/StrictOpt.so --passes="function(strict-opt)" $DIR/s7.ll -disable-output -S -o -

