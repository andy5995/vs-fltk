#!/bin/bash

OLDPWD=$PWD

if [ -z "HOSTUID" ]; then
  echo "HOSTUID is not set."
  exit 1
fi

if [ -z "HOSTGID" ]; then
  echo "HOSTGID is not set."
  exit 1
fi

usermod -u $HOSTUID builder
groupmod -g $HOSTGID builder
chown -R $HOSTUID:$HOSTGID /home/builder
su builder -c "PATH=/home/builder/.local/bin:$PATH && cd $OLDPWD && $1"
