#!/bin/bash
grep PROGRAM `find * -name settings.cfg` | sed -e 's:/.*= :/:' | sed -e 's:^:cp ../CLAM_Skeleton_project.dsp :' | sed -e 's:$:.dsp:' > foo

for a in `find -name *.dsp`; do echo $a ; (cd `dirname $a`; ../../../srcdeps/srcdeps.exe settings.cfg `basename $a`); done