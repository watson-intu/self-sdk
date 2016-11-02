#!/bin/bash
# Usage: install_tc.sh.sh <target>  
# Install toolchain for the given target

export TARGET=$1

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
BUILD_DIR=$DIR/..
PACKAGES_DIR=$BUILD_DIR/packages
STORE_URL="http://75.126.4.99/xray/?action="
TC_NAME=$TARGET-self

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

if [ ! -d $PACKAGES_DIR ]; then
        mkdir -p $PACKAGES_DIR
fi

if [ "$TOOLCHAIN" != "" ]; then
		cd $PACKAGES_DIR
		TOOLCHAIN_ZIP=$TOOLCHAIN.zip
		echo "Downloading toolchain $TOOLCHAIN_ZIP..."
		curl "${STORE_URL}/download?packageId=$TOOLCHAIN_ZIP" --output $TOOLCHAIN_ZIP
		unzip $TOOLCHAIN_ZIP
		cd $BUILD_DIR
		qitoolchain create $TC_NAME $PACKAGES_DIR/$TOOLCHAIN/toolchain.xml
		if [ $? != 0 ]; then exit 1; fi
		qibuild add-config $TC_NAME --toolchain $TC_NAME
		if [ $? != 0 ]; then exit 1; fi
else
		cd $BUILD_DIR
		echo "Creating toolchain for target: $TARGET..."
		qitoolchain create $TC_NAME
		qibuild add-config $TC_NAME --toolchain $TC_NAME
fi

PACKAGE_ID=Self-SDK-$TARGET
PACKAGE_ZIP=$PACKAGE_ID.zip
PACKAGE_DIR=$PACKAGES_DIR/$PACKAGE_ID/

echo "Downloading package $PACKAGE_ZIP..."
cd $PACKAGES_DIR
curl "${STORE_URL}/download?packageId=$PACKAGE_ZIP" --output $PACKAGE_ZIP
rm -rf $PACKAGE_ID
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


