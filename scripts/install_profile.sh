#!/bin/bash
#Usage: install_profile.sh <profile> <target>

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
ETC_DIR=$DIR/../bin/$2/etc

if [ "$2" != "" ]; then
	echo Installing profile $1...

	rm -rf $ETC_DIR/profile
	mkdir -p $ETC_DIR/profile
	cp -R $DIR/../profiles/$1/* $ETC_DIR/profile/
else
    echo "Usage: install_profile.sh <profile> <target>"
fi

