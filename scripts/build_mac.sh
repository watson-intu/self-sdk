#!/bin/bash
# Usage: build_mac.sh [profile]

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
$DIR/build.sh mac $1

