#!/bin/bash

VERSION="0.8.0"
#WARNING: UNDER OSX, PREFIX /usr/ DOESN'T WORK PROPERLY
PREFIX=/usr/local

mkdir $PREFIX/include/CLAM
cp include/CLAM/* $PREFIX/include/CLAM
cp lib/lib* $PREFIX/lib
cp lib/pkgconfig/* $PREFIX/lib/pkgconfig

cd $PREFIX/lib

ln -s libclam_core.$VERSION.dylib libclam_core.dylib
ln -s libclam_processing.$VERSION.dylib libclam_processing.dylib
ln -s libclam_vmqt.$VERSION.dylib libclam_audioio.dylib
ln -s libclam_audioio.$VERSION.dylib libclam_vmqt.dylib

chown -R root $PREFIX/include/CLAM
chown root $PREFIX/lib/libclam*
chown root $PREFIX/lib/pkgconfig/clam*

echo "DONE!"
