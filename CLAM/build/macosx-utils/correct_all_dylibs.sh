#!/bin/bash

sep=""
libs=""

usage()
{
  echo ""
  echo "Usage: $0 [library ...]"
  echo ""
  echo "  Modifies all dynamic library binding in the list of"
  echo "  libraries specified, so that they point to the matching"
  echo "  libraries found in the list of libraries specified"
  echo ""
}

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

for lib in $libs; do
  if [ -L $lib ]; then
    echo "  Skipping $lib: is a symbolic link"
  else
    echo "  Correcting library paths in $lib"
    for check in $libs; do
      basename=`basename $check`
      tochanges=`otool -L $lib | grep -v : | tr '\11' ' ' | grep "[/ ]$basename" | cut -d' ' -f2`
      for tochange in $tochanges; do
        if [ "$tochange" != "$check" ]; then 
          echo "    Changing binding of $tochange to $check in $lib"
          install_name_tool -change $tochange $check $lib
        fi
      done
    done
  fi
done

echo "Done"
