SMS is a CLAM usage example. As a final application it is still in an
alpha stage, so all bug-reports or comments will be welcomed at the CLAM mailing-list.

The Analysis application has to versions, one of which has a FLTK graphical user
interface and the other being command-line based. All you have to do to compile one
or the other is to compile the file SMSTools.cxx (if you want GUI) or 
SMS.cxx (if you don't).

The only externals you need for compiling the SMS app are Xerces, 
FLTK (in the case you work with the GUI version, and fftw. All of them should be
correctly installed in your system by the time you read this.

The application has different possible inputs:

1. A configuration xml file. This configuration file is used to configure both the 
	analysis and synthesis processes. (1)
2. An analysis xml file. This file will be the result of a previously performed 
	and stored analysis. The xml parser (xerces) features include a rather slow
	parsing/storing of documents. On the other hand, xml is a text-based format
	and thus you can expect a 1 MB sound file become 10 MB of xml analysis data.
	For all those reasons the storing/loading of analysis data, although fully working,
	is not recommended until those problems are solved.
3. A Transformation score in xml format (2).

From these inputs, the application is able to generate the following outputs:

1. Analysis data xml file.
2. Melody xml file (3).
3. Output sound: global sound, sinusoidal component, residual component

Notes:

/*************************************************************************************/
/*************************************************************************************/
/******************************** CONFIGURATION **************************************/
/*************************************************************************************/
/*************************************************************************************/


(1) The config file is the one that includes all parameters necessary for the 
SMS analysis/synthesis process. Use the sample given in /examples/SMS/config.xml
and leave those parameters you don't need to modify as they are.

Note: If you load a configuration with wrong parameters (for example with an input
soundfile that does not exist) the analysis process will remain disabled.

Here is the description of all the possible parameters:

/****************************** Global parameters ************************************/

<Name>: Particular name you want to give to your configuration file. Not used for
anything except the xml parsing.

<InputSoundFile>: path and name of the input sound file you want to analyze.

<OutputSoundFile>: path and name of where you want to have your output synthesized
sound file. Sinusoidal component will have a _sin.wav termination and the  residual
a _res.wav termination.

<OutputXMLFile>: path and name of where you want your output analysis data to
be stored.

<InputXMLFile>: path and name of where you want your input analysis data to
be loaded from. Not used in the GUI version as it is loaded from the dialog.

<SamplingRate>: global sampling rate


/*************************** Analysis parameters ************************************/

<AnalysisWindowSize>: window size in number of samples of the sinusoidal component. It must be odd.

<ResAnalysisWindowSize>: window size in number of samples of the residual component.It must be odd.

<AnalysisWindowType>: type of window used for the sinusoidal analysis. Available: Hamming,
Triangular, BlackmannHarris62, BlackmannHarris70, BlackmannHarris74, BlackmannHarris92, 
KaisserBessel17, KaisserBessel18, KaisserBessel19, KaisserBessel20, KaisserBessel25,KaisserBessel30, 
KaisserBessel35. 

<ResAnalysisWindowType>: type of window used for the residual analysis. Available: Same as in sinusoidal.
Recommended: as sinusoidal spectrum is synthesized using the transform of the BlackmannHarris 92dB, it
is necessary to use that window in the analysis of the residual component.

<AnalysisHopSize>: hop size in number of samples. It is the same both for the sinusoidal
and residual component. If -1, it is taken as half the residual window size -1 (recommended)

<AnalysisZeroPaddingFactor> Zero padding factor applied to both components. 0 means that zeros will be added
to the input audio frame till it reaches the next power of two, 1 means that zeros will be added to
the next power of two etc... 

<AnalysisPeakDetectMagThreshold>: magnitude threshold in dB's in order to say that a given
peak is valid or not. Recommended: -80.

<AnalysisMaxSines>: number of sinusoidals to be used in the tracking process. Recommended: 50.

<AnalysisSinTrackingFreqDeviation>: maximum deviation in herz for a sinusoidal track.

<AnalyisReferenceFundFreq>: in herz, reference fundamental.

<AnalyisLowestFundFreq>: in herz, lowest fundamental frequency to be acknowledged.

<AnalyisHighestFundFreq>: in herz, highest fundamental frequency to be acknowledged.

<AnalyisMaxFundFreqError>: maximum error in herz for the fundamental detection algorithm.

<AnalyisMaxFundCandidates>: maximum number of candidate frequencies for the fundamental detection algorithm.

<AnalysisHarmonic>: if 1, harmonic analysis is performed on all segments that have
a valid pitch. In those segments the track number assigned to each peak corresponds to
the harmonic number. On unvoiced segments, inharmonic analysis is still performed.
Use if the type of transformation you want to perform depends on this harmonic characteristic.
It is highly recommended that if you use harmonic analysis you set a very low threshold
for <AnalysisPeakDetectMagThreshold> (i.e. -120).

/*************************** Synthesis parameters ************************************/

<SynthesisFrameSize>: in number of samples, size of the synthesis frame. If set to -1 it is 
(ResAnalysisWindowSize-1)/2. If any other number is used you are bound to get synthesis artifacts.

<SynthesisWindowType>: type of window used for the residual analysis. Available: Same as in sinusoidal.

<SynthesisPhaseManagementType>: type of phase management to be applied to the sinusoidal synthesis.
Available: Continuation, Align, Random. Recommended: Continuation.


/*************************************************************************************/
/*************************************************************************************/
/******************************* TRANSFORMATION **************************************/
/*************************************************************************************/
/*************************************************************************************/

(2) The transformation score is, for the time being, a very simple one.

It has two possible fields FAmount and BPFAmount.

FAmount: is a floating point value that is given as initial value and will remain 
constant if no BPFAmount is given.

BPFAmount: is a break point function value. The way this envelope is applied to a
particular transformation will depend on the type of transformation and the way it
is supposed to react. In the SMSFreqShift.cxx for example, you will see that the
envelope is used as a temporal envelope for the amount of frequency shift to be applied
and the breakpoint function's x axis must be defined between 0 and 1.

/*************************************************************************************/
/*************************************************************************************/
/********************** WRITING YOUR OWN TRANSFORMATION ******************************/
/*************************************************************************************/
/*************************************************************************************/
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

2. Then go to the SMSBase.cxx file and make the necessary include

	ex. #include "MyTransformation.hxx"

3. Finally, in the same file, find the Transform() method and make a new of your particular
transformation.
	ex. SetTransformation(new MyTransformation ());

and that's all!

Note: this feature is not implemented in the command line version at this writting but
will soon be.


/*************************************************************************************/
/*************************************************************************************/
/****************************** MELODY ANALYSIS **************************************/
/*************************************************************************************/
/*************************************************************************************/

(3) The melody analysis is at a very experimental stage and has just been kept for showing
future capabilities of the application.

The analysis of a melody gives an MPEG-7 like xml output. It will only work on
monophonic single instrument phrases. Currently it only gives acceptable results for
sax phrases (this problem is mainly due to the missing features of the segmentation
algorithm which we are now working on).

No melody analysis can be performed if you load previously analyzed xml data. The analysis
must be done in the application.