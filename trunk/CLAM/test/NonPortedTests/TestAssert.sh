#!/bin/bash

for cocksure in "" -DCLAM_DISABLE_CHECKS; do
	for debug in "" -DDEBUG; do
		for userelease in "" -DCLAM_USE_RELEASE_ASSERTS; do
			echo "-------------- Active flags: $debug $userelease $cocksure"
			g++ -g $debug $userelease $cocksure CLAMAssert.cxx TestAssert.cxx
			./a.out
		done
	done
done
