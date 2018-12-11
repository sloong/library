#!/bin/bash
# 
# the version resource text file 
VER_FILE_PATH=univ/version.h
# the version file
VER_PATH=version
# the version template file
VER_TEMPLATE_PATH=version.template
# remove old version file
rm -f $VER_FILE_PATH
rm -f $VER_PATH
# get git version
_GIT_VERSION_VALUE=`git rev-list --all | wc -l`
# build version text with template file. set text 'GET_VERSION' to value of git version.
VER_TEXT=$(cat $VER_TEMPLATE_PATH | sed "s/GIT_VERSION/$_GIT_VERSION_VALUE/g")
echo "$VER_TEXT" > $VER_FILE_PATH

for  i  in  `cat $VER_TEMPLATE_PATH`
do
	case $i in
		L*"GIT_VERSION"*) VER_TEXT=$i;;
	esac
done
# build version file
VER_TEXT=${VER_TEXT/GIT_VERSION/$_GIT_VERSION_VALUE}
VER_TEXT=${VER_TEXT#*\"}
VER_TEXT=${VER_TEXT%\"*}

echo $VER_TEXT > $VER_PATH
# update version to git
git add $VER_FILE_PATH
git add $VER_PATH