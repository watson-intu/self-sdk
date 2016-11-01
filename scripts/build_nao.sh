#!/bin/bash
# Usage: build_nao.sh [profile]

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
$DIR/build.sh nao $1

