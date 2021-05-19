#!/bin/bash

###Snippet from http://stackoverflow.com/questions/59895/
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
###end snippet

CONTAINER_VERSION="v0.1"

docker build -t luntlab/snot_build:${CONTAINER_VERSION} --target snot_build ${SCRIPT_DIR} || exit 1
docker tag luntlab/snot_build:${CONTAINER_VERSION} luntlab/snot_build:latest

#docker push luntlab/snot_build:${CONTAINER_VERSION}
#docker push luntlab/snot_build:latest
