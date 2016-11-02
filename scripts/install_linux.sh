#!/bin/bash
#Usage: install_linux.sh <target> [profile]

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
$DIR/install.sh linux $1 $2

