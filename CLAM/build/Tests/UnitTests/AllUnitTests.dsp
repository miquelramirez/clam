# Microsoft Developer Studio Project File - Name="AllUnitTests" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=AllUnitTests - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "AllUnitTests.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "AllUnitTests.mak" CFG="AllUnitTests - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "AllUnitTests - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "AllUnitTests - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "empty"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "AllUnitTests - Win32 Release"

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
# ADD CPP /nologo /MD /W3 /GR /GX /O2 /Ob0 /FD /c /Zm1000 /D "HAVE_STANDARD_SSTREAM" /D "HAVE_STANDARD_SSTREAM_STR" /D "HAVE_STANDARD_VECTOR_AT" /D "HAVE_STANDARD_UNICODE" /D "WIN32" /D "_MBCS" /D "_CONSOLE" /D "CLAM_FLOAT" /D "CLAM_USE_XML" /D "ID3LIB_LINKOPTION=1" /I "..\..\..\src\Defines" /FI"preinclude.hxx" /I "..\..\..\externals\CbLib" /I "..\..\..\..\xercesc\include" /I "..\..\..\..\fftw\include" /I "..\..\..\..\pthreads\include" /I "..\..\..\..\cppunit\include" /I "..\..\..\..\libsndfile\include" /I "..\..\..\..\oggvorbis\include" /I "..\..\..\..\libmad\include" /I "..\..\..\..\id3lib\include" /I "..\..\..\test\UnitTests" /I "..\..\..\test\UnitTests\CommonHelpers" /I "..\..\..\src\Base" /I "..\..\..\src\Data\Base" /I "..\..\..\src\Data\BasicProcessing" /I "..\..\..\src\Defines" /I "..\..\..\src\Defines\Windows" /I "..\..\..\src\Errors" /I "..\..\..\src\Flow\Controls" /I "..\..\..\src\Flow\Nodes" /I "..\..\..\src\Flow\Ports" /I "..\..\..\src\Processing\Analysis" /I "..\..\..\src\Processing\ArithOps" /I "..\..\..\src\Processing\AudioIO" /I "..\..\..\src\Processing\Base" /I "..\..\..\src\Processing\Controls" /I "..\..\..\src\Processing\Generators" /I "..\..\..\src\Processing\AudioFileIO" /I "..\..\..\src\Standard" /I "..\..\..\src\Storage\Base" /I "..\..\..\src\Storage\XML" /I "..\..\..\src\System\Threads" /I "..\..\..\src\Tools\AudioFileIO" /I "..\..\..\src\Tools\AudioIO" /I "..\..\..\src\Visualization\Base" /I "..\..\..\src\Visualization\Util" /I "..\..\..\src\Visualization\View" /I "..\..\..\externals"
# ADD BASE RSC /l 0xc0a /d "NDEBUG"
# ADD RSC /l 0xc0a /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32
# ADD LINK32 /libpath:"..\..\..\..\fftw\lib" /libpath:"..\..\..\..\xercesc\lib" /libpath:"..\..\..\..\pthreads\lib" /libpath:"..\..\..\..\cppunit\lib" /libpath:"..\..\..\..\libsndfile\lib" /libpath:"..\..\..\..\oggvorbis\lib" /libpath:"..\..\..\..\libmad\lib" /libpath:"..\..\..\..\id3lib\lib" RFFTW2st.lib FFTW2st.lib xerces-c_2.lib pthreadVC.lib cppunit_vc6.lib libsndfile.lib ogg_static.lib vorbis_static.lib vorbisenc_static.lib vorbisfile_static.lib id3lib.lib libmad.lib /nologo /subsystem:console /NODEFAULTLIB:MSVCRTD /machine:I386
# SUBTRACT LINK32 /nologo /verbose /pdb:none /debug

!ELSEIF  "$(CFG)" == "AllUnitTests - Win32 Debug"

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
# ADD CPP /nologo /MDd /W3 /GR /GX /ZI /Od /D "_DEBUG" /FD /GZ /Zm1000 /c /D "HAVE_STANDARD_SSTREAM" /D "HAVE_STANDARD_SSTREAM_STR" /D "HAVE_STANDARD_VECTOR_AT" /D "HAVE_STANDARD_UNICODE" /D "WIN32" /D "_MBCS" /D "_CONSOLE" /D "CLAM_FLOAT" /D "CLAM_USE_XML" /D "ID3LIB_LINKOPTION=1" /I "..\..\..\src\Defines" /FI"preinclude.hxx" /I "..\..\..\externals\CbLib" /I "..\..\..\..\xercesc\include" /I "..\..\..\..\fftw\include" /I "..\..\..\..\pthreads\include" /I "..\..\..\..\cppunit\include" /I "..\..\..\..\libsndfile\include" /I "..\..\..\..\oggvorbis\include" /I "..\..\..\..\libmad\include" /I "..\..\..\..\id3lib\include" /I "..\..\..\test\UnitTests" /I "..\..\..\test\UnitTests\CommonHelpers" /I "..\..\..\src\Base" /I "..\..\..\src\Data\Base" /I "..\..\..\src\Data\BasicProcessing" /I "..\..\..\src\Defines" /I "..\..\..\src\Defines\Windows" /I "..\..\..\src\Errors" /I "..\..\..\src\Flow\Controls" /I "..\..\..\src\Flow\Nodes" /I "..\..\..\src\Flow\Ports" /I "..\..\..\src\Processing\Analysis" /I "..\..\..\src\Processing\ArithOps" /I "..\..\..\src\Processing\AudioIO" /I "..\..\..\src\Processing\Base" /I "..\..\..\src\Processing\Controls" /I "..\..\..\src\Processing\Generators" /I "..\..\..\src\Processing\AudioFileIO" /I "..\..\..\src\Standard" /I "..\..\..\src\Storage\Base" /I "..\..\..\src\Storage\XML" /I "..\..\..\src\System\Threads" /I "..\..\..\src\Tools\AudioFileIO" /I "..\..\..\src\Tools\AudioIO" /I "..\..\..\src\Visualization\Base" /I "..\..\..\src\Visualization\Util" /I "..\..\..\src\Visualization\View" /I "..\..\..\externals"
# ADD BASE RSC /l 0xc0a /d "_DEBUG"
# ADD RSC /l 0xc0a
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32
# ADD LINK32 /libpath:"..\..\..\..\fftw\lib" /libpath:"..\..\..\..\xercesc\lib" /libpath:"..\..\..\..\pthreads\lib" /libpath:"..\..\..\..\cppunit\lib" /libpath:"..\..\..\..\libsndfile\lib" /libpath:"..\..\..\..\oggvorbis\lib" /libpath:"..\..\..\..\libmad\lib" /libpath:"..\..\..\..\id3lib\lib" FFTW2std.lib RFFTW2std.lib xerces-c_2d.lib pthreadVC.lib cppunitd_vc6.lib libsndfiled.lib ogg_static_d.lib vorbis_static_d.lib vorbisenc_static_d.lib vorbisfile_static_d.lib id3libd.lib libmadd.lib /nologo /subsystem:console /NODEFAULTLIB:MSVCRT /debug /machine:I386
# SUBTRACT LINK32 /nologo /verbose /pdb:none

!ENDIF 

# Begin Target

# Name "AllUnitTests - Win32 Release"
# Name "AllUnitTests - Win32 Debug"
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Group "Source Files"

# Begin Group "test Sources"

# Begin Group "UnitTests Sources"

# Begin Source File

SOURCE=..\..\..\test\UnitTests\TestRunnerConsole.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\test\UnitTests\cppUnitHelperTest.cxx
# End Source File
# Begin Group "FactoryTest Sources"

# Begin Source File

SOURCE=..\..\..\test\UnitTests\FactoryTest\FactoryTest.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\test\UnitTests\FactoryTest\FactoryRegistratorTest.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\test\UnitTests\FactoryTest\ExternalRegistrators.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\test\UnitTests\FactoryTest\FactoryRegistryTest.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\test\UnitTests\FactoryTest\FactoryOnOtherTranslationUnits.cxx
# End Source File
# End Group
# Begin Group "ProcessingsTests Sources"

# Begin Source File

SOURCE=..\..\..\test\UnitTests\ProcessingsTests\AudioMixerTest.cxx
# End Source File
# End Group
# Begin Group "PortsTests Sources"

# Begin Source File

SOURCE=..\..\..\test\UnitTests\PortsTests\PortsTest.cxx
# End Source File
# End Group
# Begin Group "ProcessingBaseTests Sources"

# Begin Source File

SOURCE=..\..\..\test\UnitTests\ProcessingBaseTests\ProcessingTest.cxx
# End Source File
# End Group
# Begin Group "VisualizationTests Sources"

# Begin Source File

SOURCE=..\..\..\test\UnitTests\VisualizationTests\FundamentalAdapterTest.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\test\UnitTests\VisualizationTests\FundamentalAdapterTestHelper.cxx
# End Source File
# End Group
# Begin Group "NonComponentData Sources"

# Begin Source File

SOURCE=..\..\..\test\UnitTests\NonComponentData\TextTest.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\test\UnitTests\NonComponentData\ComplexTest.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\test\UnitTests\NonComponentData\PolarTest.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\test\UnitTests\NonComponentData\PointTest.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\test\UnitTests\NonComponentData\EnumTest.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\test\UnitTests\NonComponentData\FlagsTest.cxx
# End Source File
# End Group
# Begin Group "DynamicTypeTests Sources"

# Begin Source File

SOURCE=..\..\..\test\UnitTests\DynamicTypeTests\DummyDynamicTypes.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\test\UnitTests\DynamicTypeTests\DynamicTypeBasicTest.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\test\UnitTests\DynamicTypeTests\DynamicTypeAdvancedTest.cxx
# End Source File
# End Group
# Begin Group "ControlsTests Sources"

# Begin Source File

SOURCE=..\..\..\test\UnitTests\ControlsTests\ControlsTest.cxx
# End Source File
# End Group
# Begin Group "XMLAdaptersTests Sources"

# Begin Source File

SOURCE=..\..\..\test\UnitTests\XMLAdaptersTests\XmlWriteContextTest.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\test\UnitTests\XMLAdaptersTests\XmlScopedTest.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\test\UnitTests\XMLAdaptersTests\XercesDomWriterTest.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\test\UnitTests\XMLAdaptersTests\XercesDomReaderTest.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\test\UnitTests\XMLAdaptersTests\XercesDomReadingContextTest.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\test\UnitTests\XMLAdaptersTests\XmlMockUpObjectsTest.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\test\UnitTests\XMLAdaptersTests\ClamObjects2XercesDomTest.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\test\UnitTests\XMLAdaptersTests\XercesDom2ClamObjectsTest.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\test\UnitTests\XMLAdaptersTests\AdaptersTest.cxx
# End Source File
# End Group
# Begin Group "ToolsTests Sources"

# Begin Source File

SOURCE=..\..\..\test\UnitTests\ToolsTests\AudioFileTest.cxx
# End Source File
# End Group
# Begin Group "StandardTests Sources"

# Begin Source File

SOURCE=..\..\..\test\UnitTests\StandardTests\StatsTest.cxx
# End Source File
# End Group
# Begin Group "CommonHelpers Sources"

# Begin Source File

SOURCE=..\..\..\test\UnitTests\CommonHelpers\XMLTestHelper.cxx
# End Source File
# End Group
# End Group
# End Group
# Begin Group "Processing Sources"

# Begin Group "Base Sources"

# Begin Source File

SOURCE=..\..\..\src\Processing\Base\DefaultProcessingFactoryRegistration.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Processing\Base\PublishedInControls.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Processing\Base\PublishedOutControls.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Processing\Base\PublishedInPorts.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Processing\Base\PublishedOutPorts.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Processing\Base\Processing.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Processing\Base\ProcessingComposite.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Processing\Base\TopLevelProcessing.cxx
# End Source File
# End Group
# Begin Group "Generators Sources"

# Begin Source File

SOURCE=..\..\..\src\Processing\Generators\SimpleOscillator.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Processing\Generators\Oscillator.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Processing\Generators\ADSR.cxx
# End Source File
# End Group
# Begin Group "ArithOps Sources"

# Begin Source File

SOURCE=..\..\..\src\Processing\ArithOps\AudioMultiplier.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Processing\ArithOps\AudioAdder.cxx
# End Source File
# End Group
# Begin Group "AudioFileIO Sources"

# Begin Source File

SOURCE=..\..\..\src\Processing\AudioFileIO\AudioFileConfig.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Processing\AudioFileIO\AudioFileOut.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Processing\AudioFileIO\AudioFileIn.cxx
# End Source File
# End Group
# Begin Group "Controls Sources"

# Begin Source File

SOURCE=..\..\..\src\Processing\Controls\AutoPanner.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Processing\Controls\Controller.cxx
# End Source File
# End Group
# Begin Group "Analysis Sources"

# Begin Source File

SOURCE=..\..\..\src\Processing\Analysis\FFT_rfftw.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Processing\Analysis\FFT.cxx
# End Source File
# End Group
# Begin Group "AudioIO Sources"

# Begin Source File

SOURCE=..\..\..\src\Processing\AudioIO\AudioOut.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Processing\AudioIO\AudioIn.cxx
# End Source File
# End Group
# End Group
# Begin Group "Errors Sources"

# Begin Source File

SOURCE=..\..\..\src\Errors\Err.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Errors\ErrOutOfMemory.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Errors\ErrProcessingObj.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Errors\ErrSoundFileIO.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Errors\ErrDynamicType.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Errors\ErrSystem.cxx
# End Source File
# End Group
# Begin Group "Defines Sources"

# Begin Source File

SOURCE=..\..\..\src\Defines\Assert.cxx
# End Source File
# End Group
# Begin Group "Storage Sources"

# Begin Group "XML Sources"

# Begin Source File

SOURCE=..\..\..\src\Storage\XML\BasicXMLable.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Storage\XML\XMLAdapter.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Storage\XML\XMLComponentAdapter.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Storage\XML\XMLStorage.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Storage\XML\XmlWriteContext.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Storage\XML\XercesDomWriter.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Storage\XML\XMLStaticAdapter.cxx
# End Source File
# End Group
# End Group
# Begin Group "Base Sources No. 1"

# Begin Source File

SOURCE=..\..\..\src\Base\DynamicType.cxx
# End Source File
# End Group
# Begin Group "Flow Sources"

# Begin Group "Controls Sources No. 1"

# Begin Source File

SOURCE=..\..\..\src\Flow\Controls\InControl.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Flow\Controls\OutControl.cxx
# End Source File
# End Group
# Begin Group "Ports Sources"

# Begin Source File

SOURCE=..\..\..\src\Flow\Ports\Port.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Flow\Ports\OutPort.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Flow\Ports\InPort.cxx
# End Source File
# End Group
# Begin Group "Nodes Sources"

# Begin Source File

SOURCE=..\..\..\src\Flow\Nodes\StreamRegion.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Flow\Nodes\ReadStreamRegion.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Flow\Nodes\SourceStreamRegion.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Flow\Nodes\WriteStreamRegion.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Flow\Nodes\Node.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Flow\Nodes\DelayStreamRegion.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Flow\Nodes\InplaceStreamRegion.cxx
# End Source File
# End Group
# End Group
# Begin Group "Data Sources"

# Begin Group "BasicProcessing Sources"

# Begin Source File

SOURCE=..\..\..\src\Data\BasicProcessing\Audio.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Data\BasicProcessing\Spectrum.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Data\BasicProcessing\Fundamental.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Data\BasicProcessing\SpectralPeak.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Data\BasicProcessing\SpectralPeakArray.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Data\BasicProcessing\Frame.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Data\BasicProcessing\Segment.cxx
# End Source File
# End Group
# End Group
# Begin Group "Visualization Sources"

# Begin Group "Base Sources No. 2"

# Begin Source File

SOURCE=..\..\..\src\Visualization\Base\ModelAdapter.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\Base\Signal.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\Base\Connection.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\Base\Slot.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\Base\Presentation.cxx
# End Source File
# End Group
# Begin Group "Util Sources"

# Begin Source File

SOURCE=..\..\..\src\Visualization\Util\TimeFreqMagPoint.cxx
# End Source File
# End Group
# Begin Group "View Sources"

# Begin Source File

SOURCE=..\..\..\src\Visualization\View\FundamentalAdapter.cxx
# End Source File
# End Group
# End Group
# Begin Group "Standard Sources"

# Begin Source File

SOURCE=..\..\..\src\Standard\Complex.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Standard\Polar.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Standard\Enum.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Standard\GlobalEnums.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Standard\Point.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Standard\BPF.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Standard\Text.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Standard\Flags.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Standard\Order.cxx
# End Source File
# End Group
# Begin Group "Tools Sources"

# Begin Group "AudioIO Sources No. 1"

# Begin Source File

SOURCE=..\..\..\src\Tools\AudioIO\AudioManager.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Tools\AudioIO\AudioDevice.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Tools\AudioIO\AudioDeviceList.cxx
# End Source File
# End Group
# Begin Group "AudioFileIO Sources No. 1"

# Begin Source File

SOURCE=..\..\..\src\Tools\AudioFileIO\AudioFileFormats.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Tools\AudioFileIO\AudioFileHeader.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Tools\AudioFileIO\AudioTextDescriptors.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Tools\AudioFileIO\AudioFile.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Tools\AudioFileIO\SoundHeader.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Tools\AudioFileIO\SoundFileIO.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Tools\AudioFileIO\AIFFFileIO.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Tools\AudioFileIO\WaveFileIO.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Tools\AudioFileIO\AudioCodec.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Tools\AudioFileIO\PCMCodec.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Tools\AudioFileIO\OggVorbisCodec.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Tools\AudioFileIO\MpegCodec.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Tools\AudioFileIO\extended.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Tools\AudioFileIO\AudioCodecs_Stream.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Tools\AudioFileIO\PCMAudioStream.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Tools\AudioFileIO\OggVorbisAudioStream.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Tools\AudioFileIO\MpegBitstream.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Tools\AudioFileIO\MpegAudioStream.cxx
# End Source File
# End Group
# End Group
# Begin Group "System Sources"

# Begin Group "Threads Sources"

# Begin Source File

SOURCE=..\..\..\src\System\Threads\Mutex.cxx
# End Source File
# End Group
# End Group
# End Group
# Begin Group "Header Files"

# Begin Group "cppunit Headers"

# Begin Group "include Headers"

# Begin Group "cppunit Headers No. 1"

# Begin Source File

SOURCE=..\..\..\..\cppunit\include\cppunit\Portability.h
# End Source File
# Begin Group "extensions Headers"

# Begin Source File

SOURCE=..\..\..\..\cppunit\include\cppunit\extensions\TestFactory.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\cppunit\include\cppunit\extensions\TestFactoryRegistry.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\cppunit\include\cppunit\extensions\TestSuiteFactory.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\cppunit\include\cppunit\extensions\AutoRegisterSuite.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\cppunit\include\cppunit\extensions\TestSuiteBuilder.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\cppunit\include\cppunit\extensions\HelperMacros.h
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\..\..\cppunit\include\cppunit\Test.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\cppunit\include\cppunit\TestSuite.h
# End Source File
# Begin Group "ui Headers"

# Begin Group "text Headers"

# Begin Source File

SOURCE=..\..\..\..\cppunit\include\cppunit\ui\text\TestRunner.h
# End Source File
# End Group
# End Group
# Begin Source File

SOURCE=..\..\..\..\cppunit\include\cppunit\SourceLine.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\cppunit\include\cppunit\Exception.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\cppunit\include\cppunit\Asserter.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\cppunit\include\cppunit\TestAssert.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\cppunit\include\cppunit\TestFixture.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\cppunit\include\cppunit\TestCase.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\cppunit\include\cppunit\TestCaller.h
# End Source File
# End Group
# End Group
# End Group
# Begin Group "test Headers"

# Begin Group "UnitTests Headers"

# Begin Source File

SOURCE=..\..\..\test\UnitTests\cppUnitHelper.hxx
# End Source File
# Begin Group "CommonHelpers Headers"

# Begin Source File

SOURCE=..\..\..\test\UnitTests\CommonHelpers\BaseLoggable.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\test\UnitTests\CommonHelpers\DummyProcessingData.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\test\UnitTests\CommonHelpers\XMLTestHelper.hxx
# End Source File
# End Group
# Begin Group "FactoryTest Headers"

# Begin Source File

SOURCE=..\..\..\test\UnitTests\FactoryTest\DummyProducts.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\test\UnitTests\FactoryTest\FactoryOnOtherTranslationUnits.hxx
# End Source File
# End Group
# Begin Group "VisualizationTests Headers"

# Begin Source File

SOURCE=..\..\..\test\UnitTests\VisualizationTests\FundamentalAdapterTestHelper.hxx
# End Source File
# End Group
# Begin Group "DynamicTypeTests Headers"

# Begin Source File

SOURCE=..\..\..\test\UnitTests\DynamicTypeTests\DummyDynamicTypes.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\test\UnitTests\DynamicTypeTests\DynamicTypeAdvancedTest.hxx
# End Source File
# End Group
# Begin Group "XMLAdaptersTests Headers"

# Begin Source File

SOURCE=..\..\..\test\UnitTests\XMLAdaptersTests\XmlMockUpObjects.hxx
# End Source File
# End Group
# End Group
# End Group
# Begin Group "Errors Headers"

# Begin Source File

SOURCE=..\..\..\src\Errors\Err.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Errors\ErrOutOfMemory.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Errors\ErrProcessingObj.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Errors\ErrSoundFileIO.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Errors\ErrDynamicType.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Errors\ErrSystem.hxx
# End Source File
# End Group
# Begin Group "Defines Headers"

# Begin Source File

SOURCE=..\..\..\src\Defines\Assert.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Defines\StaticBool.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Defines\TypeInfo.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Defines\TypeInfoStd.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Defines\DataTypes.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Defines\ByteOrder.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Defines\EDataFormat.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Defines\CLAM_Math.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Defines\OSDefines.hxx
# End Source File
# Begin Group "Windows Headers"

# Begin Source File

SOURCE=..\..\..\src\Defines\Windows\CLAM_windows.h
# End Source File
# End Group
# End Group
# Begin Group "Base Headers"

# Begin Source File

SOURCE=..\..\..\src\Base\Factory.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Base\Component.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Base\DynamicTypeMacros.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Base\DynamicType.hxx
# End Source File
# End Group
# Begin Group "Storage Headers"

# Begin Group "Base Headers No. 1"

# Begin Source File

SOURCE=..\..\..\src\Storage\Base\Storable.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Storage\Base\Storage.hxx
# End Source File
# End Group
# Begin Group "XML Headers"

# Begin Source File

SOURCE=..\..\..\src\Storage\XML\XMLable.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Storage\XML\BasicXMLable.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Storage\XML\XMLAdapter.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Storage\XML\XMLComponentAdapter.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Storage\XML\XMLIterableAdapter.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Storage\XML\XMLArrayAdapter.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Storage\XML\XMLStorage.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Storage\XML\XmlWriteContext.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Storage\XML\XmlScoped.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Storage\XML\XercesEncodings.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Storage\XML\XercesInitializer.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Storage\XML\XercesDomWriter.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Storage\XML\XercesDomReader.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Storage\XML\XercesDomDocumentHandler.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Storage\XML\XercesDomReadingContext.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Storage\XML\XMLStaticAdapter.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Storage\XML\XercesDomWritingContext.hxx
# End Source File
# End Group
# End Group
# Begin Group "Flow Headers"

# Begin Group "Controls Headers"

# Begin Source File

SOURCE=..\..\..\src\Flow\Controls\ControlLinker.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Flow\Controls\InControl.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Flow\Controls\OutControl.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Flow\Controls\InControlArray.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Flow\Controls\InControlTmplArray.hxx
# End Source File
# End Group
# Begin Group "Ports Headers"

# Begin Source File

SOURCE=..\..\..\src\Flow\Ports\Port.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Flow\Ports\OutPort.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Flow\Ports\InPort.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Flow\Ports\InPortTmpl.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Flow\Ports\AudioInPortTmpl.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Flow\Ports\OutPortTmpl.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Flow\Ports\AudioOutPortTmpl.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Flow\Ports\InPortArrayTmpl.hxx
# End Source File
# End Group
# Begin Group "Nodes Headers"

# Begin Source File

SOURCE=..\..\..\src\Flow\Nodes\StreamRegion.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Flow\Nodes\ReadStreamRegion.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Flow\Nodes\SourceStreamRegion.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Flow\Nodes\WriteStreamRegion.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Flow\Nodes\Node.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Flow\Nodes\DelayStreamRegion.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Flow\Nodes\InplaceStreamRegion.hxx
# End Source File
# End Group
# End Group
# Begin Group "Standard Headers"

# Begin Source File

SOURCE=..\..\..\src\Standard\Array.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Standard\FastRounding.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Standard\ComplexTmplDec.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Standard\Complex.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Standard\PolarTmplDec.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Standard\Polar.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Standard\PointTmplDec.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Standard\PointTmplDef.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Standard\PointTmpl.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Standard\SearchArray.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Standard\Enum.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Standard\GlobalEnums.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Standard\BPFTmplDec.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Standard\Point.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Standard\BPF.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Standard\IndexArray.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Standard\List.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Standard\Search.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Standard\CommonOps.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Standard\Text.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Standard\Filename.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Standard\Flags.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Standard\Order.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Standard\BasicOps.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Standard\Stats.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Standard\ComplexTmplDef.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Standard\ComplexTmpl.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Standard\PolarTmplDef.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Standard\PolarTmpl.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Standard\BPFTmplDef.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Standard\BPFTmpl.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Standard\ArrayToBPFCnv.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Standard\ComplexToPolarCnv.hxx
# End Source File
# End Group
# Begin Group "Processing Headers"

# Begin Group "Base Headers No. 2"

# Begin Source File

SOURCE=..\..\..\src\Processing\Base\PublishedInControls.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Processing\Base\PublishedOutControls.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Processing\Base\PublishedInPorts.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Processing\Base\PublishedOutPorts.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Processing\Base\ProcessingConfig.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Processing\Base\Processing.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Processing\Base\ProcessingComposite.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Processing\Base\TopLevelProcessing.hxx
# End Source File
# End Group
# Begin Group "ArithOps Headers"

# Begin Source File

SOURCE=..\..\..\src\Processing\ArithOps\AudioMixer.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Processing\ArithOps\BinaryAudioOp.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Processing\ArithOps\AudioMultiplier.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Processing\ArithOps\AudioAdder.hxx
# End Source File
# End Group
# Begin Group "Generators Headers"

# Begin Source File

SOURCE=..\..\..\src\Processing\Generators\SimpleOscillator.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Processing\Generators\Oscillator.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Processing\Generators\ADSR.hxx
# End Source File
# End Group
# Begin Group "AudioFileIO Headers"

# Begin Source File

SOURCE=..\..\..\src\Processing\AudioFileIO\AudioFileConfig.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Processing\AudioFileIO\AudioFileOut.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Processing\AudioFileIO\AudioFileIn.hxx
# End Source File
# End Group
# Begin Group "Controls Headers No. 1"

# Begin Source File

SOURCE=..\..\..\src\Processing\Controls\AutoPanner.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Processing\Controls\Controller.hxx
# End Source File
# End Group
# Begin Group "Analysis Headers"

# Begin Source File

SOURCE=..\..\..\src\Processing\Analysis\FFTConfig.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Processing\Analysis\FFT_rfftw.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Processing\Analysis\FFT.hxx
# End Source File
# End Group
# Begin Group "AudioIO Headers"

# Begin Source File

SOURCE=..\..\..\src\Processing\AudioIO\AudioIO.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Processing\AudioIO\AudioOut.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Processing\AudioIO\AudioIn.hxx
# End Source File
# End Group
# End Group
# Begin Group "Data Headers"

# Begin Group "Base Headers No. 3"

# Begin Source File

SOURCE=..\..\..\src\Data\Base\ProcessingDataConfig.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Data\Base\ProcessingData.hxx
# End Source File
# End Group
# Begin Group "BasicProcessing Headers"

# Begin Source File

SOURCE=..\..\..\src\Data\BasicProcessing\Audio.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Data\BasicProcessing\Spectrum.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Data\BasicProcessing\Fundamental.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Data\BasicProcessing\SpectralPeak.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Data\BasicProcessing\SpectralPeakArray.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Data\BasicProcessing\Frame.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Data\BasicProcessing\Segment.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Data\BasicProcessing\SpecTypeFlags.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Data\BasicProcessing\SpectrumConfig.hxx
# End Source File
# End Group
# End Group
# Begin Group "Visualization Headers"

# Begin Group "Base Headers No. 4"

# Begin Source File

SOURCE=..\..\..\src\Visualization\Base\ModelAdapter.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\Base\Signal.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\Base\Connection.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\Base\Slot.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\Base\Slotv2.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\Base\ConnectionHandler.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\Base\Signalv2ImplSerious.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\Base\Signalv2.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\Base\Presentation.hxx
# End Source File
# End Group
# Begin Group "Util Headers"

# Begin Source File

SOURCE=..\..\..\src\Visualization\Util\TimeFreqMagPoint.hxx
# End Source File
# End Group
# Begin Group "View Headers"

# Begin Source File

SOURCE=..\..\..\src\Visualization\View\FundamentalAdapter.hxx
# End Source File
# End Group
# End Group
# Begin Group "externals Headers"

# Begin Group "CbLib Headers"

# Begin Source File

SOURCE=..\..\..\externals\CbLib\FunctorBase.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\externals\CbLib\Functor2.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\externals\CbLib\Functor0.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\externals\CbLib\Functor1.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\externals\CbLib\Functor3.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\externals\CbLib\Functor4.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\externals\CbLib\Functor0R.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\externals\CbLib\Functor1R.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\externals\CbLib\Functor2R.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\externals\CbLib\Functor3R.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\externals\CbLib\Functor4R.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\externals\CbLib\CBLMacros.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\externals\CbLib\CBL.hxx
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\..\externals\CLAM_deque.hxx
# End Source File
# End Group
# Begin Group "fftw Headers"

# Begin Group "include Headers No. 1"

# Begin Source File

SOURCE=..\..\..\..\fftw\include\fftw.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\fftw\include\rfftw.h
# End Source File
# End Group
# End Group
# Begin Group "Tools Headers"

# Begin Group "AudioIO Headers No. 1"

# Begin Source File

SOURCE=..\..\..\src\Tools\AudioIO\AudioManager.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Tools\AudioIO\AudioDevice.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Tools\AudioIO\AudioDeviceList.hxx
# End Source File
# End Group
# Begin Group "AudioFileIO Headers No. 1"

# Begin Source File

SOURCE=..\..\..\src\Tools\AudioFileIO\AudioFileFormats.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Tools\AudioFileIO\AudioFileHeader.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Tools\AudioFileIO\AudioTextDescriptors.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Tools\AudioFileIO\AudioFile.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Tools\AudioFileIO\SoundHeader.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Tools\AudioFileIO\SoundFileIO.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Tools\AudioFileIO\AIFFFileIO.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Tools\AudioFileIO\WaveFileIO.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Tools\AudioFileIO\AudioCodec.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Tools\AudioFileIO\PCMCodec.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Tools\AudioFileIO\OggVorbisCodec.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Tools\AudioFileIO\MpegCodec.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Tools\AudioFileIO\extended.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Tools\AudioFileIO\AudioCodecs_Stream.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Tools\AudioFileIO\PCMAudioStream.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Tools\AudioFileIO\OggVorbisAudioStream.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Tools\AudioFileIO\MpegBitstream.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Tools\AudioFileIO\MpegAudioStream.hxx
# End Source File
# End Group
# End Group
# Begin Group "pthreads Headers"

# Begin Group "include Headers No. 2"

# Begin Source File

SOURCE=..\..\..\..\pthreads\include\pthread.h
# End Source File
# End Group
# End Group
# Begin Group "System Headers"

# Begin Group "Threads Headers"

# Begin Source File

SOURCE=..\..\..\src\System\Threads\xtime.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\System\Threads\Lock.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\System\Threads\Mutex.hxx
# End Source File
# End Group
# End Group
# Begin Group "xercesc Headers"

# Begin Group "include Headers No. 3"

# Begin Group "xercesc Headers No. 1"

# Begin Group "util Headers"

# Begin Source File

SOURCE=..\..\..\..\xercesc\include\xercesc\util\XMLString.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\xercesc\include\xercesc\util\PlatformUtils.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\xercesc\include\xercesc\util\XercesDefs.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\xercesc\include\xercesc\util\XMLUni.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\xercesc\include\xercesc\util\XMemory.hpp
# End Source File
# End Group
# Begin Group "dom Headers"

# Begin Source File

SOURCE=..\..\..\..\xercesc\include\xercesc\dom\DOMImplementationLS.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\xercesc\include\xercesc\dom\DOMException.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\xercesc\include\xercesc\dom\DOMRangeException.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\xercesc\include\xercesc\dom\DOMImplementation.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\xercesc\include\xercesc\dom\DOMImplementationRegistry.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\xercesc\include\xercesc\dom\DOMNode.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\xercesc\include\xercesc\dom\DOMNodeFilter.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\xercesc\include\xercesc\dom\DOMWriterFilter.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\xercesc\include\xercesc\dom\DOMErrorHandler.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\xercesc\include\xercesc\dom\DOMWriter.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\xercesc\include\xercesc\dom\DOMDocumentRange.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\xercesc\include\xercesc\dom\DOMDocumentTraversal.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\xercesc\include\xercesc\dom\DOMDocument.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\xercesc\include\xercesc\dom\DOMElement.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\xercesc\include\xercesc\dom\DOMCharacterData.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\xercesc\include\xercesc\dom\DOMText.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\xercesc\include\xercesc\dom\DOMComment.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\xercesc\include\xercesc\dom\DOMProcessingInstruction.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\xercesc\include\xercesc\dom\DOMNamedNodeMap.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\xercesc\include\xercesc\dom\DOMNodeList.hpp
# End Source File
# End Group
# Begin Group "framework Headers"

# Begin Source File

SOURCE=..\..\..\..\xercesc\include\xercesc\framework\XMLFormatter.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\xercesc\include\xercesc\framework\MemBufFormatTarget.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\xercesc\include\xercesc\framework\MemBufInputSource.hpp
# End Source File
# End Group
# Begin Group "parsers Headers"

# Begin Source File

SOURCE=..\..\..\..\xercesc\include\xercesc\parsers\XercesDOMParser.hpp
# End Source File
# End Group
# Begin Group "sax Headers"

# Begin Source File

SOURCE=..\..\..\..\xercesc\include\xercesc\sax\DocumentHandler.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\xercesc\include\xercesc\sax\DTDHandler.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\xercesc\include\xercesc\sax\EntityResolver.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\xercesc\include\xercesc\sax\ErrorHandler.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\xercesc\include\xercesc\sax\SAXException.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\xercesc\include\xercesc\sax\SAXParseException.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\xercesc\include\xercesc\sax\HandlerBase.hpp
# End Source File
# End Group
# End Group
# End Group
# End Group
# Begin Group "libsndfile Headers"

# Begin Group "include Headers No. 4"

# Begin Source File

SOURCE=..\..\..\..\libsndfile\include\sndfile.h
# End Source File
# End Group
# End Group
# Begin Group "oggvorbis Headers"

# Begin Group "include Headers No. 5"

# Begin Group "ogg Headers"

# Begin Source File

SOURCE=..\..\..\..\oggvorbis\include\ogg\os_types.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\oggvorbis\include\ogg\ogg.h
# End Source File
# End Group
# Begin Group "vorbis Headers"

# Begin Source File

SOURCE=..\..\..\..\oggvorbis\include\vorbis\codec.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\oggvorbis\include\vorbis\vorbisfile.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\oggvorbis\include\vorbis\vorbisenc.h
# End Source File
# End Group
# End Group
# End Group
# Begin Group "libmad Headers"

# Begin Group "include Headers No. 6"

# Begin Source File

SOURCE=..\..\..\..\libmad\include\mad.h
# End Source File
# End Group
# End Group
# Begin Group "id3lib Headers"

# Begin Group "include Headers No. 7"

# Begin Group "id3 Headers"

# Begin Source File

SOURCE=..\..\..\..\id3lib\include\id3\sized_types.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\id3lib\include\id3\globals.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\id3lib\include\id3\id3lib_frame.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\id3lib\include\id3\field.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\id3lib\include\id3\id3lib_streams.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\id3lib\include\id3\id3lib_strings.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\id3lib\include\id3\utils.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\id3lib\include\id3\tag.h
# End Source File
# End Group
# End Group
# End Group
# End Group
# Begin Group "Qt .ui Files"

# End Group
# Begin Source File

SOURCE=settings.cfg

!IF  "$(CFG)" == "AllUnitTests - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build
InputPath=settings.cfg

"buildstamp" : $(SOURCE) "$(INTDIR)"
	srcdeps.exe settings.cfg

# End Custom Build

!ELSEIF  "$(CFG)" == "AllUnitTests - Win32 Debug"

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
