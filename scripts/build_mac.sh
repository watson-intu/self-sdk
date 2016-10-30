#!/bin/bash
# Usage: build_self.sh [profile]

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
BUILD_DIR=$DIR/..

cd $BUILD_DIR/
export TARGET=MAC
qibuild make -c mac
if [ $? -ne 0 ]; then
	cd $DIR
	echo "Build Error!"
	exit 1
fi

cd $DIR
./stage_mac.sh $@

