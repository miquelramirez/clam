# Microsoft Developer Studio Project File - Name="AllApplicationTests" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=AllApplicationTests - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "AllApplicationTests.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "AllApplicationTests.mak" CFG="AllApplicationTests - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "AllApplicationTests - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "AllApplicationTests - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "empty"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "AllApplicationTests - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../ReleaseFiles/bin"
# PROP Intermediate_Dir "../ReleaseFiles/obj"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GR /GX /O2 /FD /c /Zm1000 /D "NDEBUG" /D "HAVE_STANDARD_SSTREAM" /D "HAVE_STANDARD_SSTREAM_STR" /D "HAVE_STANDARD_VECTOR_AT" /D "HAVE_STANDARD_UNICODE" /D "WIN32" /D "_MBCS" /D "_CONSOLE" /D "CLAM_FLOAT" /D "CLAM_USE_XML" /D "ID3LIB_LINKOPTION=1" /I "..\..\..\..\src\Defines" /FI"preinclude.hxx" /I "..\..\..\..\test\UnitTests" /I "..\..\..\..\examples" /I "..\..\..\..\..\xercesc\include" /I "..\..\..\..\..\fftw\include" /I "..\..\..\..\..\pthreads\include" /I "..\..\..\..\..\cppunit\include" /I "..\..\..\..\..\libsndfile\include" /I "..\..\..\..\..\oggvorbis\include" /I "..\..\..\..\..\libmad\include" /I "..\..\..\..\..\id3lib\include" /I "..\..\..\..\examples\SMS" /I "..\..\..\..\src\Base" /I "..\..\..\..\src\Data\Base" /I "..\..\..\..\src\Data\BasicProcessing" /I "..\..\..\..\src\Data\Descriptors" /I "..\..\..\..\src\Data\Descriptors\Melody" /I "..\..\..\..\src\Defines" /I "..\..\..\..\src\Defines\Windows" /I "..\..\..\..\src\Errors" /I "..\..\..\..\src\Flow\Controls" /I "..\..\..\..\src\Flow\Nodes" /I "..\..\..\..\src\Flow\Ports" /I "..\..\..\..\src\Processing\Analysis" /I "..\..\..\..\src\Processing\ArithOps" /I "..\..\..\..\src\Processing\AudioIO" /I "..\..\..\..\src\Processing\Base" /I "..\..\..\..\src\Processing\SDIFIO" /I "..\..\..\..\src\Processing\Synthesis" /I "..\..\..\..\src\Processing\Transformations" /I "..\..\..\..\src\Processing\Transformations\SMS" /I "..\..\..\..\src\Processing\AudioFileIO" /I "..\..\..\..\src\Standard" /I "..\..\..\..\src\Storage\Base" /I "..\..\..\..\src\Storage\XML" /I "..\..\..\..\src\System" /I "..\..\..\..\src\System\Threads" /I "..\..\..\..\src\Tools\AudioFileIO" /I "..\..\..\..\src\Tools\AudioIO" /I "..\..\..\..\src\Tools\SDIF" /I "..\..\..\..\externals" /I "..\..\..\..\externals\numrec"
# ADD BASE RSC /l 0xc0a /d "NDEBUG"
# ADD RSC /l 0xc0a /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32
# ADD LINK32 /libpath:"..\..\..\..\..\fftw\lib" /libpath:"..\..\..\..\..\xercesc\lib" /libpath:"..\..\..\..\..\pthreads\lib" /libpath:"..\..\..\..\..\dxsdk\lib" /libpath:"..\..\..\..\..\cppunit\lib" /libpath:"..\..\..\..\..\libsndfile\lib" /libpath:"..\..\..\..\..\oggvorbis\lib" /libpath:"..\..\..\..\..\libmad\lib" /libpath:"..\..\..\..\..\id3lib\lib" RFFTW2st.lib FFTW2st.lib xerces-c_2.lib dsound.lib dxerr8.lib user32.lib gdi32.lib pthreadVC.lib cppunit_vc6.lib libsndfile.lib ogg_static.lib vorbis_static.lib vorbisenc_static.lib vorbisfile_static.lib id3lib.lib libmad.lib /nologo /subsystem:console /NODEFAULTLIB:MSVCRTD /machine:I386
# SUBTRACT LINK32 /nologo /verbose /pdb:none /debug

!ELSEIF  "$(CFG)" == "AllApplicationTests - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../DebugFiles/bin"
# PROP Intermediate_Dir "../DebugFiles/obj"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /GR /GX /ZI /Od /D "_DEBUG" /FD /GZ /Zm1000 /c /D "HAVE_STANDARD_SSTREAM" /D "HAVE_STANDARD_SSTREAM_STR" /D "HAVE_STANDARD_VECTOR_AT" /D "HAVE_STANDARD_UNICODE" /D "WIN32" /D "_MBCS" /D "_CONSOLE" /D "CLAM_FLOAT" /D "CLAM_USE_XML" /D "ID3LIB_LINKOPTION=1" /I "..\..\..\..\src\Defines" /FI"preinclude.hxx" /I "..\..\..\..\test\UnitTests" /I "..\..\..\..\examples" /I "..\..\..\..\..\xercesc\include" /I "..\..\..\..\..\fftw\include" /I "..\..\..\..\..\pthreads\include" /I "..\..\..\..\..\cppunit\include" /I "..\..\..\..\..\libsndfile\include" /I "..\..\..\..\..\oggvorbis\include" /I "..\..\..\..\..\libmad\include" /I "..\..\..\..\..\id3lib\include" /I "..\..\..\..\examples\SMS" /I "..\..\..\..\src\Base" /I "..\..\..\..\src\Data\Base" /I "..\..\..\..\src\Data\BasicProcessing" /I "..\..\..\..\src\Data\Descriptors" /I "..\..\..\..\src\Data\Descriptors\Melody" /I "..\..\..\..\src\Defines" /I "..\..\..\..\src\Defines\Windows" /I "..\..\..\..\src\Errors" /I "..\..\..\..\src\Flow\Controls" /I "..\..\..\..\src\Flow\Nodes" /I "..\..\..\..\src\Flow\Ports" /I "..\..\..\..\src\Processing\Analysis" /I "..\..\..\..\src\Processing\ArithOps" /I "..\..\..\..\src\Processing\AudioIO" /I "..\..\..\..\src\Processing\Base" /I "..\..\..\..\src\Processing\SDIFIO" /I "..\..\..\..\src\Processing\Synthesis" /I "..\..\..\..\src\Processing\Transformations" /I "..\..\..\..\src\Processing\Transformations\SMS" /I "..\..\..\..\src\Processing\AudioFileIO" /I "..\..\..\..\src\Standard" /I "..\..\..\..\src\Storage\Base" /I "..\..\..\..\src\Storage\XML" /I "..\..\..\..\src\System" /I "..\..\..\..\src\System\Threads" /I "..\..\..\..\src\Tools\AudioFileIO" /I "..\..\..\..\src\Tools\AudioIO" /I "..\..\..\..\src\Tools\SDIF" /I "..\..\..\..\externals" /I "..\..\..\..\externals\numrec"
# ADD BASE RSC /l 0xc0a /d "_DEBUG"
# ADD RSC /l 0xc0a
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32
# ADD LINK32 /libpath:"..\..\..\..\..\fftw\lib" /libpath:"..\..\..\..\..\xercesc\lib" /libpath:"..\..\..\..\..\pthreads\lib" /libpath:"..\..\..\..\..\dxsdk\lib" /libpath:"..\..\..\..\..\cppunit\lib" /libpath:"..\..\..\..\..\libsndfile\lib" /libpath:"..\..\..\..\..\oggvorbis\lib" /libpath:"..\..\..\..\..\libmad\lib" /libpath:"..\..\..\..\..\id3lib\lib" FFTW2std.lib RFFTW2std.lib xerces-c_2d.lib dsound.lib dxerr8.lib user32.lib gdi32.lib pthreadVC.lib cppunitd_vc6.lib libsndfiled.lib ogg_static_d.lib vorbis_static_d.lib vorbisenc_static_d.lib vorbisfile_static_d.lib id3libd.lib libmadd.lib /nologo /subsystem:console /NODEFAULTLIB:MSVCRT /debug /machine:I386
# SUBTRACT LINK32 /nologo /verbose /pdb:none

!ENDIF 

# Begin Target

# Name "AllApplicationTests - Win32 Release"
# Name "AllApplicationTests - Win32 Debug"
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Group "Source Files"

# Begin Group "test Sources"

# Begin Group "UnitTests Sources"

# Begin Source File

SOURCE=..\..\..\..\test\UnitTests\TestRunnerConsole.cxx
# End Source File
# End Group
# Begin Group "FunctionalTests Sources"

# Begin Group "Applications Sources"

# Begin Group "SMSTests Sources"

# Begin Source File

SOURCE=..\..\..\..\test\FunctionalTests\Applications\SMSTests\SMSExampleTest.cxx
# End Source File
# End Group
# End Group
# Begin Group "ProcessingTests Sources"

# Begin Source File

SOURCE=..\..\..\..\test\FunctionalTests\ProcessingTests\TestFFT.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\test\FunctionalTests\ProcessingTests\TestIFFT.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\test\FunctionalTests\ProcessingTests\TestSpectralPeakDetect.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\test\FunctionalTests\ProcessingTests\TestMelFilterBank.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\test\FunctionalTests\ProcessingTests\TestCepstralTransform.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\test\FunctionalTests\ProcessingTests\TestMultiChannelAudioFileWriter.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\test\FunctionalTests\ProcessingTests\TestMonoAudioFileReader.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\test\FunctionalTests\ProcessingTests\TestMultiChannelAudioFileReader.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\test\FunctionalTests\ProcessingTests\TestMonoAudioFileWriter.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\test\FunctionalTests\ProcessingTests\similarityHelper.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\test\FunctionalTests\ProcessingTests\AudioFileOpsTestsHelper.cxx
# End Source File
# End Group
# End Group
# End Group
# Begin Group "Errors Sources"

# Begin Source File

SOURCE=..\..\..\..\src\Errors\Err.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Errors\ErrOutOfMemory.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Errors\ErrProcessingObj.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Errors\ErrSoundFileIO.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Errors\ErrDynamicType.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Errors\ErrOpenFile.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Errors\ErrSystem.cxx
# End Source File
# End Group
# Begin Group "Defines Sources"

# Begin Source File

SOURCE=..\..\..\..\src\Defines\Assert.cxx
# End Source File
# End Group
# Begin Group "Standard Sources"

# Begin Source File

SOURCE=..\..\..\..\src\Standard\Enum.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Standard\Complex.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Standard\Polar.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Standard\GlobalEnums.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Standard\Point.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Standard\BPF.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Standard\Text.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Standard\Order.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Standard\Flags.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Standard\Matrix.cxx
# End Source File
# End Group
# Begin Group "Storage Sources"

# Begin Group "XML Sources"

# Begin Source File

SOURCE=..\..\..\..\src\Storage\XML\XMLStorage.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Storage\XML\BasicXMLable.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Storage\XML\XMLAdapter.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Storage\XML\XMLComponentAdapter.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Storage\XML\XercesDomWriter.cxx
# End Source File
# End Group
# End Group
# Begin Group "examples Sources"

# Begin Group "SMS Sources"

# Begin Source File

SOURCE=..\..\..\..\examples\SMS\XMLSerializer.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\examples\SMS\SDIFSerializer.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\examples\SMS\SerializationController.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\examples\SMS\SMSAnalysisSynthesisConfig.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\examples\SMS\Progress.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\examples\SMS\WaitMessage.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\examples\SMS\SMSBase.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\examples\SMS\StdOutProgress.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\examples\SMS\StdOutWaitMessage.cxx
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\..\..\examples\SpectralAnalysisExampleHelper.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\examples\SpectralPeakDetectExampleHelper.cxx
# End Source File
# End Group
# Begin Group "Base Sources"

# Begin Source File

SOURCE=..\..\..\..\src\Base\DynamicType.cxx
# End Source File
# End Group
# Begin Group "Data Sources"

# Begin Group "BasicProcessing Sources"

# Begin Source File

SOURCE=..\..\..\..\src\Data\BasicProcessing\Spectrum.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Data\BasicProcessing\Fundamental.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Data\BasicProcessing\SpectralPeak.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Data\BasicProcessing\SpectralPeakArray.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Data\BasicProcessing\Audio.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Data\BasicProcessing\Frame.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Data\BasicProcessing\Segment.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Data\BasicProcessing\MelSpectrum.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Data\BasicProcessing\MelCepstrum.cxx
# End Source File
# End Group
# Begin Group "Descriptors Sources"

# Begin Group "Melody Sources"

# Begin Source File

SOURCE=..\..\..\..\src\Data\Descriptors\Melody\MediaTime.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Data\Descriptors\Melody\PitchNote.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Data\Descriptors\Melody\Note.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Data\Descriptors\Melody\Melody.cxx
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\..\..\src\Data\Descriptors\AudioDescriptors.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Data\Descriptors\SpectralDescriptors.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Data\Descriptors\SpectralPeakDescriptors.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Data\Descriptors\PeriodogramPeak.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Data\Descriptors\MorphologicalFrameDescriptors.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Data\Descriptors\FrameDescriptors.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Data\Descriptors\EnvelopeDescriptors.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Data\Descriptors\MorphologicalSegmentDescriptors.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Data\Descriptors\SegmentDescriptors.cxx
# End Source File
# End Group
# End Group
# Begin Group "Flow Sources"

# Begin Group "Controls Sources"

# Begin Source File

SOURCE=..\..\..\..\src\Flow\Controls\InControl.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Flow\Controls\OutControl.cxx
# End Source File
# End Group
# Begin Group "Ports Sources"

# Begin Source File

SOURCE=..\..\..\..\src\Flow\Ports\Port.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Flow\Ports\OutPort.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Flow\Ports\InPort.cxx
# End Source File
# End Group
# Begin Group "Nodes Sources"

# Begin Source File

SOURCE=..\..\..\..\src\Flow\Nodes\StreamRegion.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Flow\Nodes\ReadStreamRegion.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Flow\Nodes\SourceStreamRegion.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Flow\Nodes\WriteStreamRegion.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Flow\Nodes\Node.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Flow\Nodes\DelayStreamRegion.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Flow\Nodes\AddStreamRegion.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Flow\Nodes\InplaceStreamRegion.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Flow\Nodes\StreamRegionContainer.cxx
# End Source File
# End Group
# End Group
# Begin Group "Processing Sources"

# Begin Group "Base Sources No. 1"

# Begin Source File

SOURCE=..\..\..\..\src\Processing\Base\PublishedInControls.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\Base\PublishedOutControls.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\Base\PublishedInPorts.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\Base\PublishedOutPorts.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\Base\Processing.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\Base\ProcessingComposite.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\Base\ProcessingChain.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\Base\TopLevelProcessing.cxx
# End Source File
# End Group
# Begin Group "SDIFIO Sources"

# Begin Source File

SOURCE=..\..\..\..\src\Processing\SDIFIO\SDIFIn.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\SDIFIO\SDIFOut.cxx
# End Source File
# End Group
# Begin Group "Transformations Sources"

# Begin Group "SMS Sources No. 1"

# Begin Source File

SOURCE=..\..\..\..\src\Processing\Transformations\SMS\SMSTransformationConfig.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\Transformations\SMS\SMSTransformation.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\Transformations\SMS\SMSMorphConfig.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\Transformations\SMS\SMSTimeStretchConfig.cxx
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\..\..\src\Processing\Transformations\Normalization.cxx
# End Source File
# End Group
# Begin Group "AudioIO Sources"

# Begin Source File

SOURCE=..\..\..\..\src\Processing\AudioIO\AudioOut.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\AudioIO\AudioIn.cxx
# End Source File
# End Group
# Begin Group "AudioFileIO Sources"

# Begin Source File

SOURCE=..\..\..\..\src\Processing\AudioFileIO\AudioFileConfig.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\AudioFileIO\AudioFileIn.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\AudioFileIO\AudioFileOut.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\AudioFileIO\MultiChannelAudioFileReaderConfig.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\AudioFileIO\MultiChannelAudioFileReader.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\AudioFileIO\MultiChannelAudioFileWriterConfig.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\AudioFileIO\MultiChannelAudioFileWriter.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\AudioFileIO\MonoAudioFileReaderConfig.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\AudioFileIO\MonoAudioFileReader.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\AudioFileIO\MonoAudioFileWriterConfig.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\AudioFileIO\MonoAudioFileWriter.cxx
# End Source File
# End Group
# Begin Group "Analysis Sources"

# Begin Source File

SOURCE=..\..\..\..\src\Processing\Analysis\FFT_rfftw.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\Analysis\FFT.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\Analysis\FFT_numrec.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\Analysis\FFT_ooura.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\Analysis\WindowGenerator.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\Analysis\ZeroPadder.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\Analysis\CircularShift.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\Analysis\SpectralPeakDetect.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\Analysis\MelFilterBank.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\Analysis\CepstralTransform.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\Analysis\FundFreqDetect.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\Analysis\SinTracking.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\Analysis\SpectralAnalysis.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\Analysis\SMSAnalysis.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\Analysis\CleanTracks.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\Analysis\Segmentator.cxx
# End Source File
# End Group
# Begin Group "Synthesis Sources"

# Begin Source File

SOURCE=..\..\..\..\src\Processing\Synthesis\IFFT_rfftw.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\Synthesis\IFFT.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\Synthesis\SynthSineSpectrum.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\Synthesis\PhaseManagement.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\Synthesis\OverlapAdd.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\Synthesis\SpectralSynthesis.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\Synthesis\SMSSynthesis.cxx
# End Source File
# End Group
# Begin Group "ArithOps Sources"

# Begin Source File

SOURCE=..\..\..\..\src\Processing\ArithOps\AudioMultiplier.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\ArithOps\SpectrumSubstracter2.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\ArithOps\SpectrumAdder2.cxx
# End Source File
# End Group
# End Group
# Begin Group "Tools Sources"

# Begin Group "AudioIO Sources No. 1"

# Begin Source File

SOURCE=..\..\..\..\src\Tools\AudioIO\AudioManager.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Tools\AudioIO\AudioDevice.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Tools\AudioIO\AudioDeviceList.cxx
# End Source File
# End Group
# Begin Group "AudioFileIO Sources No. 1"

# Begin Source File

SOURCE=..\..\..\..\src\Tools\AudioFileIO\AudioFileFormats.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Tools\AudioFileIO\AudioFileHeader.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Tools\AudioFileIO\AudioTextDescriptors.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Tools\AudioFileIO\AudioFile.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Tools\AudioFileIO\SoundHeader.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Tools\AudioFileIO\SoundFileIO.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Tools\AudioFileIO\AIFFFileIO.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Tools\AudioFileIO\WaveFileIO.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Tools\AudioFileIO\AudioCodec.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Tools\AudioFileIO\PCMCodec.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Tools\AudioFileIO\OggVorbisCodec.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Tools\AudioFileIO\MpegCodec.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Tools\AudioFileIO\AudioCodecs_Stream.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Tools\AudioFileIO\PCMAudioStream.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Tools\AudioFileIO\extended.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Tools\AudioFileIO\OggVorbisAudioStream.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Tools\AudioFileIO\MpegBitstream.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Tools\AudioFileIO\MpegAudioStream.cxx
# End Source File
# End Group
# Begin Group "SDIF Sources"

# Begin Source File

SOURCE=..\..\..\..\src\Tools\SDIF\SDIFType.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Tools\SDIF\SDIFHeader.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Tools\SDIF\SDIFFrame.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Tools\SDIF\SDIFMatrix.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Tools\SDIF\SDIFFile.cxx
# End Source File
# End Group
# End Group
# Begin Group "externals Sources"

# Begin Group "numrec Sources"

# Begin Source File

SOURCE=..\..\..\..\externals\numrec\numrecipes_fft.cxx
# End Source File
# End Group
# End Group
# Begin Group "System Sources"

# Begin Group "Threads Sources"

# Begin Source File

SOURCE=..\..\..\..\src\System\Threads\Mutex.cxx
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\..\..\src\System\FileSystem.cxx
# End Source File
# End Group
# End Group
# Begin Group "Header Files"

# Begin Group "cppunit Headers"

# Begin Group "include Headers"

# Begin Group "cppunit Headers No. 1"

# Begin Source File

SOURCE=..\..\..\..\..\cppunit\include\cppunit\Portability.h
# End Source File
# Begin Group "extensions Headers"

# Begin Source File

SOURCE=..\..\..\..\..\cppunit\include\cppunit\extensions\TestFactory.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\cppunit\include\cppunit\extensions\TestFactoryRegistry.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\cppunit\include\cppunit\extensions\TestSuiteFactory.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\cppunit\include\cppunit\extensions\AutoRegisterSuite.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\cppunit\include\cppunit\extensions\TestSuiteBuilder.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\cppunit\include\cppunit\extensions\HelperMacros.h
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\..\..\..\cppunit\include\cppunit\Test.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\cppunit\include\cppunit\TestSuite.h
# End Source File
# Begin Group "ui Headers"

# Begin Group "text Headers"

# Begin Source File

SOURCE=..\..\..\..\..\cppunit\include\cppunit\ui\text\TestRunner.h
# End Source File
# End Group
# End Group
# Begin Source File

SOURCE=..\..\..\..\..\cppunit\include\cppunit\SourceLine.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\cppunit\include\cppunit\Exception.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\cppunit\include\cppunit\Asserter.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\cppunit\include\cppunit\TestAssert.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\cppunit\include\cppunit\TestFixture.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\cppunit\include\cppunit\TestCase.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\cppunit\include\cppunit\TestCaller.h
# End Source File
# End Group
# End Group
# End Group
# Begin Group "test Headers"

# Begin Group "UnitTests Headers"

# Begin Source File

SOURCE=..\..\..\..\test\UnitTests\cppUnitHelper.hxx
# End Source File
# End Group
# Begin Group "FunctionalTests Headers"

# Begin Group "ProcessingTests Headers"

# Begin Source File

SOURCE=..\..\..\..\test\FunctionalTests\ProcessingTests\similarityHelper.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\test\FunctionalTests\ProcessingTests\AudioFileOpsTestsHelper.hxx
# End Source File
# End Group
# End Group
# End Group
# Begin Group "Errors Headers"

# Begin Source File

SOURCE=..\..\..\..\src\Errors\Err.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Errors\ErrOutOfMemory.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Errors\ErrProcessingObj.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Errors\ErrSoundFileIO.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Errors\ErrDynamicType.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Errors\ErrOpenFile.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Errors\ErrSystem.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Errors\ErrFormat.hxx
# End Source File
# End Group
# Begin Group "Defines Headers"

# Begin Source File

SOURCE=..\..\..\..\src\Defines\Assert.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Defines\StaticBool.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Defines\TypeInfo.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Defines\TypeInfoStd.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Defines\DataTypes.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Defines\ByteOrder.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Defines\EDataFormat.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Defines\CLAM_Math.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Defines\OSDefines.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Defines\HeapDbg.hxx
# End Source File
# Begin Group "Windows Headers"

# Begin Source File

SOURCE=..\..\..\..\src\Defines\Windows\CLAM_windows.h
# End Source File
# End Group
# End Group
# Begin Group "Storage Headers"

# Begin Group "Base Headers"

# Begin Source File

SOURCE=..\..\..\..\src\Storage\Base\Storable.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Storage\Base\Storage.hxx
# End Source File
# End Group
# Begin Group "XML Headers"

# Begin Source File

SOURCE=..\..\..\..\src\Storage\XML\XMLStorage.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Storage\XML\XMLable.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Storage\XML\BasicXMLable.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Storage\XML\XMLAdapter.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Storage\XML\XMLComponentAdapter.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Storage\XML\XMLIterableAdapter.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Storage\XML\XMLArrayAdapter.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Storage\XML\XercesEncodings.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Storage\XML\XercesInitializer.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Storage\XML\XercesDomReader.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Storage\XML\XercesDomWriter.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Storage\XML\XercesDomDocumentHandler.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Storage\XML\XercesDomWritingContext.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Storage\XML\XercesDomReadingContext.hxx
# End Source File
# End Group
# End Group
# Begin Group "Base Headers No. 1"

# Begin Source File

SOURCE=..\..\..\..\src\Base\Component.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Base\DynamicTypeMacros.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Base\DynamicType.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Base\Factory.hxx
# End Source File
# End Group
# Begin Group "Standard Headers"

# Begin Source File

SOURCE=..\..\..\..\src\Standard\Enum.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Standard\Array.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Standard\FastRounding.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Standard\ComplexTmplDec.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Standard\Complex.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Standard\PolarTmplDec.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Standard\Polar.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Standard\PointTmplDec.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Standard\PointTmplDef.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Standard\PointTmpl.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Standard\SearchArray.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Standard\GlobalEnums.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Standard\BPFTmplDec.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Standard\Point.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Standard\BPF.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Standard\IndexArray.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Standard\List.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Standard\Search.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Standard\Text.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Standard\Filename.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Standard\Order.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Standard\BasicOps.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Standard\Stats.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Standard\Flags.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Standard\CommonOps.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Standard\PhantomBuffer.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Standard\ComplexTmplDef.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Standard\ComplexTmpl.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Standard\PolarTmplDef.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Standard\PolarTmpl.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Standard\BPFTmplDef.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Standard\BPFTmpl.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Standard\ArrayToBPFCnv.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Standard\ComplexToPolarCnv.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Standard\CircularBuffer.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Standard\MatrixTmplDec.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Standard\Matrix.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Standard\MatrixTmplDef.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Standard\MatrixTmpl.hxx
# End Source File
# End Group
# Begin Group "examples Headers"

# Begin Group "SMS Headers"

# Begin Source File

SOURCE=..\..\..\..\examples\SMS\Serializer.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\examples\SMS\XMLSerializer.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\examples\SMS\SDIFSerializer.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\examples\SMS\SerializationController.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\examples\SMS\SMSAnalysisSynthesisConfig.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\examples\SMS\Progress.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\examples\SMS\WaitMessage.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\examples\SMS\SMSAppState.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\examples\SMS\SMSBase.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\examples\SMS\StdOutProgress.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\examples\SMS\StdOutWaitMessage.hxx
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\..\..\examples\SpectralAnalysisExampleHelper.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\examples\SpectralPeakDetectExampleHelper.hxx
# End Source File
# End Group
# Begin Group "Data Headers"

# Begin Group "Base Headers No. 2"

# Begin Source File

SOURCE=..\..\..\..\src\Data\Base\ProcessingDataConfig.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Data\Base\ProcessingData.hxx
# End Source File
# End Group
# Begin Group "BasicProcessing Headers"

# Begin Source File

SOURCE=..\..\..\..\src\Data\BasicProcessing\Spectrum.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Data\BasicProcessing\Fundamental.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Data\BasicProcessing\SpectralPeak.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Data\BasicProcessing\SpectralPeakArray.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Data\BasicProcessing\Audio.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Data\BasicProcessing\Frame.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Data\BasicProcessing\Segment.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Data\BasicProcessing\SpecTypeFlags.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Data\BasicProcessing\SpectrumConfig.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Data\BasicProcessing\MelSpectrum.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Data\BasicProcessing\MelCepstrum.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Data\BasicProcessing\AudioCircularBuffer.hxx
# End Source File
# End Group
# Begin Group "Descriptors Headers"

# Begin Group "Melody Headers"

# Begin Source File

SOURCE=..\..\..\..\src\Data\Descriptors\Melody\MediaTime.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Data\Descriptors\Melody\PitchNote.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Data\Descriptors\Melody\Note.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Data\Descriptors\Melody\Melody.hxx
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\..\..\src\Data\Descriptors\Descriptor.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Data\Descriptors\AudioDescriptors.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Data\Descriptors\SpectralDescriptors.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Data\Descriptors\SpectralPeakDescriptors.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Data\Descriptors\PeriodogramPeak.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Data\Descriptors\MorphologicalFrameDescriptors.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Data\Descriptors\FrameDescriptors.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Data\Descriptors\EnvelopeDescriptors.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Data\Descriptors\MorphologicalSegmentDescriptors.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Data\Descriptors\SegmentDescriptors.hxx
# End Source File
# End Group
# End Group
# Begin Group "Flow Headers"

# Begin Group "Controls Headers"

# Begin Source File

SOURCE=..\..\..\..\src\Flow\Controls\ControlLinker.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Flow\Controls\InControl.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Flow\Controls\OutControl.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Flow\Controls\InControlTmplArray.hxx
# End Source File
# End Group
# Begin Group "Ports Headers"

# Begin Source File

SOURCE=..\..\..\..\src\Flow\Ports\Port.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Flow\Ports\OutPort.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Flow\Ports\InPort.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Flow\Ports\InPortTmpl.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Flow\Ports\AudioInPortTmpl.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Flow\Ports\OutPortTmpl.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Flow\Ports\AudioOutPortTmpl.hxx
# End Source File
# End Group
# Begin Group "Nodes Headers"

# Begin Source File

SOURCE=..\..\..\..\src\Flow\Nodes\StreamRegion.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Flow\Nodes\ReadStreamRegion.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Flow\Nodes\SourceStreamRegion.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Flow\Nodes\WriteStreamRegion.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Flow\Nodes\Node.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Flow\Nodes\DelayStreamRegion.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Flow\Nodes\AddStreamRegion.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Flow\Nodes\InplaceStreamRegion.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Flow\Nodes\StreamRegionContainer.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Flow\Nodes\CircularStreamImpl.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Flow\Nodes\AudioStreamBuffer.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Flow\Nodes\StreamBuffer.hxx
# End Source File
# End Group
# End Group
# Begin Group "Processing Headers"

# Begin Group "Base Headers No. 3"

# Begin Source File

SOURCE=..\..\..\..\src\Processing\Base\PublishedInControls.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\Base\PublishedOutControls.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\Base\PublishedInPorts.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\Base\PublishedOutPorts.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\Base\ProcessingConfig.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\Base\Processing.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\Base\ProcessingComposite.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\Base\ProcessingChain.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\Base\TopLevelProcessing.hxx
# End Source File
# End Group
# Begin Group "SDIFIO Headers"

# Begin Source File

SOURCE=..\..\..\..\src\Processing\SDIFIO\SDIFIn.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\SDIFIO\SDIFOut.hxx
# End Source File
# End Group
# Begin Group "Synthesis Headers"

# Begin Source File

SOURCE=..\..\..\..\src\Processing\Synthesis\EPhaseGeneration.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\Synthesis\PhaseManagementConfig.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\Synthesis\IFFTConfig.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\Synthesis\SpectralSynthesisConfig.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\Synthesis\OverlapAddConfig.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\Synthesis\SynthSineSpectrumConfig.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\Synthesis\SMSSynthesisConfig.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\Synthesis\IFFT_rfftw.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\Synthesis\IFFT.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\Synthesis\SynthSineSpectrum.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\Synthesis\PhaseManagement.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\Synthesis\OverlapAdd.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\Synthesis\SpectralSynthesis.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\Synthesis\SMSSynthesis.hxx
# End Source File
# End Group
# Begin Group "Analysis Headers"

# Begin Source File

SOURCE=..\..\..\..\src\Processing\Analysis\WindowGeneratorConfig.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\Analysis\CircularShiftConfig.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\Analysis\FFTConfig.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\Analysis\SpectralAnalysisConfig.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\Analysis\SpectralPeakDetectConfig.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\Analysis\FundFreqDetectConfig.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\Analysis\SinTrackingConfig.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\Analysis\SMSAnalysisConfig.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\Analysis\FFT_rfftw.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\Analysis\FFT.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\Analysis\FFT_numrec.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\Analysis\FFT_ooura.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\Analysis\WindowGenerator.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\Analysis\ZeroPadder.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\Analysis\CircularShift.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\Analysis\SpectralPeakDetect.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\Analysis\MelFilterBank.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\Analysis\CepstralTransform.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\Analysis\FundFreqDetect.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\Analysis\SinTracking.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\Analysis\SpectralAnalysis.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\Analysis\SMSAnalysis.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\Analysis\CleanTracksConfig.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\Analysis\CleanTracks.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\Analysis\SegmentatorConfig.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\Analysis\Segmentator.hxx
# End Source File
# End Group
# Begin Group "Transformations Headers"

# Begin Group "SMS Headers No. 1"

# Begin Source File

SOURCE=..\..\..\..\src\Processing\Transformations\SMS\SMSTransformationConfig.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\Transformations\SMS\SMSTransformation.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\Transformations\SMS\SMSTransformationChain.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\Transformations\SMS\SMSMorphConfig.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\Transformations\SMS\SMSTimeStretchConfig.hxx
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\..\..\src\Processing\Transformations\Normalization.hxx
# End Source File
# End Group
# Begin Group "AudioIO Headers"

# Begin Source File

SOURCE=..\..\..\..\src\Processing\AudioIO\AudioIO.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\AudioIO\AudioOut.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\AudioIO\AudioIn.hxx
# End Source File
# End Group
# Begin Group "AudioFileIO Headers"

# Begin Source File

SOURCE=..\..\..\..\src\Processing\AudioFileIO\AudioFileConfig.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\AudioFileIO\AudioFileIn.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\AudioFileIO\AudioFileOut.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\AudioFileIO\MultiChannelAudioFileReaderConfig.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\AudioFileIO\MultiChannelAudioFileReader.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\AudioFileIO\MultiChannelAudioFileWriterConfig.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\AudioFileIO\MultiChannelAudioFileWriter.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\AudioFileIO\MonoAudioFileReaderConfig.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\AudioFileIO\MonoAudioFileReader.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\AudioFileIO\MonoAudioFileWriterConfig.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\AudioFileIO\MonoAudioFileWriter.hxx
# End Source File
# End Group
# Begin Group "ArithOps Headers"

# Begin Source File

SOURCE=..\..\..\..\src\Processing\ArithOps\BinaryAudioOp.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\ArithOps\AudioMultiplier.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\ArithOps\SpectrumSubstracter2.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\ArithOps\SpectrumAdder2.hxx
# End Source File
# End Group
# End Group
# Begin Group "Tools Headers"

# Begin Group "AudioIO Headers No. 1"

# Begin Source File

SOURCE=..\..\..\..\src\Tools\AudioIO\AudioManager.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Tools\AudioIO\AudioDevice.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Tools\AudioIO\AudioDeviceList.hxx
# End Source File
# End Group
# Begin Group "AudioFileIO Headers No. 1"

# Begin Source File

SOURCE=..\..\..\..\src\Tools\AudioFileIO\AudioFileFormats.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Tools\AudioFileIO\AudioFileHeader.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Tools\AudioFileIO\AudioTextDescriptors.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Tools\AudioFileIO\AudioFile.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Tools\AudioFileIO\SoundHeader.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Tools\AudioFileIO\SoundFileIO.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Tools\AudioFileIO\AIFFFileIO.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Tools\AudioFileIO\WaveFileIO.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Tools\AudioFileIO\AudioCodec.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Tools\AudioFileIO\PCMCodec.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Tools\AudioFileIO\OggVorbisCodec.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Tools\AudioFileIO\MpegCodec.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Tools\AudioFileIO\AudioCodecs_Stream.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Tools\AudioFileIO\PCMAudioStream.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Tools\AudioFileIO\extended.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Tools\AudioFileIO\OggVorbisAudioStream.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Tools\AudioFileIO\MpegBitstream.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Tools\AudioFileIO\MpegAudioStream.hxx
# End Source File
# End Group
# Begin Group "SDIF Headers"

# Begin Source File

SOURCE=..\..\..\..\src\Tools\SDIF\SDIFType.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Tools\SDIF\SDIFHeader.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Tools\SDIF\SDIFFrame.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Tools\SDIF\SDIFMatrix.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Tools\SDIF\SDIFStorage.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Tools\SDIF\SDIFFile.hxx
# End Source File
# End Group
# End Group
# Begin Group "fftw Headers"

# Begin Group "include Headers No. 1"

# Begin Source File

SOURCE=..\..\..\..\..\fftw\include\fftw.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\fftw\include\rfftw.h
# End Source File
# End Group
# End Group
# Begin Group "libsndfile Headers"

# Begin Group "include Headers No. 2"

# Begin Source File

SOURCE=..\..\..\..\..\libsndfile\include\sndfile.h
# End Source File
# End Group
# End Group
# Begin Group "xercesc Headers"

# Begin Group "include Headers No. 3"

# Begin Group "xercesc Headers No. 1"

# Begin Group "util Headers"

# Begin Source File

SOURCE=..\..\..\..\..\xercesc\include\xercesc\util\XMLString.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\xercesc\include\xercesc\util\XercesDefs.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\xercesc\include\xercesc\util\XMLUni.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\xercesc\include\xercesc\util\XMemory.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\xercesc\include\xercesc\util\PlatformUtils.hpp
# End Source File
# End Group
# Begin Group "parsers Headers"

# Begin Source File

SOURCE=..\..\..\..\..\xercesc\include\xercesc\parsers\XercesDOMParser.hpp
# End Source File
# End Group
# Begin Group "framework Headers"

# Begin Source File

SOURCE=..\..\..\..\..\xercesc\include\xercesc\framework\MemBufInputSource.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\xercesc\include\xercesc\framework\XMLFormatter.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\xercesc\include\xercesc\framework\MemBufFormatTarget.hpp
# End Source File
# End Group
# Begin Group "sax Headers"

# Begin Source File

SOURCE=..\..\..\..\..\xercesc\include\xercesc\sax\DocumentHandler.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\xercesc\include\xercesc\sax\DTDHandler.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\xercesc\include\xercesc\sax\EntityResolver.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\xercesc\include\xercesc\sax\ErrorHandler.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\xercesc\include\xercesc\sax\SAXException.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\xercesc\include\xercesc\sax\SAXParseException.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\xercesc\include\xercesc\sax\HandlerBase.hpp
# End Source File
# End Group
# Begin Group "dom Headers"

# Begin Source File

SOURCE=..\..\..\..\..\xercesc\include\xercesc\dom\DOMImplementationLS.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\xercesc\include\xercesc\dom\DOMException.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\xercesc\include\xercesc\dom\DOMRangeException.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\xercesc\include\xercesc\dom\DOMImplementation.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\xercesc\include\xercesc\dom\DOMImplementationRegistry.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\xercesc\include\xercesc\dom\DOMNode.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\xercesc\include\xercesc\dom\DOMNodeFilter.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\xercesc\include\xercesc\dom\DOMWriterFilter.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\xercesc\include\xercesc\dom\DOMErrorHandler.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\xercesc\include\xercesc\dom\DOMWriter.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\xercesc\include\xercesc\dom\DOMDocumentRange.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\xercesc\include\xercesc\dom\DOMDocumentTraversal.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\xercesc\include\xercesc\dom\DOMDocument.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\xercesc\include\xercesc\dom\DOMElement.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\xercesc\include\xercesc\dom\DOMCharacterData.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\xercesc\include\xercesc\dom\DOMText.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\xercesc\include\xercesc\dom\DOMNamedNodeMap.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\xercesc\include\xercesc\dom\DOMNodeList.hpp
# End Source File
# End Group
# End Group
# End Group
# End Group
# Begin Group "externals Headers"

# Begin Group "numrec Headers"

# Begin Source File

SOURCE=..\..\..\..\externals\numrec\numrecipes_fft.h
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\..\..\externals\CLAM_deque.hxx
# End Source File
# End Group
# Begin Group "pthreads Headers"

# Begin Group "include Headers No. 4"

# Begin Source File

SOURCE=..\..\..\..\..\pthreads\include\pthread.h
# End Source File
# End Group
# End Group
# Begin Group "System Headers"

# Begin Group "Threads Headers"

# Begin Source File

SOURCE=..\..\..\..\src\System\Threads\xtime.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\System\Threads\Lock.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\System\Threads\Mutex.hxx
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\..\..\src\System\FileSystem.hxx
# End Source File
# End Group
# Begin Group "oggvorbis Headers"

# Begin Group "include Headers No. 5"

# Begin Group "ogg Headers"

# Begin Source File

SOURCE=..\..\..\..\..\oggvorbis\include\ogg\os_types.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\oggvorbis\include\ogg\ogg.h
# End Source File
# End Group
# Begin Group "vorbis Headers"

# Begin Source File

SOURCE=..\..\..\..\..\oggvorbis\include\vorbis\codec.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\oggvorbis\include\vorbis\vorbisfile.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\oggvorbis\include\vorbis\vorbisenc.h
# End Source File
# End Group
# End Group
# End Group
# Begin Group "libmad Headers"

# Begin Group "include Headers No. 6"

# Begin Source File

SOURCE=..\..\..\..\..\libmad\include\mad.h
# End Source File
# End Group
# End Group
# Begin Group "id3lib Headers"

# Begin Group "include Headers No. 7"

# Begin Group "id3 Headers"

# Begin Source File

SOURCE=..\..\..\..\..\id3lib\include\id3\sized_types.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\id3lib\include\id3\globals.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\id3lib\include\id3\id3lib_frame.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\id3lib\include\id3\field.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\id3lib\include\id3\id3lib_streams.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\id3lib\include\id3\id3lib_strings.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\id3lib\include\id3\utils.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\id3lib\include\id3\tag.h
# End Source File
# End Group
# End Group
# End Group
# End Group
# Begin Group "Qt .ui Files"

# End Group
# Begin Source File

SOURCE=settings.cfg

!IF  "$(CFG)" == "AllApplicationTests - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build
InputPath=settings.cfg

"buildstamp" : $(SOURCE) "$(INTDIR)"
	srcdeps.exe settings.cfg

# End Custom Build

!ELSEIF  "$(CFG)" == "AllApplicationTests - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build
InputPath=settings.cfg

"buildstamp" : $(SOURCE) "$(INTDIR)"
	srcdeps.exe settings.cfg

# End Custom Build

!ENDIF 

# End Source File
# End Target
# End Project
