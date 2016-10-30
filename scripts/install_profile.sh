#!/bin/bash
#Usage: install_profile.sh <profile> 

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

STAGE_DIR=$DIR/../stage/
ETC_DIR=$STAGE_DIR/etc/

if [ "$1" != "" ]; then
	echo Installing profile $1...

	rm -rf $ETC_DIR/profile
	mkdir $ETC_DIR/profile
	cp -R $DIR/../profiles/$1/* $ETC_DIR/profile/
else
    echo "Usage: install_profile.sh <profile> <target>"
fi

