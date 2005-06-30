#!/bin/bash

# TODO: Not removing new versions

GenerateDoxygen()
{
	Name=$1
	Branch=$2
	TargetDir=$3
	Date=$4
	cvs co $Branch -d CLAM-for-doxygen CLAM
	pushd CLAM-for-doxygen
		Version=`grep PROJECT_NUMBER doxygen.cfg | (read var eq value; echo $value )`
		echo Generating Doxygen, output will be on the DoxyLog file...
		(
			cat doxygen.cfg
			echo "PROJECT_NAME = $Name"
			echo "PROJECT_NUMBER = $Version $Date"
		) | doxygen - >DoxyLog 2>&1 

		Tarball=$TargetDir.tar.bz

		echo creating tarball
		mv doxygen/html $TargetDir
		tar cfvj $Tarball $TargetDir

		echo removing old remote dir $TargetDir
		ssh clamadm@www.iua.upf.es rm -rf $TargetDir
		echo transferring new tarball
		scp -r $Tarball "clamadm@www.iua.upf.es:"
		echo extracting new tarball on remote
		ssh clamadm@www.iua.upf.es tar xfvj $Tarball
		echo cleaning remote tarball
		ssh clamadm@www.iua.upf.es rm $Tarball
		echo transferring Doxy Tags
		scp -r CLAM.tag "clamadm@www.iua.upf.es:$TargetDir"
		echo transferring DoxyLog
		scp -r DoxyLog "clamadm@www.iua.upf.es:$TargetDir"
	popd
	echo removing local temporary CLAM repository
	rm -rf CLAM-for-doxygen
}


if [ "$1" == "main" ]
then
	GenerateDoxygen "CLAM" "-rCLAM-stable" "CLAM-doxygen" ""
elif [ "$1" == "devel" ]
then
	Date=`date -u +"%Y-%m-%d"`
	GenerateDoxygen "CLAM-development" "" "CLAM-devel-doxygen" "__$Date"
else
	echo "For main branch:  $0 main "  >&2
	echo "For devel branch:  $0 devel "  >&2
	exit -1
fi




