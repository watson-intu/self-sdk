#!/bin/bash
#Usage: install_raspi.sh <target> [profile]

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
$DIR/install.sh raspi $1 $2
