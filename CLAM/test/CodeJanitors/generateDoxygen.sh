#!/bin/bash

cd CajitasDeArena/CLAM-new-devel
doxygen .doxygen.cfg
slogin clamadm@www.iua.upf.es rm -rf doxygen/devel
scp -r ~/public_html/CLAM-devel clamadm@www.iua.upf.es:doxygen/devel


