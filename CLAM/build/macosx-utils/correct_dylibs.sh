#!/bin/bash

app=$1
shift
sep=""
libs=""

usage()
{
  echo ""
  echo "Usage: $0 [executable] [library ...]"
  echo ""
  echo "  Modifies all dynamic library binding in [executable]"
  echo "  to point to the matching libraries found in the list"
  echo "  of libraries specified"
  echo ""
}

if [ "$app" == "" ]; then
  echo ""
  echo "Error: missing argument for executable"
  usage
  exit -1
fi

if [ ! -e $app ]; then
  echo ""
  echo "Error: could not find $app"
  usage
  exit -1
fi

for lib in $*; do
  dir=`dirname $lib`
  dir=`cd $dir ; pwd` 
  basename=`basename $lib`
  libs="$libs$sep$dir/$basename"
  if [ ! -e $dir/$basename ]; then
    echo ""
    echo "Error: could not find $lib"
    usage
    exit -1
  fi
  sep=" "
done

if [ "$libs" == "" ]; then
  echo ""
  echo "Error: no libraries specified"
  usage
  exit -1
fi

for check in $libs; do
  basename=`basename $check`
  tochanges=`otool -L $app | grep -v : | tr '\11' ' ' | grep "[/ ]$basename" | cut -d' ' -f2`
  for tochange in $tochanges; do
    if [ "$tochange" != "$check" ]; then 
#      echo install_name_tool -change $tochange $check $app
      install_name_tool -change $tochange $check $app
    fi
  done
done

echo "Done"
