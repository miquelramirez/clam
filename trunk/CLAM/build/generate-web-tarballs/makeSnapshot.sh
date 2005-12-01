#! /bin/sh
#cd /home/admin150/tarballs

#module=$1
#tag=$2
#tarname=$3
#maketar=$4
#makezip=$5
#maxsize=$6
#clamver=$7

rm -f out/$3.tar.gz
rm -f out/$3.zip

rm -rf tmp
mkdir tmp

echo == Running cvs export to store $1, tag $2 as $3

#( cd tmp ; cvs -d :ext:admincvs@mtg150.upf.es:/mnt/cvsroot \
#  export -r $2 -d $3 $1 2>&1 | grep -v "^U ")
( cd tmp ; cvs export -r $2 -d $3 $1 2>&1 | grep -v "^U ")

chmod -R u+w tmp

#( cd tmp/$3; chmod +x ./create-symlinks.sh ; ./create-symlinks.sh ; rm ./create-symlinks.sh )

if [ "$6" != "" ]; then
echo == Cleaning .not-public dirs and files bigger than $6 k
else
echo == Cleaning .not-public dirs
fi

perl `dirname $0`/preclean.pl tmp/$3 $6

if [ $7 ]; then
	echo >> tmp/$3/CLAM-version-needed.txt
	echo Compiles with: >> tmp/$3/CLAM-version-needed.txt
	echo $7 >> tmp/$3/CLAM-version-needed.txt
	cp tmp/$3/CLAM-version-needed.txt out/$3.txt
fi

if [ $4 == "1" ]; then 
  echo == Creating out/$3.tar.gz

  ( cd tmp ; tar zcf $3.tar.gz $3 )

  mv tmp/$3.tar.gz out/
fi

if [ $5 == "1" ]; then 
  echo == Converting CR/LFs for cxx,hxx,dsp and txt files

  find tmp/$3 -type f -iname "Makefile" -exec flip -b -m {} \;
  find tmp/$3 -type f -iname "Makefile.*" -exec flip -b -m {} \;
  find tmp/$3 -type f -iname "CHANGELOG" -exec flip -b -m {} \;
  find tmp/$3 -type f -iname "CONTENTS" -exec flip -b -m {} \;
  find tmp/$3 -type f -iname "LICENSE" -exec flip -b -m {} \;
  find tmp/$3 -type f -iname "README" -exec flip -b -m {} \;
  find tmp/$3 -type f -iname "MIDIMessages" -exec flip -b -m {} \;
  find tmp/$3 -type f -iname "README.w32" -exec flip -b -m {} \;
  find tmp/$3 -type f -iname "README.win32" -exec flip -b -m {} \;
  find tmp/$3 -type f -iname "*.cxx" -exec flip -b -m {} \;
  find tmp/$3 -type f -iname "*.cpp" -exec flip -b -m {} \;
  find tmp/$3 -type f -iname "*.cc" -exec flip -b -m {} \;
  find tmp/$3 -type f -iname "*.C" -exec flip -b -m {} \;
  find tmp/$3 -type f -iname "*.c" -exec flip -b -m {} \;
  find tmp/$3 -type f -iname "*.hxx" -exec flip -b -m {} \;
  find tmp/$3 -type f -iname "*.hpp" -exec flip -b -m {} \;
  find tmp/$3 -type f -iname "*.hh" -exec flip -b -m {} \;
  find tmp/$3 -type f -iname "*.H" -exec flip -b -m {} \;
  find tmp/$3 -type f -iname "*.h" -exec flip -b -m {} \;
  find tmp/$3 -type f -iname "*.m" -exec flip -b -m {} \;
  find tmp/$3 -type f -iname "*.sh" -exec flip -b -m {} \;
  find tmp/$3 -type f -iname "*.dsp" -exec flip -b -m {} \;
  find tmp/$3 -type f -iname "*.vcproj" -exec flip -b -m {} \;
  find tmp/$3 -type f -iname "*.dsw" -exec flip -b -m {} \;
  find tmp/$3 -type f -iname "*.txt" -exec flip -b -m {} \;
  find tmp/$3 -type f -iname "*.xml" -exec flip -b -m {} \;
  find tmp/$3 -type f -iname "*.html" -exec flip -b -m {} \;
  find tmp/$3 -type f -iname "*.htm" -exec flip -b -m {} \;
  find tmp/$3 -type f -iname "*.css" -exec flip -b -m {} \;
  find tmp/$3 -type f -iname "*.cfg" -exec flip -b -m {} \;
  find tmp/$3 -type f -iname "*.cfg.in" -exec flip -b -m {} \;
  find tmp/$3 -type f -iname "*.h.in" -exec flip -b -m {} \;
  find tmp/$3 -type f -iname "configure.in" -exec flip -b -m {} \;
  find tmp/$3 -type f -iname "*.m4" -exec flip -b -m {} \;
  find tmp/$3 -type f -iname "*.fl" -exec flip -b -m {} \;
  find tmp/$3 -type f -iname "*.fl.orig" -exec flip -b -m {} \;
  find tmp/$3 -type f -iname "*.ui" -exec flip -b -m {} \;
  find tmp/$3 -type f -iname "*.xpm" -exec flip -b -m {} \;

  echo == Creating out/$3.zip

  (cd tmp ; zip -r $3.zip $3)

  mv tmp/$3.zip out/

fi

echo == Cleaning tmp files

rm -rf tmp

echo == Done
