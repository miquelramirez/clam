#! /bin/sh
echo About to upload : 
ls -lh out/*.tar.gz
scp out/*.tar.gz clamadm@www.iua.upf.es:download/
