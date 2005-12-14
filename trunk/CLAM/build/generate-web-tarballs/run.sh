#! /bin/sh
makeSnapshot=./makeSnapshot.sh 

#####  CVS-module			CVS-tag         Filename    			tar 	zip limit-in-k  needs-clam-version

# CVS snapshots
$makeSnapshot CLAM_NetworkEditor 	HEAD		CLAM-NetworkEditor-cvs		1	0 300 CLAM-cvs
$makeSnapshot CLAM_Annotator		HEAD		CLAM-Annotator-cvs		1	0 300 CLAM-cvs
$makeSnapshot CLAM_Voice2MIDI		HEAD		CLAM-Voice2MIDI-cvs		1	0 300 CLAM-cvs
$makeSnapshot CLAM_SMSTools		HEAD		CLAM-SMSTools-cvs		1	0 300 CLAM-cvs
$makeSnapshot CLAM			HEAD		CLAM-cvs			1	0 300 CLAM-cvs

# Release 0.9.0-pre2
$makeSnapshot CLAM_NetworkEditor 	HEAD		CLAM-NetworkEditor-0.90-pre2	1	0 300 CLAM-0.90-pre2
$makeSnapshot CLAM_Annotator		HEAD		CLAM-Annotator-0.2-pre2		1	0 300 CLAM-0.90-pre2
$makeSnapshot CLAM_Voice2MIDI		HEAD		CLAM-Voice2MIDI-0.3-pre2	1	0 300 CLAM-0.90-pre2
$makeSnapshot CLAM_SMSTools		HEAD		CLAM-SMSTools-0.4-pre2		1	0 300 CLAM-0.90-pre2
$makeSnapshot CLAM			HEAD		CLAM-0.90-pre2			1	0 300 CLAM-0.90-pre2
#$makeSnapshot CLAM-TestData		HEAD		CLAM-TestData			1	1 

exit

# Release 0.8.0
$makeSnapshot CLAM_NetworkEditor 	tests-passed-on-linux-rel				NetworkEditor-0.2	1 1 300 CLAM-0.8.0
$makeSnapshot CLAM_Annotator		HEAD				Annotator-0.2		1 1 300 CLAM-0.8.0
$makeSnapshot CLAM_Voice2MIDI		HEAD				Voice2MIDI-0.2		1 1 300 CLAM-0.8.0
$makeSnapshot CLAM_SMSTools		tests-passed-on-linux-rel	SMSTools2-0.3		1 1 300 CLAM-0.8.0
#$makeSnapshot CLAM-TestData		HEAD				CLAM-TestData		1 1 
$makeSnapshot CLAM			tests-passed-on-linux-rel	CLAM-0.8.0		1 1 300

exit
$makeSnapshot CLAM_Voice2MIDI		stable-rel	Voice2MIDI-0.1		1 1 300 CLAM-0.7.0

exit

$makeSnapshot CLAM_SMSTools		stable-rel	SMSTools2-0.2		1 1 300 CLAM-0.7.0

$makeSnapshot CLAM			CLAM-0_7_0-rel	CLAM-0.7.0		1 1 300

$makeSnapshot CLAM-TestData		CLAM-0_7_0-rel	CLAM-TestData-0.7.0	1 1 300

$makeSnapshot CLAM_NetworkEditor	stable-rel 	NetworkEditor-0.1	1 1 300 CLAM-0.7.0


$makeSnapshot CLAM_SDIFDisplay		stable-rel	SDIFDisplay-0.1		1 1 300 CLAM-0.7.0

exit

#####  CVS-module                     CVS-tag        DIR        Filename                    tar zip limit-in-k

$makeSnapshot fltk-1.1.4-VC                  HEAD           fltk        fltk-1.1.4-VC-build3        0   1

$makeSnapshot portmidi			     HEAD	    portmidi    portmidi		   0   1

$makeSnapshot fftw-2.1.3-VC-float            HEAD           fftw-float  fftw-2.1.3-VC-float-build2         0   1
$makeSnapshot fftw-2.1.3-VC-double           HEAD           fftw-double fftw-2.1.3-VC-double-build2        0   1
$makeSnapshot id3lib                         HEAD           id3lib      id3lib                     0   1
$makeSnapshot oggvorbis                      HEAD           oggvorbis   oggvorbis                  0   1
$makeSnapshot libsndfile                     HEAD           libsndfile  libsndfile-1.0.6-VC        0   1
$makeSnapshot libmad                         HEAD           libmad      libmad                     0   1


$makeSnapshot xercesc-2.3.0-VC6               HEAD           xercesc     xercesc-1.3.0-VC6-build1            0   1

 $makeSnapshot CLAM-TestData	 	      HEAD           CLAM-TestData     CLAM-TestData        1   1

$makeSnapshot libsndfile                     HEAD           libsndfile  libsndfile                 0   1
 
 $makeSnapshot fltk-1.1.4-g++2.95.4-debian HEAD           fltk        fltk-1.1.4-debian_woody-build2  1   0
exit
 $makeSnapshot compiling_against_CLAM_example HEAD           compiling_against_CLAM_example compiling_against_CLAM_example           1   1 
 


 $makeSnapshot fltk-1.1.3-redhat9             HEAD           fltk        fltk-1.1.3-redhat9          1   0   
 $makeSnapshot fftw-2.1.5-redhat9             HEAD           fftw        fftw-2.1.5-redhat9          1   0   
 $makeSnapshot xercesc-1.7.0-g++2.95.4-debian HEAD           xercesc     xercesc-1.7.0-debian_woody  1   0
 $makeSnapshot xercesc-1.7.0-redhat9          HEAD           xercesc     xercesc-1.7.0-redhat9       1   0
 
 
