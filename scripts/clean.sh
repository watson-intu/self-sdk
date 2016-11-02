#!/bin/bash
# Usage: clean.sh 

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
BUILD_DIR=$DIR/..

rm -rf $BUILD_DIR/packages
rm -rf $BUILD_DIR/.qi
rm -rf $BUILD_DIR/build*

qitoolchain remove nao-self --force
qitoolchain remove mac-self --force
qitoolchain remove raspi-self --force
qitoolchain remove linux-self --force

