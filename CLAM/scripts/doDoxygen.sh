#!/bin/bash

# TODO: Not removing new versions

GenerateDoxygen()
{
	Name=$1
	CheckoutRevision=$2
	TargetDir=$3
	Date=$4
	svn co http://clam-project.org/clam/trunk/CLAM $CheckoutRevision CLAM-for-doxygen
	(
		cd CLAM-for-doxygen
		Version=`grep "CLAM" CHANGES | (read date name version svn revisionTag revision dollar; echo $version )`
		Revision=`grep "CLAM" CHANGES | (read date name version svn revisionTag revision dollar; echo $revision )`
		if [ "$Revision" != "" ]; then
			Version="$Version~svn$Revision"
		fi
		echo Generating Doxygen, output will be on the DoxyLog file...
		(
			cat doxygen.cfg
			echo "PROJECT_NAME = $Name"
			echo "PROJECT_NUMBER = $Version $Date"
		) | doxygen - >DoxyLog 2>&1 

		Tarball=$TargetDir-$Version.tar.bz

		echo creating tarball
		mv doxygen/html $TargetDir
		tar cfvj $Tarball $TargetDir

		echo removing old remote dir $TargetDir
		ssh clamadm@clam-project.org mv $TargetDir $TargetDir-old
		echo transferring new tarball
		scp -r $Tarball "clamadm@clam-project.org:"
		echo extracting new tarball on remote
		ssh clamadm@clam-project.org tar xfvj $Tarball
		echo cleaning remote tarball
		ssh clamadm@clam-project.org rm $Tarball
		echo transferring Doxy Tags
		scp -r CLAM.tag "clamadm@clam-project.org:$TargetDir"
		echo transferring DoxyLog
		scp -r DoxyLog "clamadm@clam-project.org:$TargetDir"
	)
	echo removing local temporary CLAM repository
#	rm -rf CLAM-for-doxygen
}


if [ "$1" == "main" ]
then
	GenerateDoxygen "CLAM" "" "CLAM-doxygen" ""
elif [ "$1" == "devel" ]
then
	Date=`date -u +"%Y-%m-%d"`
	GenerateDoxygen "CLAM-development" "" "CLAM-devel-doxygen" "__$Date"
else
	echo "For main branch:  $0 main "  >&2
	echo "For devel branch:  $0 devel "  >&2
	exit -1
fi




