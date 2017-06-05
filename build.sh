#!/bin/bash
#

show_help(){
	echo -e "param error! \n-r: to build release version \n-d: to build debug version \n-rz: build release to tar.gz\n-dz: build debug to tar.gz"
}

# default value is release
OUTPATH=output/release
TARGPATH=univ
MAKEFLAG=release

build(){
	cd $TARGPATH
	make clean
	make $MAKEFLAG
	cd ..
	rm -rdf OUTPATH
	mkdir -p $OUTPATH
	cp -f $TARGPATH/libuniv.so $OUTPATH/libuniv.so
	mkdir -p $OUTPATH/include/univ/
	cp -f $TARGPATH/*.h $OUTPATH/include/univ/
	cp -f $TARGPATH/sloong.conf $OUTPATH/sloong.conf
	cp -f install.sh $OUTPATH/install.sh
}

build_debug(){
	OUTPATH=output/debug
	MAKEFLAG=debug
	build
}

zip(){
	VERSION_STR=$(cat version)
	tar -czf libuniv_v$VERSION_STR.tar.gz $OUTPATH/*
}


if [ $# -lt 1 ]
then
	show_help
else
	case $1 in 
		-r) build;;
		-d) build_debug;;
		-rz) 
			build
			zip;;
		-dz) 
			build_debug
			zip;;
		* ) show_help;;
	esac
	
fi
