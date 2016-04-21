#!/bin/bash
rm -f univ/version.h
_GIT_VERSION=`git rev-list --all | wc -l`

cat version.h.template | sed "s/\$GIT_VERSION/$_GIT_VERSION/g" > univ/version.h

echo "Generated version.h"

git add univ/version.h