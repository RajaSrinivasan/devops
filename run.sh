#!/bin/bash

docker run --rm \
  --mount type=bind,source=$HOME/Workspace,target=/ws \
  --rm -p 2322:2322 devopsnative:latest

docker run --rm \
  --mount type=bind,source=$HOME/Workspace,target=/ws \
  --rm -p 2222:22 devopsbuild:latest