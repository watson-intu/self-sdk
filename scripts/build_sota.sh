#!/bin/bash
# Usage: build_sota.sh [profile]

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
$DIR/build.sh sota $1

