# Microsoft Developer Studio Project File - Name="midi2xml" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=midi2xml - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "midi2xml.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "midi2xml.mak" CFG="midi2xml - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "midi2xml - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "midi2xml - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "empty"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "midi2xml - Win32 Release"

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
# ADD CPP /nologo /MD /W3 /GR /GX /O2 /I "..\..\..\..\src\Defines" /I "..\..\..\..\..\xercesc\include" /I "..\..\..\..\..\pthreads\include" /I "..\..\..\..\src\Base" /I "..\..\..\..\src\Data\Base" /I "..\..\..\..\src\Data\BasicProcessing" /I "..\..\..\..\src\Data\Descriptors\Melody" /I "..\..\..\..\src\Defines\Windows" /I "..\..\..\..\src\Errors" /I "..\..\..\..\src\Flow\Controls" /I "..\..\..\..\src\Flow\Nodes" /I "..\..\..\..\src\Flow\Ports" /I "..\..\..\..\src\Processing\Base" /I "..\..\..\..\src\Processing\Controls" /I "..\..\..\..\src\Processing\MIDIIO" /I "..\..\..\..\src\Standard" /I "..\..\..\..\src\Storage\Base" /I "..\..\..\..\src\Storage\XML" /I "..\..\..\..\src\System\Threads" /I "..\..\..\..\src\Tools\MIDIIO" /I "..\..\..\..\src\Tools\MIDIIO\File" /FI"preinclude.hxx" /D "NDEBUG" /D "HAVE_STANDARD_SSTREAM" /D "HAVE_STANDARD_SSTREAM_STR" /D "HAVE_STANDARD_VECTOR_AT" /D "WIN32" /D "_MBCS" /D "_CONSOLE" /D "CLAM_FLOAT" /D "CLAM_USE_XML" /FD /Zm1000 /c
# ADD BASE RSC /l 0xc0a /d "NDEBUG"
# ADD RSC /l 0xc0a /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /machine:IX86
# ADD LINK32 xerces-c_2.lib pthreadVC.lib /subsystem:console /machine:I386 /libpath:"..\..\..\..\..\xercesc\lib" /libpath:"..\..\..\..\..\pthreads\lib"
# SUBTRACT LINK32 /nologo /verbose /pdb:none /debug

!ELSEIF  "$(CFG)" == "midi2xml - Win32 Debug"

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
# ADD CPP /nologo /MDd /W3 /GR /GX /ZI /Od /I "..\..\..\..\src\Defines" /I "..\..\..\..\..\xercesc\include" /I "..\..\..\..\..\pthreads\include" /I "..\..\..\..\src\Base" /I "..\..\..\..\src\Data\Base" /I "..\..\..\..\src\Data\BasicProcessing" /I "..\..\..\..\src\Data\Descriptors\Melody" /I "..\..\..\..\src\Defines\Windows" /I "..\..\..\..\src\Errors" /I "..\..\..\..\src\Flow\Controls" /I "..\..\..\..\src\Flow\Nodes" /I "..\..\..\..\src\Flow\Ports" /I "..\..\..\..\src\Processing\Base" /I "..\..\..\..\src\Processing\Controls" /I "..\..\..\..\src\Processing\MIDIIO" /I "..\..\..\..\src\Standard" /I "..\..\..\..\src\Storage\Base" /I "..\..\..\..\src\Storage\XML" /I "..\..\..\..\src\System\Threads" /I "..\..\..\..\src\Tools\MIDIIO" /I "..\..\..\..\src\Tools\MIDIIO\File" /FI"preinclude.hxx" /D "_DEBUG" /D "HAVE_STANDARD_SSTREAM" /D "HAVE_STANDARD_SSTREAM_STR" /D "HAVE_STANDARD_VECTOR_AT" /D "WIN32" /D "_MBCS" /D "_CONSOLE" /D "CLAM_FLOAT" /D "CLAM_USE_XML" /FD /GZ /Zm1000 /c
# ADD BASE RSC /l 0xc0a /d "_DEBUG"
# ADD RSC /l 0xc0a
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /machine:IX86
# ADD LINK32 xerces-c_2d.lib pthreadVC.lib /subsystem:console /debug /machine:I386 /libpath:"..\..\..\..\..\xercesc\lib" /libpath:"..\..\..\..\..\pthreads\lib"
# SUBTRACT LINK32 /nologo /verbose /pdb:none

!ENDIF 

# Begin Target

# Name "midi2xml - Win32 Release"
# Name "midi2xml - Win32 Debug"
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Group "Source Files"

# PROP Default_Filter ""
# Begin Group "examples Sources"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\examples\Midi2XmlExample.cxx
# End Source File
# End Group
# Begin Group "Tools Sources"

# PROP Default_Filter ""
# Begin Group "MIDIIO Sources"

# PROP Default_Filter ""
# Begin Group "File Sources"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\src\Tools\MIDIIO\File\FileMIDIDevice.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Tools\MIDIIO\File\MIDIReader.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Tools\MIDIIO\File\MIDISong.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Tools\MIDIIO\File\MIDISongPlayer.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Tools\MIDIIO\File\MIDITempo.cxx
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\..\..\src\Tools\MIDIIO\MIDIDevice.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Tools\MIDIIO\MIDIDeviceList.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Tools\MIDIIO\MIDIEnums.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Tools\MIDIIO\MIDIManager.cxx
# End Source File
# End Group
# End Group
# Begin Group "Errors Sources"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\src\Errors\Err.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Errors\ErrDynamicType.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Errors\ErrOutOfMemory.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Errors\ErrProcessingObj.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Errors\ErrSystem.cxx
# End Source File
# End Group
# Begin Group "Defines Sources"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\src\Defines\Assert.cxx
# End Source File
# End Group
# Begin Group "Storage Sources"

# PROP Default_Filter ""
# Begin Group "XML Sources"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\src\Storage\XML\BasicXMLable.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Storage\XML\XercesDomWriter.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Storage\XML\XMLAdapter.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Storage\XML\XMLComponentAdapter.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Storage\XML\XMLStorage.cxx
# End Source File
# End Group
# End Group
# Begin Group "Base Sources"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\src\Base\DynamicType.cxx
# End Source File
# End Group
# Begin Group "Flow Sources"

# PROP Default_Filter ""
# Begin Group "Controls Sources"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\src\Flow\Controls\InControl.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Flow\Controls\OutControl.cxx
# End Source File
# End Group
# Begin Group "Ports Sources"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\src\Flow\Ports\InPort.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Flow\Ports\OutPort.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Flow\Ports\Port.cxx
# End Source File
# End Group
# Begin Group "Nodes Sources"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\src\Flow\Nodes\DelayStreamRegion.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Flow\Nodes\InplaceStreamRegion.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Flow\Nodes\Node.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Flow\Nodes\ReadStreamRegion.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Flow\Nodes\SourceStreamRegion.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Flow\Nodes\StreamRegion.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Flow\Nodes\WriteStreamRegion.cxx
# End Source File
# End Group
# End Group
# Begin Group "Processing Sources"

# PROP Default_Filter ""
# Begin Group "Base Sources No. 1"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\src\Processing\Base\Processing.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\Base\ProcessingComposite.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\Base\PublishedInControls.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\Base\PublishedInPorts.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\Base\PublishedOutControls.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\Base\PublishedOutPorts.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\Base\TopLevelProcessing.cxx
# End Source File
# End Group
# Begin Group "Controls Sources No. 1"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\src\Processing\Controls\Control2Data.cxx
# End Source File
# End Group
# Begin Group "MIDIIO Sources No. 1"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\src\Processing\MIDIIO\MIDI2Melody.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\MIDIIO\MIDIClocker.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\MIDIIO\MIDIIn.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\MIDIIO\MIDIInControl.cxx
# End Source File
# End Group
# End Group
# Begin Group "System Sources"

# PROP Default_Filter ""
# Begin Group "Threads Sources"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\src\System\Threads\Mutex.cxx
# End Source File
# End Group
# End Group
# Begin Group "Standard Sources"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\src\Standard\Enum.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Standard\TraverseDirectory.cxx
# End Source File
# End Group
# Begin Group "Data Sources"

# PROP Default_Filter ""
# Begin Group "Descriptors Sources"

# PROP Default_Filter ""
# Begin Group "Melody Sources"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\src\Data\Descriptors\Melody\MediaTime.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Data\Descriptors\Melody\MIDIMelody.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Data\Descriptors\Melody\MIDINote.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Data\Descriptors\Melody\PitchNote.cxx
# End Source File
# End Group
# End Group
# Begin Group "BasicProcessing Sources"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\src\Data\BasicProcessing\Audio.cxx
# End Source File
# End Group
# End Group
# End Group
# Begin Group "Header Files"

# PROP Default_Filter ""
# Begin Group "Storage Headers"

# PROP Default_Filter ""
# Begin Group "Base Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\src\Storage\Base\Storable.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Storage\Base\Storage.hxx
# End Source File
# End Group
# Begin Group "XML Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\src\Storage\XML\BasicXMLable.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Storage\XML\XercesDomDocumentHandler.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Storage\XML\XercesDomReader.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Storage\XML\XercesDomReadingContext.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Storage\XML\XercesDomWriter.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Storage\XML\XercesDomWritingContext.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Storage\XML\XercesEncodings.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Storage\XML\XercesInitializer.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Storage\XML\XMLable.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Storage\XML\XMLAdapter.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Storage\XML\XMLArrayAdapter.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Storage\XML\XMLComponentAdapter.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Storage\XML\XMLIterableAdapter.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Storage\XML\XMLStorage.hxx
# End Source File
# End Group
# End Group
# Begin Group "Errors Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\src\Errors\Err.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Errors\ErrDynamicType.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Errors\ErrOutOfMemory.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Errors\ErrProcessingObj.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Errors\ErrSystem.hxx
# End Source File
# End Group
# Begin Group "Defines Headers"

# PROP Default_Filter ""
# Begin Group "Windows Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\src\Defines\Windows\CLAM_windows.h
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\..\..\src\Defines\Assert.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Defines\ByteOrder.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Defines\CLAM_Math.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Defines\DataTypes.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Defines\EDataFormat.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Defines\mtgsstream.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Defines\OSDefines.hxx
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
# End Group
# Begin Group "Base Headers No. 1"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\src\Base\Component.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Base\DynamicType.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Base\DynamicTypeMacros.hxx
# End Source File
# End Group
# Begin Group "Flow Headers"

# PROP Default_Filter ""
# Begin Group "Controls Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\src\Flow\Controls\ControlLinker.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Flow\Controls\InControl.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Flow\Controls\InControlTmplArray.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Flow\Controls\OutControl.hxx
# End Source File
# End Group
# Begin Group "Ports Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\src\Flow\Ports\AudioInPortTmpl.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Flow\Ports\AudioOutPortTmpl.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Flow\Ports\InPort.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Flow\Ports\InPortTmpl.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Flow\Ports\OutPort.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Flow\Ports\OutPortTmpl.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Flow\Ports\Port.hxx
# End Source File
# End Group
# Begin Group "Nodes Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\src\Flow\Nodes\DelayStreamRegion.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Flow\Nodes\InplaceStreamRegion.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Flow\Nodes\Node.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Flow\Nodes\ReadStreamRegion.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Flow\Nodes\SourceStreamRegion.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Flow\Nodes\StreamRegion.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Flow\Nodes\WriteStreamRegion.hxx
# End Source File
# End Group
# End Group
# Begin Group "Standard Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\src\Standard\Array.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Standard\Enum.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Standard\FastRounding.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Standard\List.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Standard\TraverseDirectory.hxx
# End Source File
# End Group
# Begin Group "Processing Headers"

# PROP Default_Filter ""
# Begin Group "Base Headers No. 2"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\src\Processing\Base\Processing.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\Base\ProcessingComposite.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\Base\ProcessingConfig.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\Base\PublishedInControls.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\Base\PublishedInPorts.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\Base\PublishedOutControls.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\Base\PublishedOutPorts.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\Base\TopLevelProcessing.hxx
# End Source File
# End Group
# Begin Group "Controls Headers No. 1"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\src\Processing\Controls\Control2Data.hxx
# End Source File
# End Group
# Begin Group "MIDIIO Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\src\Processing\MIDIIO\MIDI2Melody.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\MIDIIO\MIDIClocker.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\MIDIIO\MIDIIn.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\MIDIIO\MIDIInControl.hxx
# End Source File
# End Group
# End Group
# Begin Group "Data Headers"

# PROP Default_Filter ""
# Begin Group "Base Headers No. 3"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\src\Data\Base\ProcessingData.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Data\Base\ProcessingDataConfig.hxx
# End Source File
# End Group
# Begin Group "Descriptors Headers"

# PROP Default_Filter ""
# Begin Group "Melody Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\src\Data\Descriptors\Melody\MediaTime.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Data\Descriptors\Melody\MIDIMelody.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Data\Descriptors\Melody\MIDINote.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Data\Descriptors\Melody\PitchNote.hxx
# End Source File
# End Group
# End Group
# Begin Group "BasicProcessing Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\src\Data\BasicProcessing\Audio.hxx
# End Source File
# End Group
# End Group
# Begin Group "pthreads Headers"

# PROP Default_Filter ""
# Begin Group "include Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\..\pthreads\include\pthread.h
# End Source File
# End Group
# End Group
# Begin Group "System Headers"

# PROP Default_Filter ""
# Begin Group "Threads Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\src\System\Threads\Lock.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\System\Threads\Mutex.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\System\Threads\xtime.hxx
# End Source File
# End Group
# End Group
# Begin Group "Tools Headers"

# PROP Default_Filter ""
# Begin Group "MIDIIO Headers No. 1"

# PROP Default_Filter ""
# Begin Group "File Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\src\Tools\MIDIIO\File\MIDIDataTypes.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Tools\MIDIIO\File\MIDIEvent.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Tools\MIDIIO\File\MIDIMessage.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Tools\MIDIIO\File\MIDIReader.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Tools\MIDIIO\File\MIDISong.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Tools\MIDIIO\File\MIDISongPlayer.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Tools\MIDIIO\File\MIDITempo.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Tools\MIDIIO\File\MIDITrack.hxx
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\..\..\src\Tools\MIDIIO\MIDIDevice.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Tools\MIDIIO\MIDIDeviceList.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Tools\MIDIIO\MIDIEnums.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Tools\MIDIIO\MIDIManager.hxx
# End Source File
# End Group
# End Group
# Begin Group "xercesc Headers"

# PROP Default_Filter ""
# Begin Group "include Headers No. 1"

# PROP Default_Filter ""
# Begin Group "xercesc Headers No. 1"

# PROP Default_Filter ""
# Begin Group "util Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\..\xercesc\include\xercesc\util\PlatformUtils.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\xercesc\include\xercesc\util\XercesDefs.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\xercesc\include\xercesc\util\XMemory.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\xercesc\include\xercesc\util\XMLString.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\xercesc\include\xercesc\util\XMLUni.hpp
# End Source File
# End Group
# Begin Group "parsers Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\..\xercesc\include\xercesc\parsers\XercesDOMParser.hpp
# End Source File
# End Group
# Begin Group "framework Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\..\xercesc\include\xercesc\framework\MemBufFormatTarget.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\xercesc\include\xercesc\framework\MemBufInputSource.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\xercesc\include\xercesc\framework\XMLFormatter.hpp
# End Source File
# End Group
# Begin Group "sax Headers"

# PROP Default_Filter ""
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

SOURCE=..\..\..\..\..\xercesc\include\xercesc\sax\HandlerBase.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\xercesc\include\xercesc\sax\SAXException.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\xercesc\include\xercesc\sax\SAXParseException.hpp
# End Source File
# End Group
# Begin Group "dom Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\..\xercesc\include\xercesc\dom\DOMCharacterData.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\xercesc\include\xercesc\dom\DOMDocument.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\xercesc\include\xercesc\dom\DOMDocumentRange.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\xercesc\include\xercesc\dom\DOMDocumentTraversal.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\xercesc\include\xercesc\dom\DOMElement.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\xercesc\include\xercesc\dom\DOMErrorHandler.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\xercesc\include\xercesc\dom\DOMException.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\xercesc\include\xercesc\dom\DOMImplementation.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\xercesc\include\xercesc\dom\DOMImplementationLS.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\xercesc\include\xercesc\dom\DOMImplementationRegistry.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\xercesc\include\xercesc\dom\DOMNamedNodeMap.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\xercesc\include\xercesc\dom\DOMNode.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\xercesc\include\xercesc\dom\DOMNodeFilter.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\xercesc\include\xercesc\dom\DOMNodeList.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\xercesc\include\xercesc\dom\DOMRangeException.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\xercesc\include\xercesc\dom\DOMText.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\xercesc\include\xercesc\dom\DOMWriter.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\xercesc\include\xercesc\dom\DOMWriterFilter.hpp
# End Source File
# End Group
# End Group
# End Group
# End Group
# End Group
# Begin Source File

SOURCE=settings.cfg

!IF  "$(CFG)" == "midi2xml - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "midi2xml - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# End Target
# End Project
