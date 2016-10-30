#!/bin/bash
#Usage: stage_nao.sh [profile]

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
if [ "$BUILD_CONFIG" == "" ]; then 
	BUILD_CONFIG="build-sota"
fi

BUILD_DIR=$DIR/../$BUILD_CONFIG
BIN_DIR=$BUILD_DIR/sdk/bin
LIB_DIR=$BUILD_DIR/sdk/lib
NAO_BIN_DIR=$DIR/../bin/sota
STAGE_DIR=$DIR/../stage

echo Staging files...
if [ -d $STAGE_DIR ]; then
	rm -rf $STAGE_DIR
fi
mkdir -p $STAGE_DIR

# copy all files into the stage directory
cp -R $NAO_BIN_DIR/* $STAGE_DIR/
cp $BIN_DIR/* $STAGE_DIR/
cp $LIB_DIR/* $STAGE_DIR/

if [ "$1" != "" ]; then
        $DIR/install_profile.sh $1 
fi

