#!/bin/bash

docker run --rm \
  --mount type=bind,source=$HOME/Workspace,target=/ws \
  --rm -p 2222:22 devops:latest