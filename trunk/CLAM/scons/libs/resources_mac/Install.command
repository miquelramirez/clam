#!/bin/bash

#WARNING: UNDER OSX, PREFIX /usr/ DOESN'T WORK PROPERLY
PREFIX=/usr/local

sudo tar jxfv headers.tbz2 -C $PREFIX/include
sudo tar jxfv libraries.tbz2 -C $PREFIX/lib
