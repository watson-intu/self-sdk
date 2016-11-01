#!/bin/bash
#Usage: install_nao.sh <target> [profile]

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
$DIR/install.sh nao $1 $2

