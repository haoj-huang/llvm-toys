#/bin/bash

DIR=$(cd $(dirname $0);pwd)

$DIR/../build/ASTtraverse $DIR/sum.c

