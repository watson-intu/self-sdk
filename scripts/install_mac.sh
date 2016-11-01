#!/bin/bash
#Usage: install_self.sh <target> [profile]

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
$DIR/install.sh mac $1 $2

