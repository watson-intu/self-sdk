#!/bin/bash
# Usage: build_self.sh <target> [profile]

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
BUILD_DIR=$DIR/..

cd $BUILD_DIR/
export TARGET=SOTA
qibuild make -c sota
if [ $? -ne 0 ]; then
	cd $DIR
	echo "Build Error!"
	exit 1
fi

cd $DIR
./install_sota.sh $1 $2

