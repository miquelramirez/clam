#!/bin/bash

# TODO: Not removing new versions

GenerateDoxygen()
{
	Name=$1
	CheckoutRevision=$2
	RemoteServer=$3
	RemoteLocation=$4
	ProjectName=$Name
	Link=$Name-doxygen
	Date=""
	svn co http://clam-project.org/clam/trunk/CLAM $CheckoutRevision CLAM-for-doxygen
	(
		cd CLAM-for-doxygen
		Version=`grep "CLAM" CHANGES | (read date name version svn revisionTag revision dollar; echo $version )`
		Development=`grep "CLAM" CHANGES | (read date name version svn revisionTag revision dollar; echo $svn )`
		if [ "$Development" == "SVN" ]; then
			Link=$Name-devel-doxygen
			ProjectName="$Name (svn)"
			Revision=`svn info | grep ^Revision | cut -d' ' -f 2`
			Version="$Version~svn$Revision"
			Date=__`date -u +"%Y-%m-%d"`
		fi
		echo Project: $ProjectName
		echo Version: $Version$Date
		echo Generating Doxygen, output will be on the DoxyLog file...
		(
			cat doxygen.cfg
			echo "PROJECT_NAME = $ProjectName"
			echo "PROJECT_NUMBER = $Version $Date"
		) | doxygen - >DoxyLog 2>&1 
		TargetDir=$Name-doxygen-$Version
		Tarball=$TargetDir.tar.bz

		echo creating tarball
		mv doxygen/html $TargetDir
		mv CLAM.tag $TargetDir
		mv DoxyLog $TargetDir
		tar cfvj $Tarball $TargetDir

		echo transferring new tarball
		scp -r $Tarball "$RemoteServer:"
		echo deploying tarball on remote
		ssh $RemoteServer "
			mv $RemoteLocation$TargetDir $RemoteLocation$TargetDir-old
			tar xfvj $Tarball;
			mv $TargetDir $RemoteLocation; 
			rm -f $RemoteLocation$Link; 
			ln -s $TargetDir $RemoteLocation$Link;
			rm $Tarball;
			"
	)
	echo removing local temporary CLAM repository
#	rm -rf CLAM-for-doxygen
}

GenerateDoxygen "CLAM" "$1" "clamadm@clam-project.org" "clam-project.org/doc/"



