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

<!-- The following is a transformation score for the SMS Analysis Synthesis Application. 
You may find an explanation of the AnalysisSynthesis scheme and most of these transformations 
in chapter 10 of Udo Zoelzer's (ed.) DAFX. Digital Audio Effects (Wiley 2002). Of course you 
can also access the source code of the example and the comments therein.

This configuration file is basically a composition of concrete configurations that affect 
specific transformations. You may change the order, activate/deactivate or configure any 
of them. The way the configuration affects a concrete transformation may vary, you should
read the comments before each transformation for learning the particularities of every transformation.

All transformations share some common fields. First they need to define the kind of concrete 
transformation in the ConcreteClassName attribute. Then you need to specify the values for the 
transformation. This can usually be done in two different ways: using a single-value Amount 
attribute or a breakpoint function BPFAmount attribute in which you declare a function as a 
set of points and an interpolation type (note: this function may represent a time envelope or 
a spectral envelope depending on the transformation).

In any transformation chain there need to be two special transformations called SMSTransformationChainIO 
as first and last transformation to the chain. These transformations act as input and output, 
are mandatory but do not affect the result.

After the particular transformations there is an 'OnArray' where you have the same number 
of 1's or 0's as transformations in the Chain. 1 means active and 0 means bypassed 
(SMSTransformationChainIO's are not affected by this).-->

Example with commented elements:

<SMSTransformationChainConfig>
	<Configurations>
		<!-- Input to the transformation chain. Must always be present. -->
		<Config>
			<ConcreteClassName>SMSTransformationChainIO</ConcreteClassName>
		</Config>
		<!-- Pitch shift with timbre preservation. You can define a single-value amount or a 
		time envelope. 1 means no change and 0.5 means multiplying current pitch by 0.5-->
		<Config>
			<ConcreteClassName>SMSPitchShift</ConcreteClassName>
			<BPFAmount>
				<Interpolation>Linear</Interpolation>
				<Points>{0 1.5} {1 1.5}</Points>
			</BPFAmount>
		</Config>
		<!-- Gender change. If amount is 0 it means from male to female. If it is 1 it means 
		from female to male. -->
		<Config>
			<ConcreteClassName>SMSGenderChange</ConcreteClassName>
			<Amount>0</Amount>
		</Config>
		<!-- Pitch discretization to temparate scale. If active it rounds the pitch to nearest 
		note according to temperate musical scale.-->
		<Config>
			<ConcreteClassName>SMSPitchDiscretization</ConcreteClassName>
		</Config>
		<!-- Gain applied to odd harmonics in relation to even harmonic. E.g. A value of 6 means 
		that a 6dB difference will be introduced, thus, odd harmonics will be 3dB higher and even 
		harmonic 3dB lower.-->
		<Config>
			<ConcreteClassName>SMSOddEvenHarmonicRatio</ConcreteClassName>
			<Amount>12</Amount>
		</Config>
		<!-- Frequency shift applied to all partials expressed in Hz-->
		<Config>
			<ConcreteClassName>SMSFreqShift</ConcreteClassName>
			<Amount>100</Amount>
		</Config>
		<!-- Filter expressed in bpf format applied to only the sinusoidal components-->
		<Config>
			<ConcreteClassName>SMSSineFilter</ConcreteClassName>
			<BPFAmount>
				<Interpolation>Step</Interpolation>
				<Points>{0 6} {1 0} {2 0} {100 0} </Points>
			</BPFAmount>
		</Config>
		<!-- Gain in dB's applied to the residual component-->
		<Config>
			<ConcreteClassName>SMSResidualGain</ConcreteClassName>
			<Amount>6</Amount>
		</Config>
		<!-- Gain in dB's applied to the sinusoidal component-->
		<Config>
			<ConcreteClassName>SMSSinusoidalGain</ConcreteClassName>
			<Amount>3</Amount>
		</Config>
		<!-- Harmonizer. Each point defines a new voice added to the harmonization. The X value is the 
		gain in relation to the original one and the Y value the pitch transposition factor. -->
		<Config>
			<ConcreteClassName>SMSHarmonizer</ConcreteClassName>
			<BPFAmount>
				<Interpolation>Step</Interpolation>
				<Points>{-3 1.3} {-3 1.5} {-3 1.7}</Points>
			</BPFAmount>
		</Config>
		<!-- Frequency shift applied to the sinusoidal spectral shape expressed in Hz.-->
		<Config>
			<ConcreteClassName>SMSSpectralShapeShift</ConcreteClassName>
			<Amount>50</Amount>
		</Config>
		<!-- Morphing between two different sounds. SMSMorph has many different parameters to configure 
		some of which are not even implemented. But the basic ones (and already tested) are:
		- <HypBPB>: BPF (envelope-like) Parameter. Defines how much of each sound is being used from 0 to 1 
		- <SynchronizeTime>: BPF (envelope-like) Parameter. Defines temporal relation between input sound and sound
			to hybridize 
		- <HybPitch: BPF (envelope-like) Parameter. Pitch to use: 0 input, 1 sound to hybridize-->
		<Config>
			<ConcreteClassName>SMSMorph</ConcreteClassName>
			<HybBPF>
				<Interpolation>Linear</Interpolation>
				<Points>{0 0} {1 1}</Points>
			</HybBPF>
		</Config>
		<!-- Output to the transformation chain. Must always be present. -->
		<Config>
			<ConcreteClassName>SMSTransformationChainIO</ConcreteClassName>
		</Config>
	</Configurations>
	<!-- Array defining what transformations are active(1) or bypassed(0). WARNING: This array always has to 
	be the same size as the number of previous transformations available.-->
	<OnArray>1 0 0 0 0 0 0 0 0 0 0 1 1</OnArray>
</SMSTransformationChainConfig>


/*************************** Writing your own transformation ************************************/

MISSING, COMING SOON.


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