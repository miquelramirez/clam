INTRODUCTION

The "Analysis Synthesis Example" is an example, that features a full-fledged interface
entirely developed using CLAM available tools. You should not consider it a finished,
stable application, so expect some major changes in both the external appearance and 
the internal implementation.

Any comments about the graphical user interface or some missing functionality that could be
convenient for users in general, can be discussed  at the CLAM mailing list. Bug reporting 
should be done through the Mantis Bug Tracker web interface available from our web page.

INSTALLATION

The example requires requires the following external libraries: Xercesc 1.7.0, 
for the XML support, FLTK 1.0.11, for the graphical user interface, the fftw v.2.1.3 
library, and either Mesa (3.4.2 or higher) library or vendor specific OpenGL binaries, 
for the flashy graphics. You should have them installed on your system before trying 
to build the application from the library sources. 

The Analysis Synthesis Example, or "AnalysisSynthesis" for short, comes in two "flavors":
one features a graphical user interface and the other features a text-based (command-line) 
interface.

To build the AnalysisSynthesis application:

	 + Under "sane" GNU Linuxes:

	 Go to <clam sources dir>/unix_tools and issue the following sequence of commands
		$ autoconf
		$ ./configure
		
	 this will generate the makefiles for building the example. Note that you may change
	 building options such as including debugging information playing around with 
	 configure options. Execute 
		$ ./configure --help
	 to see the full list of possible options.

	 Once the configure finishes with generating the makefiles ( if it does not signal
	 some error before ) you should move into the example folder
		$ cd ../examples/AnalysisSynthesis

	 And issue 
		$ make depend
		$ make AnalysisSynthesisExampleGUI

	 for building the AnalysisSynthesis application with GUI, or

		$ make depend
		$ make AnalysisSynthesisExample

	 for building the console version.

	 Once the application is build you just issue 
		$ ./AnalysisSynthesisExampleGUI
	 or
		$ ./AnalysisSynthesisExample

     to execute it.


	 + Under Windows

	 at <clam sources dir>/visual_c++/AnalysisSynthesis folder you will find a Microsoft
	 Visual C++ 6.0 workspace (.dsw) already configured for building the application. Check
	 the Project->Settings->Link->Input options so they match your local library deployment.

	 By default the workspace will build the GUI-based AnalysisSynthesis application, to
	 make it build the console based interface go to menu Build->Set Active Configuration
	 and select the version you desire.

Happy compiling!

FIRST STEPS

Once you have built the Application you may follow the steps below to both get a first
impression of the application capabilities and check that everything is right:

1) Find the file named config.xml located at <clam sources dir>/examples/AnalysisSynthesis
   folder and edit the <InputSoundFile> </InputSoundFile> tag substituting the filename
   that appears there for a wave file of yours.

1) Go to Input menu and select the "Load Configuration..." option. Then a 
   file choosing dialog should pop up, letting you specify the "configuration"
   for the analysis and synthesis process. Look again for the config.xml file. Select
   the file and click "OK".

2) Once the configuration is loaded, you may take a look at the parameters involved
   selecting the "Input->Edit Configuration..." menu option. You may change the values
   that appear there as you wish. Press "Discard".

3) Select "Input->Display->Sound", and you should get a graphic displaying the waveform
   of the input file specified in config.xml

4) Select "Input->Play" to hear the file.

5) Select "Analysis->Analyze" and wait for the application to complete the analysis. This
   may take a while depending on the size of the input wave.

6) Select "Synthesis->Synthesis" to perform the SMS synthesis from the data uncovered by
   the previous analysis.

7) Select "Output->Display->Sinusoidal" and "Output->Display->Residual" for displaying both
   the sinusoidal and residual components of the input sound.

8) Select "Output->Play->Sinusoidal" and you will hear the sinusoidal component

9) Select "Output->Play->Residual" and you will hear the residual component

10) Finally select "Analysis->Store" so it stores the analysis result in .sdif format

11) Build the SDIFDisplay example ( examples/SDIFDisplay ) for seeing the resulting .sdif
    file

ANALYSIS/SYNTHESIS INPUTS AND OUTPUTS

The application has the follwing possible inputs:

1. A configuration xml file. This configuration file is used to configure both the 
   analysis and synthesis processes. 

2. An analysis xml file. This file will be the result of a previously performed 
   and stored analysis. The xml parser (xerces) features include a rather slow
   parsing/storing of documents. On the other hand, xml is a text-based format
   and thus you can expect a 1 MB sound file become 10 MB of xml analysis data.
   For all those reasons the storing/loading of analysis data, although fully working,
   is not recommended until those problems are solved.

3. An analysis SDIF file. This file will be the result of a previously performed
   and stored analysis. This format is less complete than the XML passivation, but
   it is dramatically more compact ( data is not stored in a "human readable" format ).

4. A Transformation score in xml format.

From these inputs, the application is able to generate the following outputs:

1. Analysis data xml file.

2. Melody xml file.

3. Output sound: global sound, sinusoidal component, residual component


CONFIGURATION FILE PARAMETERS DESCRIPTION:

The config file is the one that includes all parameters necessary for the 
SMS analysis/synthesis process. Use the sample given in /examples/analysissynthesis/config.xml
and leave those parameters you don't need to modify as they are.

Note: If you load a configuration with wrong parameters (for example with an input
soundfile that does not exist) the analysis process will remain disabled.

Here is the description of all the possible parameters:

	 + Global parameters:

	 <Name>: Particular name you want to give to your configuration file. Not used for
	 anything except the xml parsing.

	 <InputSoundFile>: path and name of the input sound file you want to analyze.

	 <OutputSoundFile>: path and name of where you want to have your output synthesized
	 sound file. Sinusoidal component will have a _sin.wav termination and the  residual
	 a _res.wav termination.

	 <OutputAnalysisFile>: path and name of where you want your output analysis data to
	 be stored.

	 <InputAnalysisFile>: path and name of where you want your input analysis data to
	 be loaded from. Not used in the GUI version as it is loaded from the dialog.

	 <SamplingRate>: global sampling rate


	 + Analysis parameters:

	 <AnalysisWindowSize>: window size in number of samples of the sinusoidal component. It 
	 must be odd.

	 <ResAnalysisWindowSize>: window size in number of samples of the residual component.It 
	 must be odd.

	 <AnalysisWindowType>: type of window used for the sinusoidal analysis. Available: Hamming,
	 Triangular, BlackmannHarris62, BlackmannHarris70, BlackmannHarris74, BlackmannHarris92, 
	 KaisserBessel17, KaisserBessel18, KaisserBessel19, KaisserBessel20, KaisserBessel25,
	 KaisserBessel30, KaisserBessel35. 

	 <ResAnalysisWindowType>: type of window used for the residual analysis. Available: Same as 
	 in sinusoidal. HIGHLY RECOMMENDED: as sinusoidal spectrum is synthesized using the transform 
	 of the BlackmannHarris 92dB, it is necessary to use that window in the analysis of the residual 
	 component.

	 <AnalysisHopSize>: hop size in number of samples. It is the same both for the sinusoidal
	 and residual component. If -1, it is taken as half the residual window size (recommended)

	 <AnalysisZeroPaddingFactor> Zero padding factor applied to both components. 0 means that zeros
	 will be added to the input audio frame till it reaches the next power of two, 1 means that zeros
	 will be added to the next power of two etc... 

	 <AnalysisPeakDetectMagThreshold>: magnitude threshold in dB's in order to say that a given
	 peak is valid or not. Recommended: -80.

	 <AnalysisMaxSines>: number of sinusoidals to be used in the tracking process. Recommended: 50.

	 <AnalysisSinTrackingFreqDeviation>: maximum deviation in herz for a sinusoidal track.

	 <AnalyisReferenceFundFreq>: in herz, reference fundamental.

	 <AnalyisLowestFundFreq>: in herz, lowest fundamental frequency to be acknowledged.

	 <AnalyisHighestFundFreq>: in herz, highest fundamental frequency to be acknowledged.

	 <AnalyisMaxFundFreqError>: maximum error in herz for the fundamental detection algorithm.

	 <AnalyisMaxFundCandidates>: maximum number of candidate frequencies for the fundamental detection 
	 algorithm.

	 <AnalysisHarmonic>: if 1, harmonic analysis is performed on all segments that have
	 a valid pitch. In those segments the track number assigned to each peak corresponds to
	 the harmonic number. On unvoiced segments, inharmonic analysis is still performed.
	 Use if the type of transformation you want to perform depends on this harmonic characteristic.
	 It is highly recommended that if you use harmonic analysis you set a very low threshold
	 for <AnalysisPeakDetectMagThreshold> (i.e. -120).

	 + Synthesis Parameters:

	 <SynthesisFrameSize>: in number of samples, size of the synthesis frame. If set to -1 it is 
	 (ResAnalysisWindowSize-1)/2. If any other number is used you are bound to get synthesis artifacts.

	 <SynthesisWindowType>: type of window used for the residual analysis. Available: Same as in sinusoidal.

	 <SynthesisHopSize>: synthesis hop size in number of samples. If -1 it is half the synthesis frame 
	 size (recommended)

	 <SynthesisZeroPaddingFactor> zero padding factor for the ifft. Must be the same as the one used
	 in the analysis.

	 <SynthesisPhaseManagementType>: type of phase management to be applied to the sinusoidal synthesis.
	 Available: Continuation, Align, Random. Recommended: Continuation.


TRANSFORMATION SCORES:

The transformation score is, for the time being, a very simple one.

It has two possible fields FAmount and BPFAmount:

   + FAmount is a floating point value that is given as initial value and will remain 
     constant if no BPFAmount is given.

   + BPFAmount: is a break point function value. The way this envelope is applied to a
     particular transformation will depend on the type of transformation and the way it
	 is supposed to react. In src/Processing/Transformation/SMSFreqShift.cxx, you will see that the
	 envelope is used as a temporal envelope for the amount of frequency shift to be applied
	 and the breakpoint function's x axis must be defined between 0 and 1.

WRITING YOUR OWN TRANSFORMATION SCORE:

If you want to write your own transformation, follow the following steps:

1. Look at the SMSFreqShift.hxx and .cxx files. It is a sample transformation that is
used as default. Write your own MyTransformation.hxx and MyTransformation.cxx files
following the same structure. Note that you only have to add code to any of the possible
overloads of the Do(in, out) method. Accepted overloads are for (audio,audio), 
(spectrum,spectrum) (spectralpeakarray,spectralpeakarray), (frame,frame) and (segment,segment).

So, if MyTransformation only gets every audioSample and divides it by two, I should
only have to write the following overload:

bool MyTransformation::Do(const Audio& in, Audio& out)
{
	TSize size=in.GetSize()
	DataArray& inBuffer=in.GetBuffer();
	DataArray& outBuffer=out.GetBuffer();
	for(int i=0;i<size;i++)
		outBuffer[i]=inBuffer[i]/2;
	return true;
}

2. Then go to the AnalysisSynthesisExampleGUI.cxx file and make the necessary include
(in line 11).
	ex. #include "MyTransformation.hxx"

3. Finally, in the same file, go to line 477 and modify it to instantiate your particular
transformation.
	ex. MyTransformation myTrans;

recompile AnalysisSynthesis and that's all!

Note: this feature is not implemented in the command line version at the time of writing 
this but will soon be.


MELODY ANALYSIS:

The melody analysis is at a *VERY EXPERIMENTAL* stage and has just been kept for showing
future capabilities of the application.

The analysis of a melody gives an MPEG-7 like xml output. It will only work on
monophonic single instrument phrases. Currently it only gives acceptable results for
sax phrases (this problem is mainly due to the missing features of the segmentation
algorithm which we are now working on).

No melody analysis can be performed if you load previously analyzed xml data. The analysis
must be done in the application.


The CLAM development team
clam@iua.upf.es