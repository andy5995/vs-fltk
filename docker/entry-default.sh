#!/bin/bash

set -e
$WORKSPACE/docker/entry-common.sh

su builder -c "cd $WORKSPACE && . ~/.profile && bash"
