#!/bin/bash
#
#echo '$0: '$0  
#echo "pwd: "`pwd`  
#echo "scriptPath1: "$(cd `dirname $0`; pwd)  
#echo "scriptPath2: "$(dirname $(readlink -f $0))

#WORKFOLDER=`pwd`
#echo "Workfolder: "$WORKFOLDER


show_help(){
	echo -e "build.sh [module] [operation]
module: proxy|control|firewall|process|data
operation:
	-r: to build release version 
	-d: to build debug version 
	-rz: build release to tar.gz
	-dz: build debug to tar.gz"
}


SCRIPTFOLDER=$(dirname $(readlink -f $0))
#echo "ScriptFolder: "$SCRIPTFOLDER
# cd to current file folder
cd $SCRIPTFOLDER

# default value is debug
VERSION_STR=$(cat $SCRIPTFOLDER/../version)
PROJECT=libuniv

init(){
	MAKEFLAG=debug
	CMAKE_FILE_PATH=$SCRIPTFOLDER/../univ
}

clean(){
	rm -rdf $MAKEFLAG
}

build(){
	if [ ! -d $MAKEFLAG  ];then
		mkdir $MAKEFLAG
	fi
	cd $MAKEFLAG
	cmake -DCMAKE_BUILD_TYPE=$MAKEFLAG $CMAKE_FILE_PATH
	make
	cd ../
}

build_debug(){
	OUTPATH=$PROJECT-debug-v$VERSION_STR
	MAKEFLAG=debug
	clean
	build
}

build_release(){
	OUTPATH=$PROJECT-v$VERSION_STR
	MAKEFLAG=release
	clean
	build
}

zip(){
	tar -cf $OUTPATH.tar -C $MAKEFLAG/ $PROJECT.so

}


# -eq 等于,
# -ne 不等于
# -gt 大于,
# -ge 大于等于,
# -lt 小于,
# -le 小于等于
if [ $# -eq 0 ]; then
	build
	exit
fi

if [ $# -ge 1 ]; then
	case $1 in 
		-r) build_release;;
		-d) build_debug;;
		-rz) 
			build_release
			zip;;
		-dz) 
			build_debug
			zip;;
		* ) show_help;;
	esac
fi

