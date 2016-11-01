#!/bin/bash
#Usage: stage.sh <platform> [profile]

if [ ! "$1" != "" ]; then
	echo "Usage: stage.sh <platform> [profile]"
	exit 1
fi

PLATFORM=$1
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
if [ "$BUILD_CONFIG" == "" ]; then 
	BUILD_CONFIG="build-$PLATFORM-self"
fi

BUILD_DIR=$DIR/../$BUILD_CONFIG
BIN_DIR=$BUILD_DIR/sdk/bin
LIB_DIR=$BUILD_DIR/sdk/lib
STAGE_DIR=$DIR/../bin/$PLATFORM

echo Staging files...
mkdir -p $STAGE_DIR

# copy all files into the stage directory
if [ -d $BIN_DIR ]; then 
	cp $BIN_DIR/* $STAGE_DIR/
fi
if [ -d $LIB_DIR ]; then
	cp $LIB_DIR/* $STAGE_DIR/
fi

if [ "$2" != "" ]; then
        $DIR/install_profile.sh $2 $PLATFORM
fi

