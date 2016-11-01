#!/bin/bash
# Usage: build.sh <platform> [profile]

PLATFORM=$1
export TARGET=$PLATFORM

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
BUILD_DIR=$DIR/..
PACKAGES_DIR=$BUILD_DIR/packages
PACKAGE_ID=Self-SDK-$PLATFORM
PACKAGE_ZIP=$PACKAGE_ID.zip
PACKAGE_DIR=$PACKAGES_DIR/$PACKAGE_ID/

cd $BUILD_DIR

if [ ! -f $PACKAGES_DIR/$PACKAGE_ZIP ]; then
	echo Downloading package $PACKAGE_ZIP...
	mkdir -p $PACKAGES_DIR
	cd $PACKAGES_DIR
	curl "http://75.126.4.99/xray/?action=/download?packageId=$PACKAGE_ZIP" --output $PACKAGE_ZIP
	unzip $PACKAGE_ZIP
	cp -R $PACKAGE_ID/* ../
	rm -rf $PACKAGE_ID
	
	echo Installing toolchains...
	for f in $BUILD_DIR/toolchain/*.zip; do
		qitoolchain add-package -c $PLATFORM $f
	done
	cd $BUILD_DIR
	qibuild configure -c $PLATFORM
fi

qibuild make -c $PLATFORM -j 4
if [ $? -ne 0 ]; then
	echo "Build Error!"
	exit 1
fi

$DIR/stage.sh $1 $2

