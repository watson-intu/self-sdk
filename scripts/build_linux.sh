#!/bin/bash
# Usage: build_linux.sh <target> [profile]

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
BUILD_DIR=$DIR/..

cd $BUILD_DIR/
export TARGET=LINUX
qibuild make -c linux
if [ $? -ne 0 ]; then
	cd $DIR
	echo "Build Error!"
	exit 1
fi

cd $DIR
./install_linux.sh $1 $2
