#!/bin/bash
#Usage: install_self.sh <target> [profile]

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
if [ "$BUILD_CONFIG" == "" ]; then 
	BUILD_CONFIG="build-pepper"
fi

STAGE_DIR=$DIR/../stage/
$DIR/stage_nao.sh $2

if [ "$1" != "" ]; then
	echo Stopping self instance...
	ssh $1 "killall -INT self_instance"
	ssh $1 "killall -9 run_self.sh"
	sleep 5
	ssh $1 "killall -9 self_instance"

	if (! ssh $1 '[ -d ~/self/dev ]')
	then
	{
    		ssh $1 "mkdir -p ~/self/dev"
	}
	fi
	echo Installing self on $1...
	rsync -tru --inplace --progress $STAGE_DIR/* $1:~/self/dev/
	if [ $? -ne 0 ]; then
		exit 1
	fi
	ssh $1 "rm ~/self/latest; ln -s ~/self/dev ~/self/latest"
fi

