#!/bin/bash
#Usage: install.sh <platform> <target> [profile]

if [ ! "$1" != "" ]; then
	echo "Usage: install.sh <platform> <target> [profile]"
	exit 1
fi

PLATFORM=$1
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

STAGE_DIR=$DIR/../bin/$PLATFORM/
$DIR/build.sh $PLATFORM $3

if [ "$2" != "" ]; then
	echo Stopping self instance...
	ssh $2 "killall -INT self_instance"
	ssh $2 "killall -9 run_self.sh"
	sleep 5
	ssh $2 "killall -9 self_instance"
    	ssh $2 "mkdir -p ~/self/dev"
	
	echo Installing self on $2...
	rsync -tru --inplace --progress $STAGE_DIR/* $2:~/self/dev/
	if [ $? -ne 0 ]; then
		exit 1
	fi
	ssh $2 "rm ~/self/latest; ln -s ~/self/dev ~/self/latest"
fi

