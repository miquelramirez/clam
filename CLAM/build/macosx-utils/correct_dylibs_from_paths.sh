#!/bin/bash

usage()
{
  echo ""
  echo "Usage: $0 [executable] [-L<librarypath> ...]"
  echo ""
  echo "  Modifies all dynamic library binding in [executable]"
  echo "  to point to the matching libraries found in the library"
  echo "  paths specified"
  echo ""
}

sep=""
libs=""
libspaths=""
file=""
while [ "$1" != "" ]; do
  case $1 in
    -L*)
      if [ "$libspaths" == "" ]; then
        libspaths=`echo $1 | cut -c3-`
      else
        libspaths=$libspaths" "`echo $1 | cut -c3-`
      fi
      ;;
		-*)
			echo ""
			echo "Error: invalid option $1"
			usage
			exit -1
			;;
    *)
      if [ "$file" != "" ]; then
      	echo ""
        echo "Error: you can only specify one executable"
        echo "  previously specified: $file"
        echo "  while parsing argument: $1"
        usage
        exit -1
      fi
      file=$1
      ;;
  esac
  shift
done

if [ "$file" == "" ]; then
  echo ""
  echo "Error: missing argument for executable"
  usage
  exit -1
fi

if [ ! -e "$file" ]; then
  echo ""
  echo "Error: could not find $file"
  usage
  exit -1
fi

if [ "$libspaths" == "" ]; then
  echo ""
  echo "Error: no library paths specified"
  usage
  exit -1
fi

libs=`find $libspaths -name "*.dylib"`

if [ "$libs" == "" ]; then
  echo ""
  echo "Error: no libraries found in $libpaths"
  usage
  exit -1
fi

for lib in $libs; do
  dir=`dirname $lib`
  dir=`cd $dir ; pwd` 
  basename=`basename $lib`
  lib="$dir/$basename"
  echo $dir $basename
  tochanges=`otool -L $file | grep -v : | tr '\11' ' ' | grep "[/ ]$basename" | cut -d' ' -f2`
  for tochange in $tochanges; do
    if [ "$tochange" != "$lib" ]; then 
#      echo install_name_tool -change $tochange $lib $file
      install_name_tool -change $tochange $lib $file
    fi
  done
done
