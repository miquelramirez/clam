# Microsoft Developer Studio Project File - Name="SaltoExample" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=SaltoExample - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "SaltoExample.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "SaltoExample.mak" CFG="SaltoExample - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "SaltoExample - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "SaltoExample - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "CLAM_Skeleton_project"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "SaltoExample - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\ReleaseFiles\bin"
# PROP Intermediate_Dir "..\ReleaseFiles\obj"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GR /GX /O2 /FD /c /Zm1000 /D "HAVE_STANDARD_SSTREAM" /D "HAVE_STANDARD_SSTREAM_STR" /D "HAVE_STANDARD_VECTOR_AT" /D "WIN32" /D "_MBCS" /D "_CONSOLE" /D "CLAM_FLOAT" /D "CLAM_USE_XML" /I "..\..\..\src\Defines" /FI"preinclude.hxx" /I "..\..\..\..\fftw\include" /I "..\..\..\..\fltk\include" /I "..\..\..\..\pthreads\include" /I "..\..\..\..\portmidi\include" /I "..\..\..\..\dxsdk\include" /I "..\..\..\examples\Salto\Application" /I "..\..\..\examples\Salto\Application\MIDIIO" /I "..\..\..\examples\Salto\Base" /I "..\..\..\examples\Salto\Data" /I "..\..\..\examples\Salto\Defines" /I "..\..\..\examples\Salto\FileIO" /I "..\..\..\examples\Salto\GUI" /I "..\..\..\examples\Salto\Processing" /I "..\..\..\examples\Salto\Processing\SynthesisAudio" /I "..\..\..\examples\Salto\Storage" /I "..\..\..\src\Base" /I "..\..\..\src\Data\Base" /I "..\..\..\src\Data\BasicProcessing" /I "..\..\..\src\Data\Descriptors\Melody" /I "..\..\..\src\Defines" /I "..\..\..\src\Defines\Windows" /I "..\..\..\src\Errors" /I "..\..\..\src\Flow\Controls" /I "..\..\..\src\Flow\Nodes" /I "..\..\..\src\Flow\Ports" /I "..\..\..\src\Processing\Analysis" /I "..\..\..\src\Processing\ArithOps" /I "..\..\..\src\Processing\AudioFileIO" /I "..\..\..\src\Processing\AudioIO" /I "..\..\..\src\Processing\Base" /I "..\..\..\src\Processing\Controls" /I "..\..\..\src\Processing\MIDIIO" /I "..\..\..\src\Processing\SDIFIO" /I "..\..\..\src\Processing\Synthesis" /I "..\..\..\src\Standard" /I "..\..\..\src\Storage\Base" /I "..\..\..\src\Storage\XML" /I "..\..\..\src\System\Application" /I "..\..\..\src\System\Threads" /I "..\..\..\src\Tools\AudioFileIO" /I "..\..\..\src\Tools\AudioIO" /I "..\..\..\src\Tools\MIDIIO" /I "..\..\..\src\Tools\SDIF" /I "..\..\..\src\Visualization\Base" /I "..\..\..\src\Visualization\Widget" /I "..\..\..\..\xercesc\include"
# ADD BASE RSC /l 0xc0a /d "NDEBUG"
# ADD RSC /l 0xc0a /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 /libpath:"..\..\..\..\fftw\lib" /libpath:"..\..\..\..\fltk\lib" /libpath:"..\..\..\..\xercesc\lib" /libpath:"..\..\..\..\pthreads\lib" /libpath:"..\..\..\..\portmidi\lib" /libpath:"..\..\..\..\dxsdk\lib" RFFTW2st.lib FFTW2st.lib fltk_1_1.lib fltkforms_1_1d.lib fltkgl_1_1d.lib fltkimages_1_1d.lib comctl32.lib kernel32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib wsock32.lib user32.lib gdi32.lib opengl32.lib glu32.lib xerces-c_1.lib dsound.lib dxerr8.lib user32.lib gdi32.lib pthreadVC.lib portmidi.lib porttime.lib winmm.lib /nologo /subsystem:console /machine:I386
# SUBTRACT LINK32 /nologo /verbose /pdb:none /debug

!ELSEIF  "$(CFG)" == "SaltoExample - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\DebugFiles\bin"
# PROP Intermediate_Dir "..\DebugFiles\obj"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /GR /GX /ZI /Od /D "_DEBUG" /FD /GZ /Zm1000 /c /D "HAVE_STANDARD_SSTREAM" /D "HAVE_STANDARD_SSTREAM_STR" /D "HAVE_STANDARD_VECTOR_AT" /D "WIN32" /D "_MBCS" /D "_CONSOLE" /D "CLAM_FLOAT" /D "CLAM_USE_XML" /I "..\..\..\src\Defines" /FI"preinclude.hxx" /I "..\..\..\..\fftw\include" /I "..\..\..\..\fltk\include" /I "..\..\..\..\pthreads\include" /I "..\..\..\..\portmidi\include" /I "..\..\..\..\dxsdk\include" /I "..\..\..\examples\Salto\Application" /I "..\..\..\examples\Salto\Application\MIDIIO" /I "..\..\..\examples\Salto\Base" /I "..\..\..\examples\Salto\Data" /I "..\..\..\examples\Salto\Defines" /I "..\..\..\examples\Salto\FileIO" /I "..\..\..\examples\Salto\GUI" /I "..\..\..\examples\Salto\Processing" /I "..\..\..\examples\Salto\Processing\SynthesisAudio" /I "..\..\..\examples\Salto\Storage" /I "..\..\..\src\Base" /I "..\..\..\src\Data\Base" /I "..\..\..\src\Data\BasicProcessing" /I "..\..\..\src\Data\Descriptors\Melody" /I "..\..\..\src\Defines" /I "..\..\..\src\Defines\Windows" /I "..\..\..\src\Errors" /I "..\..\..\src\Flow\Controls" /I "..\..\..\src\Flow\Nodes" /I "..\..\..\src\Flow\Ports" /I "..\..\..\src\Processing\Analysis" /I "..\..\..\src\Processing\ArithOps" /I "..\..\..\src\Processing\AudioFileIO" /I "..\..\..\src\Processing\AudioIO" /I "..\..\..\src\Processing\Base" /I "..\..\..\src\Processing\Controls" /I "..\..\..\src\Processing\MIDIIO" /I "..\..\..\src\Processing\SDIFIO" /I "..\..\..\src\Processing\Synthesis" /I "..\..\..\src\Standard" /I "..\..\..\src\Storage\Base" /I "..\..\..\src\Storage\XML" /I "..\..\..\src\System\Application" /I "..\..\..\src\System\Threads" /I "..\..\..\src\Tools\AudioFileIO" /I "..\..\..\src\Tools\AudioIO" /I "..\..\..\src\Tools\MIDIIO" /I "..\..\..\src\Tools\SDIF" /I "..\..\..\src\Visualization\Base" /I "..\..\..\src\Visualization\Widget" /I "..\..\..\..\xercesc\include"
# ADD BASE RSC /l 0xc0a /d "_DEBUG"
# ADD RSC /l 0xc0a /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /libpath:"..\..\..\..\fftw\lib" /libpath:"..\..\..\..\fltk\lib" /libpath:"..\..\..\..\xercesc\lib" /libpath:"..\..\..\..\pthreads\lib" /libpath:"..\..\..\..\portmidi\lib" /libpath:"..\..\..\..\dxsdk\lib" FFTW2std.lib RFFTW2std.lib fltk_1_1d.lib fltkforms_1_1d.lib fltkgl_1_1d.lib fltkimages_1_1d.lib comctl32.lib kernel32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib wsock32.lib user32.lib gdi32.lib opengl32.lib glu32.lib xerces-c_1d.lib dsound.lib dxerr8.lib user32.lib gdi32.lib pthreadVC.lib portmidid.lib porttimed.lib winmm.lib /nologo /subsystem:console /debug /machine:I386
# SUBTRACT LINK32 /nologo /verbose /pdb:none

!ENDIF 

# Begin Target

# Name "SaltoExample - Win32 Release"
# Name "SaltoExample - Win32 Debug"
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Group "Source Files"

# Begin Group "examples Sources"

# Begin Group "Salto Sources"

# Begin Group "Application Sources"

# Begin Source File

SOURCE=..\..\..\examples\Salto\Application\SaltoAppMain.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\examples\Salto\Application\Parameters.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\examples\Salto\Application\Parameters.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\examples\Salto\Application\SaltoConfig.cxx
# End Source File
# Begin Group "MIDIIO Sources"

# Begin Source File

SOURCE=..\..\..\examples\Salto\Application\MIDIIO\MIDIHandler.cxx
# End Source File
# End Group
# End Group
# Begin Group "GUI Sources"

# Begin Source File

SOURCE=..\..\..\examples\Salto\GUI\EditorCallbacks.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\examples\Salto\GUI\CSaltoEditor.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\examples\Salto\GUI\CSaltoEditor.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\examples\Salto\GUI\CSaltoSpectralDisplay.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\examples\Salto\GUI\CSaltoDisplay.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\examples\Salto\GUI\CSaltoSpectrumDisplay.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\examples\Salto\GUI\EditorCallbacks.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\examples\Salto\GUI\CSaltoDisplay.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\examples\Salto\GUI\CSaltoSpectralDisplay.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\examples\Salto\GUI\CSaltoSpectrumDisplay.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\examples\Salto\GUI\SDToolTip.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\examples\Salto\GUI\SDToolTip.cxx
# End Source File
# End Group
# Begin Group "FileIO Sources"

# Begin Source File

SOURCE=..\..\..\examples\Salto\FileIO\CSaltoSegData.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\examples\Salto\FileIO\CSaltoStatTmplData.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\examples\Salto\FileIO\CSaltoDataManagment.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\examples\Salto\FileIO\CSaltoFileIO.cxx
# End Source File
# End Group
# Begin Group "Data Sources"

# Begin Source File

SOURCE=..\..\..\examples\Salto\Data\CSaltoTimbreVektor.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\examples\Salto\Data\CSaltoSynthFrame.cxx
# End Source File
# End Group
# Begin Group "Processing Sources"

# Begin Group "SynthesisAudio Sources"

# Begin Source File

SOURCE=..\..\..\examples\Salto\Processing\SynthesisAudio\SineSynthesis.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\examples\Salto\Processing\SynthesisAudio\AttackResidualSynthesis.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\examples\Salto\Processing\SynthesisAudio\StationaryResidualSynthesis.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\examples\Salto\Processing\SynthesisAudio\InterpolatingSynthesis.cxx
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\..\examples\Salto\Processing\SaltoSynth.cxx
# End Source File
# End Group
# Begin Group "Storage Sources"

# Begin Source File

SOURCE=..\..\..\examples\Salto\Storage\MelodyTranslator.cxx
# End Source File
# End Group
# Begin Group "Base Sources"

# Begin Source File

SOURCE=..\..\..\examples\Salto\Base\IO.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\examples\Salto\Base\DataFileIO.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\examples\Salto\Base\ByteOrderConvert.cxx
# End Source File
# End Group
# End Group
# End Group
# Begin Group "Tools Sources"

# Begin Group "AudioIO Sources"

# Begin Group "Windows Sources"

# Begin Source File

SOURCE=..\..\..\src\Tools\AudioIO\Windows\DirectXAudioDevice.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Tools\AudioIO\Windows\RtAAudioDevice.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Tools\AudioIO\Windows\DXFullDuplex.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Tools\AudioIO\Windows\RtAudio.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Tools\AudioIO\Windows\RtAudioUtils.cxx
# End Source File
# End Group
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
# Begin Group "MIDIIO Sources No. 1"

# Begin Group "Windows Sources No. 1"

# Begin Source File

SOURCE=..\..\..\src\Tools\MIDIIO\Windows\PORTMIDIDevice.cxx
# End Source File
# End Group
# Begin Group "File Sources"

# Begin Source File

SOURCE=..\..\..\src\Tools\MIDIIO\File\FileMIDIDevice.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Tools\MIDIIO\File\MIDISong.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Tools\MIDIIO\File\MIDITempo.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Tools\MIDIIO\File\MIDIReader.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Tools\MIDIIO\File\MIDISongPlayer.cxx
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\..\src\Tools\MIDIIO\MIDIDevice.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Tools\MIDIIO\MIDIDeviceList.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Tools\MIDIIO\MIDIManager.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Tools\MIDIIO\MIDIEnums.cxx
# End Source File
# End Group
# Begin Group "AudioFileIO Sources"

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

SOURCE=..\..\..\src\Tools\AudioFileIO\extended.cxx
# End Source File
# End Group
# Begin Group "SDIF Sources"

# Begin Source File

SOURCE=..\..\..\src\Tools\SDIF\SDIFType.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Tools\SDIF\SDIFHeader.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Tools\SDIF\SDIFFrame.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Tools\SDIF\SDIFMatrix.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Tools\SDIF\SDIFFile.cxx
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

SOURCE=..\..\..\src\Errors\ErrDynamicType.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Errors\ErrSystem.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Errors\ErrGUI.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Errors\ErrOpenFile.cxx
# End Source File
# End Group
# Begin Group "Defines Sources"

# Begin Source File

SOURCE=..\..\..\src\Defines\Assert.cxx
# End Source File
# End Group
# Begin Group "Storage Sources No. 1"

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

SOURCE=..\..\..\src\Storage\XML\XMLStaticAdapter.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Storage\XML\XercesDomPrinter.cxx
# End Source File
# End Group
# End Group
# Begin Group "Base Sources No. 1"

# Begin Source File

SOURCE=..\..\..\src\Base\DynamicType.cxx
# End Source File
# End Group
# Begin Group "Standard Sources"

# Begin Source File

SOURCE=..\..\..\src\Standard\Enum.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Standard\GlobalEnums.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Standard\Complex.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Standard\Polar.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Standard\Point.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Standard\BPF.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Standard\Flags.cxx
# End Source File
# End Group
# Begin Group "Flow Sources"

# Begin Group "Controls Sources"

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

SOURCE=..\..\..\src\Flow\Ports\InPort.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Flow\Ports\OutPort.cxx
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

SOURCE=..\..\..\src\Flow\Nodes\AddStreamRegion.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Flow\Nodes\InplaceStreamRegion.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Flow\Nodes\StreamRegionContainer.cxx
# End Source File
# End Group
# End Group
# Begin Group "Processing Sources No. 1"

# Begin Group "Base Sources No. 2"

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
# Begin Group "Analysis Sources"

# Begin Source File

SOURCE=..\..\..\src\Processing\Analysis\WindowGenerator.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Processing\Analysis\CircularShift.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Processing\Analysis\FFT_rfftw.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Processing\Analysis\FFT.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Processing\Analysis\SpectralAnalysis.cxx
# End Source File
# End Group
# Begin Group "Synthesis Sources"

# Begin Source File

SOURCE=..\..\..\src\Processing\Synthesis\IFFT.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Processing\Synthesis\IFFT_rfftw.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Processing\Synthesis\OverlapAdd.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Processing\Synthesis\SpectralSynthesis.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Processing\Synthesis\SynthSineSpectrum.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Processing\Synthesis\PhaseManagement.cxx
# End Source File
# End Group
# Begin Group "ArithOps Sources"

# Begin Source File

SOURCE=..\..\..\src\Processing\ArithOps\AudioMultiplier.cxx
# End Source File
# End Group
# Begin Group "Controls Sources No. 1"

# Begin Source File

SOURCE=..\..\..\src\Processing\Controls\Controller.cxx
# End Source File
# End Group
# Begin Group "AudioIO Sources No. 1"

# Begin Source File

SOURCE=..\..\..\src\Processing\AudioIO\AudioIn.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Processing\AudioIO\AudioOut.cxx
# End Source File
# End Group
# Begin Group "MIDIIO Sources No. 2"

# Begin Source File

SOURCE=..\..\..\src\Processing\MIDIIO\MIDIIn.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Processing\MIDIIO\MIDIInControl.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Processing\MIDIIO\MIDIClocker.cxx
# End Source File
# End Group
# Begin Group "AudioFileIO Sources No. 1"

# Begin Source File

SOURCE=..\..\..\src\Processing\AudioFileIO\AudioFile.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Processing\AudioFileIO\AudioFileOut.cxx
# End Source File
# End Group
# Begin Group "SDIFIO Sources"

# Begin Source File

SOURCE=..\..\..\src\Processing\SDIFIO\SDIFIn.cxx
# End Source File
# End Group
# End Group
# Begin Group "Data Sources No. 1"

# Begin Group "BasicProcessing Sources"

# Begin Source File

SOURCE=..\..\..\src\Data\BasicProcessing\Spectrum.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Data\BasicProcessing\Audio.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Data\BasicProcessing\SpectralPeak.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Data\BasicProcessing\SpectralPeakArray.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Data\BasicProcessing\Fundamental.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Data\BasicProcessing\Frame.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Data\BasicProcessing\Segment.cxx
# End Source File
# End Group
# Begin Group "Descriptors Sources"

# Begin Group "Melody Sources"

# Begin Source File

SOURCE=..\..\..\src\Data\Descriptors\Melody\MediaTime.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Data\Descriptors\Melody\PitchNote.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Data\Descriptors\Melody\Note.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Data\Descriptors\Melody\Melody.cxx
# End Source File
# End Group
# End Group
# End Group
# Begin Group "System Sources"

# Begin Group "Threads Sources"

# Begin Source File

SOURCE=..\..\..\src\System\Threads\Mutex.cxx
# End Source File
# End Group
# Begin Group "Application Sources No. 1"

# Begin Source File

SOURCE=..\..\..\src\System\Application\BaseAudioApplication.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\System\Application\GUIAudioApplication.cxx
# End Source File
# End Group
# End Group
# Begin Group "Visualization Sources"

# Begin Group "Base Sources No. 3"

# Begin Source File

SOURCE=..\..\..\src\Visualization\Base\WidgetTKWrapper.cxx
# End Source File
# End Group
# Begin Group "Widget Sources"

# Begin Source File

SOURCE=..\..\..\src\Visualization\Widget\FLTKWrapper.cxx
# End Source File
# End Group
# End Group
# End Group
# Begin Group "Header Files"

# Begin Group "Storage Headers"

# Begin Group "Base Headers"

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

SOURCE=..\..\..\src\Storage\XML\XMLStorage.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Storage\XML\XMLArrayAdapter.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Storage\XML\XMLStaticAdapter.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Storage\XML\XercesDomPrinter.hxx
# End Source File
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

SOURCE=..\..\..\src\Errors\ErrDynamicType.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Errors\ErrSystem.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Errors\ErrGUI.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Errors\ErrSoundFileIO.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Errors\ErrOpenFile.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Errors\ErrFormat.hxx
# End Source File
# End Group
# Begin Group "Defines Headers"

# Begin Source File

SOURCE=..\..\..\src\Defines\Assert.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Defines\mtgsstream.h
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

SOURCE=..\..\..\src\Defines\CLAM_Math.hxx
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

SOURCE=..\..\..\src\Defines\OSDefines.hxx
# End Source File
# Begin Group "Windows Headers"

# Begin Source File

SOURCE=..\..\..\src\Defines\Windows\CLAM_windows.h
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\..\src\Defines\HeapDbg.hxx
# End Source File
# End Group
# Begin Group "Base Headers No. 1"

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
# Begin Group "Data Headers"

# Begin Group "Base Headers No. 2"

# Begin Source File

SOURCE=..\..\..\src\Data\Base\ProcessingDataConfig.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Data\Base\ProcessingData.hxx
# End Source File
# End Group
# Begin Group "BasicProcessing Headers"

# Begin Source File

SOURCE=..\..\..\src\Data\BasicProcessing\Spectrum.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Data\BasicProcessing\Audio.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Data\BasicProcessing\SpectralPeak.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Data\BasicProcessing\SpectralPeakArray.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Data\BasicProcessing\Fundamental.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Data\BasicProcessing\Frame.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Data\BasicProcessing\Segment.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Data\BasicProcessing\AudioCircularBuffer.hxx
# End Source File
# End Group
# Begin Group "Descriptors Headers"

# Begin Group "Melody Headers"

# Begin Source File

SOURCE=..\..\..\src\Data\Descriptors\Melody\MediaTime.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Data\Descriptors\Melody\PitchNote.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Data\Descriptors\Melody\Note.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Data\Descriptors\Melody\Melody.hxx
# End Source File
# End Group
# End Group
# End Group
# Begin Group "Standard Headers"

# Begin Source File

SOURCE=..\..\..\src\Standard\Enum.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Standard\GlobalEnums.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Standard\Array.hxx
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

SOURCE=..\..\..\src\Standard\BPFTmplDec.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Standard\Point.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Standard\BPF.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Standard\ArrayToBPFCnv.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Standard\ComplexToPolarCnv.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Standard\Flags.hxx
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

SOURCE=..\..\..\src\Standard\PhantomBuffer.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Standard\CircularBuffer.hxx
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
# End Group
# Begin Group "Ports Headers"

# Begin Source File

SOURCE=..\..\..\src\Flow\Ports\Port.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Flow\Ports\InPort.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Flow\Ports\AudioInPortTmpl.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Flow\Ports\InPortTmpl.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Flow\Ports\OutPort.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Flow\Ports\AudioOutPortTmpl.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Flow\Ports\OutPortTmpl.hxx
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

SOURCE=..\..\..\src\Flow\Nodes\AddStreamRegion.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Flow\Nodes\InplaceStreamRegion.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Flow\Nodes\StreamRegionContainer.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Flow\Nodes\AudioStreamBuffer.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Flow\Nodes\StreamBuffer.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Flow\Nodes\CircularStreamImpl.hxx
# End Source File
# End Group
# End Group
# Begin Group "Processing Headers"

# Begin Group "Base Headers No. 3"

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

SOURCE=..\..\..\src\Processing\Base\Processing.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Processing\Base\ProcessingComposite.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Processing\Base\TopLevelProcessing.hxx
# End Source File
# End Group
# Begin Group "Analysis Headers"

# Begin Source File

SOURCE=..\..\..\src\Processing\Analysis\WindowGenerator.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Processing\Analysis\CircularShift.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Processing\Analysis\FFT_rfftw.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Processing\Analysis\FFT.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Processing\Analysis\SpectralAnalysis.hxx
# End Source File
# End Group
# Begin Group "Synthesis Headers"

# Begin Source File

SOURCE=..\..\..\src\Processing\Synthesis\IFFT.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Processing\Synthesis\IFFT_rfftw.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Processing\Synthesis\OverlapAdd.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Processing\Synthesis\SpectralSynthesis.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Processing\Synthesis\SynthSineSpectrum.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Processing\Synthesis\PhaseManagement.hxx
# End Source File
# End Group
# Begin Group "ArithOps Headers"

# Begin Source File

SOURCE=..\..\..\src\Processing\ArithOps\BinaryAudioOp.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Processing\ArithOps\AudioMultiplier.hxx
# End Source File
# End Group
# Begin Group "Controls Headers No. 1"

# Begin Source File

SOURCE=..\..\..\src\Processing\Controls\Controller.hxx
# End Source File
# End Group
# Begin Group "AudioIO Headers"

# Begin Source File

SOURCE=..\..\..\src\Processing\AudioIO\AudioIO.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Processing\AudioIO\AudioIn.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Processing\AudioIO\AudioOut.hxx
# End Source File
# End Group
# Begin Group "MIDIIO Headers"

# Begin Source File

SOURCE=..\..\..\src\Processing\MIDIIO\MIDIIn.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Processing\MIDIIO\MIDIInControl.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Processing\MIDIIO\MIDIClocker.hxx
# End Source File
# End Group
# Begin Group "AudioFileIO Headers"

# Begin Source File

SOURCE=..\..\..\src\Processing\AudioFileIO\AudioFile.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Processing\AudioFileIO\AudioFileOut.hxx
# End Source File
# End Group
# Begin Group "SDIFIO Headers"

# Begin Source File

SOURCE=..\..\..\src\Processing\SDIFIO\SDIFIn.hxx
# End Source File
# End Group
# End Group
# Begin Group "examples Headers"

# Begin Group "Salto Headers"

# Begin Group "FileIO Headers"

# Begin Source File

SOURCE=..\..\..\examples\Salto\FileIO\CSaltoSegData.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\examples\Salto\FileIO\CSaltoStatTmplData.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\examples\Salto\FileIO\CSaltoDataManagment.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\examples\Salto\FileIO\CSaltoFileIO.hxx
# End Source File
# End Group
# Begin Group "Defines Headers No. 1"

# Begin Source File

SOURCE=..\..\..\examples\Salto\Defines\CSaltoDefines.hxx
# End Source File
# End Group
# Begin Group "Data Headers No. 1"

# Begin Source File

SOURCE=..\..\..\examples\Salto\Data\CSaltoTimbreVektor.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\examples\Salto\Data\CSaltoSynthFrame.hxx
# End Source File
# End Group
# Begin Group "Application Headers"

# Begin Source File

SOURCE=..\..\..\examples\Salto\Application\Parameters.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\examples\Salto\Application\Parameters.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\examples\Salto\Application\SaltoConfig.hxx
# End Source File
# Begin Group "MIDIIO Headers No. 1"

# Begin Source File

SOURCE=..\..\..\examples\Salto\Application\MIDIIO\MIDIHandler.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\examples\Salto\Application\MIDIIO\MIDIBreathController.hxx
# End Source File
# End Group
# End Group
# Begin Group "Processing Headers No. 1"

# Begin Group "SynthesisAudio Headers"

# Begin Source File

SOURCE=..\..\..\examples\Salto\Processing\SynthesisAudio\SineSynthesis.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\examples\Salto\Processing\SynthesisAudio\AttackResidualSynthesis.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\examples\Salto\Processing\SynthesisAudio\StationaryResidualSynthesis.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\examples\Salto\Processing\SynthesisAudio\InterpolatingSynthesis.hxx
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\..\examples\Salto\Processing\SaltoSynth.hxx
# End Source File
# End Group
# Begin Group "GUI Headers"

# Begin Source File

SOURCE=..\..\..\examples\Salto\GUI\CSaltoEditor.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\examples\Salto\GUI\CSaltoEditor.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\examples\Salto\GUI\CSaltoSpectralDisplay.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\examples\Salto\GUI\CSaltoDisplay.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\examples\Salto\GUI\CSaltoSpectrumDisplay.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\examples\Salto\GUI\EditorCallbacks.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\examples\Salto\GUI\CSaltoDisplay.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\examples\Salto\GUI\CSaltoSpectralDisplay.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\examples\Salto\GUI\CSaltoSpectrumDisplay.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\examples\Salto\GUI\EditorCallbacks.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\examples\Salto\GUI\SDToolTip.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\examples\Salto\GUI\SDToolTip.hxx
# End Source File
# End Group
# Begin Group "Storage Headers No. 1"

# Begin Source File

SOURCE=..\..\..\examples\Salto\Storage\MelodyTranslator.hxx
# End Source File
# End Group
# Begin Group "Base Headers No. 4"

# Begin Source File

SOURCE=..\..\..\examples\Salto\Base\ByteOrder.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\examples\Salto\Base\EIOMode.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\examples\Salto\Base\IO.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\examples\Salto\Base\DataFileIO.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\examples\Salto\Base\ByteOrderConvert.hxx
# End Source File
# End Group
# End Group
# End Group
# Begin Group "fftw Headers"

# Begin Group "include Headers"

# Begin Source File

SOURCE=..\..\..\..\fftw\include\fftw.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\fftw\include\rfftw.h
# End Source File
# End Group
# End Group
# Begin Group "pthreads Headers"

# Begin Group "include Headers No. 1"

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
# Begin Group "Application Headers No. 1"

# Begin Source File

SOURCE=..\..\..\src\System\Application\Application.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\System\Application\BaseAudioApplication.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\System\Application\GUIAudioApplication.hxx
# End Source File
# End Group
# End Group
# Begin Group "fltk Headers"

# Begin Group "include Headers No. 2"

# Begin Group "FL Headers"

# Begin Source File

SOURCE=..\..\..\..\fltk\include\FL\Fl_Export.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\fltk\include\FL\Enumerations.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\fltk\include\FL\Fl.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\fltk\include\FL\Fl_Widget.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\fltk\include\FL\Fl_Group.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\fltk\include\FL\Fl_Window.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\fltk\include\FL\Fl_Image.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\fltk\include\FL\Fl_Menu_Item.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\fltk\include\FL\Fl_Menu_.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\fltk\include\FL\Fl_Choice.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\fltk\include\FL\Fl_Menu_Button.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\fltk\include\FL\Fl_Button.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\fltk\include\FL\Fl_Preferences.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\fltk\include\FL\Fl_Tile.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\fltk\include\FL\Fl_Valuator.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\fltk\include\FL\Fl_Slider.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\fltk\include\FL\Fl_Scrollbar.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\fltk\include\FL\Fl_Browser_.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\fltk\include\FL\Fl_Browser.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\fltk\include\FL\Fl_File_Icon.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\fltk\include\FL\filename.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\fltk\include\FL\dirent.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\fltk\include\FL\Fl_File_Browser.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\fltk\include\FL\Fl_Box.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\fltk\include\FL\Fl_Light_Button.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\fltk\include\FL\Fl_Check_Button.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\fltk\include\FL\Fl_Input_.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\fltk\include\FL\Fl_Input.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\fltk\include\FL\Fl_File_Input.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\fltk\include\FL\Fl_Return_Button.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\fltk\include\FL\fl_ask.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\fltk\include\FL\fl_file_chooser.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\fltk\include\FL\Fl_Output.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\fltk\include\FL\Fl_Double_Window.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\fltk\include\FL\fl_draw.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\fltk\include\FL\Fl_Value_Slider.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\fltk\include\FL\Fl_Value_Output.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\fltk\include\FL\Fl_Counter.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\fltk\include\FL\Fl_Adjuster.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\fltk\include\FL\Fl_Dial.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\fltk\include\FL\win32.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\fltk\include\FL\x.H
# End Source File
# End Group
# End Group
# End Group
# Begin Group "Visualization Headers"

# Begin Group "Base Headers No. 5"

# Begin Source File

SOURCE=..\..\..\src\Visualization\Base\WidgetTKWrapper.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\Base\SupportedToolkits.hxx
# End Source File
# End Group
# Begin Group "Widget Headers"

# Begin Source File

SOURCE=..\..\..\src\Visualization\Widget\FLTKWrapper.hxx
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
# Begin Group "Windows Headers No. 1"

# Begin Source File

SOURCE=..\..\..\src\Tools\AudioIO\Windows\DXFullDuplex.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Tools\AudioIO\Windows\RtAudio.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Tools\AudioIO\Windows\RtAudioUtils.hxx
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\..\src\Tools\AudioIO\AudioDeviceList.hxx
# End Source File
# End Group
# Begin Group "MIDIIO Headers No. 2"

# Begin Source File

SOURCE=..\..\..\src\Tools\MIDIIO\MIDIDevice.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Tools\MIDIIO\MIDIDeviceList.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Tools\MIDIIO\MIDIManager.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Tools\MIDIIO\MIDIEnums.hxx
# End Source File
# Begin Group "File Headers"

# Begin Source File

SOURCE=..\..\..\src\Tools\MIDIIO\File\MIDIDataTypes.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Tools\MIDIIO\File\MIDIMessage.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Tools\MIDIIO\File\MIDIEvent.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Tools\MIDIIO\File\MIDITrack.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Tools\MIDIIO\File\MIDISong.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Tools\MIDIIO\File\MIDITempo.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Tools\MIDIIO\File\MIDIReader.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Tools\MIDIIO\File\MIDISongPlayer.hxx
# End Source File
# End Group
# End Group
# Begin Group "AudioFileIO Headers No. 1"

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

SOURCE=..\..\..\src\Tools\AudioFileIO\extended.hxx
# End Source File
# End Group
# Begin Group "SDIF Headers"

# Begin Source File

SOURCE=..\..\..\src\Tools\SDIF\SDIFType.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Tools\SDIF\SDIFHeader.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Tools\SDIF\SDIFFrame.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Tools\SDIF\SDIFMatrix.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Tools\SDIF\SDIFStorage.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Tools\SDIF\SDIFFile.hxx
# End Source File
# End Group
# End Group
# Begin Group "dxsdk Headers"

# Begin Group "include Headers No. 3"

# Begin Source File

SOURCE=..\..\..\..\dxsdk\include\dxerr8.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\dxsdk\include\dsound.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\dxsdk\include\dinput.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\dxsdk\include\basetsd.h
# End Source File
# End Group
# End Group
# Begin Group "portmidi Headers"

# Begin Group "include Headers No. 4"

# Begin Source File

SOURCE=..\..\..\..\portmidi\include\portmidi.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\portmidi\include\porttime.h
# End Source File
# End Group
# End Group
# Begin Group "xercesc Headers"

# Begin Group "include Headers No. 5"

# Begin Group "xercesc Headers No. 1"

# Begin Group "util Headers"

# Begin Source File

SOURCE=..\..\..\..\xercesc\include\xercesc\util\XercesDefs.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\xercesc\include\xercesc\util\PlatformUtils.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\xercesc\include\xercesc\util\XMLString.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\xercesc\include\xercesc\util\XMLUniDefs.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\xercesc\include\xercesc\util\TranscodingException.hpp
# End Source File
# End Group
# Begin Group "parsers Headers"

# Begin Source File

SOURCE=..\..\..\..\xercesc\include\xercesc\parsers\DOMParser.hpp
# End Source File
# End Group
# Begin Group "dom Headers"

# Begin Source File

SOURCE=..\..\..\..\xercesc\include\xercesc\dom\DOMString.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\xercesc\include\xercesc\dom\DOM_Node.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\xercesc\include\xercesc\dom\DOM_Element.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\xercesc\include\xercesc\dom\DOM_Attr.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\xercesc\include\xercesc\dom\DOM_CharacterData.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\xercesc\include\xercesc\dom\DOM_Text.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\xercesc\include\xercesc\dom\DOM_CDATASection.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\xercesc\include\xercesc\dom\DOM_Comment.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\xercesc\include\xercesc\dom\DOM_DocumentType.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\xercesc\include\xercesc\dom\DOM_DOMImplementation.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\xercesc\include\xercesc\dom\DOM_DocumentFragment.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\xercesc\include\xercesc\dom\DOM_ProcessingInstruction.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\xercesc\include\xercesc\dom\DOM_Entity.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\xercesc\include\xercesc\dom\DOM_EntityReference.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\xercesc\include\xercesc\dom\DOM_NodeList.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\xercesc\include\xercesc\dom\DOM_Notation.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\xercesc\include\xercesc\dom\DOM_NodeFilter.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\xercesc\include\xercesc\dom\DOM_NodeIterator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\xercesc\include\xercesc\dom\DOM_TreeWalker.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\xercesc\include\xercesc\dom\DOM_XMLDecl.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\xercesc\include\xercesc\dom\DOM_Range.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\xercesc\include\xercesc\dom\DOM_Document.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\xercesc\include\xercesc\dom\DOM_DOMException.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\xercesc\include\xercesc\dom\DOM_NamedNodeMap.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\xercesc\include\xercesc\dom\DOM.hpp
# End Source File
# End Group
# Begin Group "framework Headers"

# Begin Source File

SOURCE=..\..\..\..\xercesc\include\xercesc\framework\XMLFormatter.hpp
# End Source File
# End Group
# End Group
# End Group
# End Group
# End Group
# Begin Source File

SOURCE=settings.cfg

!IF  "$(CFG)" == "SaltoExample - Win32 Release"

# Begin Custom Build
InputPath=settings.cfg

"buildstamp" : $(SOURCE) "$(INTDIR)" "SaltoExample.dsp"
	srcdeps.exe settings.cfg SaltoExample.dsp

# End Custom Build

!ELSEIF  "$(CFG)" == "SaltoExample - Win32 Debug"

# Begin Custom Build
InputPath=settings.cfg

"buildstamp" : $(SOURCE) "$(INTDIR)" "SaltoExample.dsp"
	srcdeps.exe settings.cfg SaltoExample.dsp

# End Custom Build

!ENDIF 

# End Source File
# End Target
# End Project
