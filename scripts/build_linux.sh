#!/bin/bash
# Usage: build_linux.sh [profile]

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
$DIR/build.sh linux $1

