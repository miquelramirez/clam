#!/bin/bash

help2man -N ./clamrefactor.py -n "Modifies CLAM networks in batch" | sed 's/clamrefactor.py/clamrefactor/' > clamrefactor.1
help2man -N ./clam_lv2_generator.py -n "Generates files for CLAM based LV2 plugins" | sed 's/clam_lv2_generator.py/clam_lv2_generator/' > clam_lv2_generator.1
help2man -N ./clamnetwork2code.py -n "Generates C++ to build a CLAM network" | sed 's/clamnetwork2code.py/clamnetwork2code/' > clamnetwork2code.1

