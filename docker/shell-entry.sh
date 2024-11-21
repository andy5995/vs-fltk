#!/bin/bash

set -e
/docker/main_entry.sh

su builder -l -c "cd /workspace && bash"
