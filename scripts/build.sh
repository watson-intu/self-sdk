#!/bin/bash
# Usage: build.sh <platform> [profile]

export TARGET=$1

TOOLCHAIN=
UNAME=$(uname)
if [ "$UNAME" == "Darwin" ]; then
	PLATFORM=mac64
	if [ "$TARGET" == "nao" ]; then
		TOOLCHAIN=ctc-mac64-atom-2.4.3.28
	elif [ "$TARGET" == "mac" ]; then
		TOOLCHAIN=naoqi-sdk-2.1.4.13-mac64
	fi
else
	PLATFORM=linux64
	if [ "$TARGET" == "nao" ]; then
		TOOLCHAIN=ctc-linux64-atom.2.4.3.28
	elif [ "$TARGET" == "linux" ]; then
		TOOLCHAIN=naoqi-sdk-2.1.4.13-linux64
	fi
fi

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
BUILD_DIR=$DIR/..
PACKAGES_DIR=$BUILD_DIR/packages
PACKAGE_ID=Self-SDK-$TARGET
PACKAGE_ZIP=$PACKAGE_ID.zip
PACKAGE_DIR=$PACKAGES_DIR/$PACKAGE_ID/

cd $BUILD_DIR

if [ ! -d "$BUILD_DIR/.qi" ]; then
        qibuild init
fi

if [ ! -d $PACKAGES_DIR ]; then
	mkdir -p $PACKAGES_DIR
fi

if [ "$TOOLCHAIN" != "" ]; then
	TOOLCHAIN_ZIP=$TOOLCHAIN.zip	
	if [ ! -f "$PACKAGES_DIR/$TOOLCHAIN_ZIP" ]; then
		cd $PACKAGES_DIR
		echo "Downloading toolchain $TOOLCHAIN_ZIP..."	
		curl "http://75.126.4.99/xray/?action=/download?packageId=$TOOLCHAIN_ZIP" --output $TOOLCHAIN_ZIP
		unzip $TOOLCHAIN_ZIP
		cd $BUILD_DIR
		qitoolchain create $TARGET-self $PACKAGES_DIR/$TOOLCHAIN/toolchain.xml
		qibuild add-config $TARGET-self --toolchain $TARGET-self	
	fi
else
	qitoolchain info $TARGET-self
	if [ $? != 0 ]; then
		echo "Creating toolchain for target: $TARGET..."
		qitoolchain create $TARGET-self
                qibuild add-config $TARGET-self --toolchain $TARGET-self
	fi

fi

if [ ! -f "$PACKAGES_DIR/$PACKAGE_ZIP" ]; then
	echo "Downloading package $PACKAGE_ZIP..."
	cd $PACKAGES_DIR
	curl "http://75.126.4.99/xray/?action=/download?packageId=$PACKAGE_ZIP" --output $PACKAGE_ZIP
	unzip $PACKAGE_ZIP
	cp -R $PACKAGE_ID/bin $BUILD_DIR/
	cp -R $PACKAGE_ID/include $BUILD_DIR/

	if [ -d "${PACKAGE_ID}/toolchain" ]; then	
		echo Installing toolchains...
		for f in ${PACKAGE_ID}/toolchain/*.zip; do
			NAME=$(basename "$f")
			echo "Installing $NAME into toolchain..."
			qitoolchain add-package -c ${TARGET}-self $f --name ${NAME}
		done
	fi
	
	cd $BUILD_DIR
	qibuild configure -c $TARGET-self
fi

qibuild make -c $TARGET-self -j 4
if [ $? -ne 0 ]; then
	echo "Build Error!"
	exit 1
fi

$DIR/stage.sh $1 $2
