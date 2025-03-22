#/bin/bash

source /etc/profile
DIR=$(cd $(dirname $0);pwd)

clang++ -fsyntax-only -fplugin=$DIR/../build/CheckNewDelete.so -Xclang -plugin -Xclang check-new-delete $DIR/newdelete.cpp

echo "--------------------------------------------------------------"

clang++ -fsyntax-only -fplugin=$DIR/../build/CheckNewDelete.so -Xclang -plugin -Xclang check-new-delete -Xclang -plugin-arg-check-new-delete -Xclang -new-delete-report-error $DIR/newdelete.cpp

