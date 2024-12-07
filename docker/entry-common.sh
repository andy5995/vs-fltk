#!/bin/bash
# entry-common.sh

set -e

if [ -z "$HOSTUID" ]; then
  echo "HOSTUID is not set."
  exit 1
fi

if [ -z "$HOSTGID" ]; then
  echo "HOSTGID is not set."
  exit 1
fi

usermod -u $HOSTUID builder
groupmod -g $HOSTGID builder

echo 'export VS_COMMONS_DIR="$WORKSPACE"/build/commons/' >> /home/builder/.profile
echo 'export VS_LOG_LEVEL=debug' >> /home/builder/.profile
