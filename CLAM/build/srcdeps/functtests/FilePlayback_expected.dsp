# Microsoft Developer Studio Project File - Name="FilePlayback" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=FilePlayback - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "FilePlayback.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "FilePlayback.mak" CFG="FilePlayback - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "FilePlayback - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "FilePlayback - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "empty"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "FilePlayback - Win32 Release"

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
# ADD CPP -O3 -fomit-frame-pointer -pipe /D "HAVE_STANDARD_SSTREAM" /D "HAVE_STANDARD_SSTREAM_STR" /D "CLAM_HAVE_PTHREADS" /D "HAVE_STANDARD_UNICODE" /D "__LINUX_ALSA__" /D "CLAM_FLOAT" /I "..\..\..\src\Defines" /FI"preinclude.hxx" /I "..\..\..\src\Base" /I "..\..\..\src\Data\Base" /I "..\..\..\src\Data\BasicProcessing" /I "..\..\..\src\Defines" /I "..\..\..\src\Errors" /I "..\..\..\src\Flow\Controls" /I "..\..\..\src\Flow\Nodes" /I "..\..\..\src\Flow\Ports" /I "..\..\..\src\Processing\AudioFileIO" /I "..\..\..\src\Processing\AudioIO" /I "..\..\..\src\Processing\Base" /I "..\..\..\src\Standard" /I "..\..\..\src\Storage\Base" /I "..\..\..\src\Storage\XML" /I "..\..\..\src\Tools\AudioFileIO" /I "..\..\..\src\Tools\AudioIO"
# ADD BASE RSC /l 0xc0a /d "NDEBUG"
# ADD RSC /l 0xc0a /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32
# ADD LINK32 /libpath:"\usr\X11R6\lib" asound.lib pthread.lib sndfile.lib vorbis.lib ogg.lib vorbisfile.lib vorbisenc.lib mad.lib m.lib fltk_images.lib png.lib jpeg.lib z.lib fltk_gl.lib GLU.lib GL.lib fltk.lib Xft.lib pthread.lib m.lib Xext.lib X11.lib stdc++.lib id3.lib z.lib
# SUBTRACT LINK32 /nologo /verbose /pdb:none /debug

!ELSEIF  "$(CFG)" == "FilePlayback - Win32 Debug"

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
# ADD CPP -g -fno-inline -pipe -D_DEBUG /D "HAVE_STANDARD_SSTREAM" /D "HAVE_STANDARD_SSTREAM_STR" /D "CLAM_HAVE_PTHREADS" /D "HAVE_STANDARD_UNICODE" /D "__LINUX_ALSA__" /D "CLAM_FLOAT" /I "..\..\..\src\Defines" /FI"preinclude.hxx" /I "..\..\..\src\Base" /I "..\..\..\src\Data\Base" /I "..\..\..\src\Data\BasicProcessing" /I "..\..\..\src\Defines" /I "..\..\..\src\Errors" /I "..\..\..\src\Flow\Controls" /I "..\..\..\src\Flow\Nodes" /I "..\..\..\src\Flow\Ports" /I "..\..\..\src\Processing\AudioFileIO" /I "..\..\..\src\Processing\AudioIO" /I "..\..\..\src\Processing\Base" /I "..\..\..\src\Standard" /I "..\..\..\src\Storage\Base" /I "..\..\..\src\Storage\XML" /I "..\..\..\src\Tools\AudioFileIO" /I "..\..\..\src\Tools\AudioIO"
# ADD BASE RSC /l 0xc0a /d "_DEBUG"
# ADD RSC /l 0xc0a
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32
# ADD LINK32 /libpath:"\usr\X11R6\lib" asound.lib pthread.lib fltk_images.lib png.lib jpeg.lib z.lib fltk_gl.lib GLU.lib GL.lib fltk.lib Xft.lib pthread.lib m.lib Xext.lib X11.lib stdc++.lib sndfile.lib vorbis.lib ogg.lib vorbisfile.lib vorbisenc.lib mad.lib m.lib id3.lib z.lib -rdynamic
# SUBTRACT LINK32 /nologo /verbose /pdb:none

!ENDIF 

# Begin Target

# Name "FilePlayback - Win32 Release"
# Name "FilePlayback - Win32 Debug"
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Group "Source Files"

# Begin Group "examples Sources"

# Begin Source File

SOURCE=..\..\..\examples\FilePlayback_example.cxx
# End Source File
# End Group
# Begin Group "Tools Sources"

# Begin Group "AudioIO Sources"

# Begin Group "Linux Sources"

# Begin Source File

SOURCE=..\..\..\src\Tools\AudioIO\Linux\ALSAAudioDevice.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Tools\AudioIO\Linux\SndPcm.cxx
# End Source File
# End Group
# Begin Group "RtAudio Sources"

# Begin Source File

SOURCE=..\..\..\src\Tools\AudioIO\RtAudio\RtAAudioDevice.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Tools\AudioIO\RtAudio\RtAudio.cxx
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
# Begin Group "AudioFileIO Sources"

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
# End Group
# End Group
# Begin Group "Base Sources"

# Begin Source File

SOURCE=..\..\..\src\Base\DynamicType.cxx
# End Source File
# End Group
# Begin Group "Data Sources"

# Begin Group "BasicProcessing Sources"

# Begin Source File

SOURCE=..\..\..\src\Data\BasicProcessing\Audio.cxx
# End Source File
# End Group
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

SOURCE=..\..\..\src\Flow\Nodes\InplaceStreamRegion.cxx
# End Source File
# End Group
# End Group
# Begin Group "Processing Sources"

# Begin Group "Base Sources No. 1"

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
# Begin Group "AudioIO Sources No. 1"

# Begin Source File

SOURCE=..\..\..\src\Processing\AudioIO\AudioOut.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Processing\AudioIO\AudioIn.cxx
# End Source File
# End Group
# Begin Group "AudioFileIO Sources No. 1"

# Begin Source File

SOURCE=..\..\..\src\Processing\AudioFileIO\MultiChannelAudioFileReaderConfig.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Processing\AudioFileIO\MultiChannelAudioFileReader.cxx
# End Source File
# End Group
# End Group
# Begin Group "Standard Sources"

# Begin Source File

SOURCE=..\..\..\src\Standard\Text.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Standard\Enum.cxx
# End Source File
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

SOURCE=..\..\..\src\Storage\XML\XMLArrayAdapter.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Storage\XML\XMLStorage.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Storage\XML\XercesDomDocumentHandler.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Storage\XML\XercesDomWritingContext.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Storage\XML\XercesDomReadingContext.hxx
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
# End Group
# Begin Group " Headers"

# Begin Group "usr Headers"

# Begin Group "include Headers"

# Begin Group "sys Headers"

# Begin Source File

SOURCE=\usr\include\sys\cdefs.h
# End Source File
# Begin Source File

SOURCE=\usr\include\sys\time.h
# End Source File
# Begin Source File

SOURCE=\usr\include\sys\select.h
# End Source File
# Begin Source File

SOURCE=\usr\include\sys\types.h
# End Source File
# Begin Source File

SOURCE=\usr\include\sys\poll.h
# End Source File
# End Group
# Begin Group "gnu Headers"

# Begin Source File

SOURCE=\usr\include\gnu\stubs.h
# End Source File
# End Group
# Begin Source File

SOURCE=\usr\include\features.h
# End Source File
# Begin Group "bits Headers"

# Begin Source File

SOURCE=\usr\include\bits\wordsize.h
# End Source File
# Begin Source File

SOURCE=\usr\include\bits\typesizes.h
# End Source File
# Begin Source File

SOURCE=\usr\include\bits\types.h
# End Source File
# Begin Source File

SOURCE=\usr\include\bits\endian.h
# End Source File
# Begin Source File

SOURCE=\usr\include\bits\time.h
# End Source File
# Begin Source File

SOURCE=\usr\include\bits\select.h
# End Source File
# Begin Source File

SOURCE=\usr\include\bits\sigset.h
# End Source File
# Begin Source File

SOURCE=\usr\include\bits\posix_opt.h
# End Source File
# Begin Source File

SOURCE=\usr\include\bits\confname.h
# End Source File
# Begin Source File

SOURCE=\usr\include\bits\fcntl.h
# End Source File
# Begin Source File

SOURCE=\usr\include\bits\poll.h
# End Source File
# Begin Source File

SOURCE=\usr\include\bits\errno.h
# End Source File
# End Group
# Begin Source File

SOURCE=\usr\include\endian.h
# End Source File
# Begin Source File

SOURCE=\usr\include\ctype.h
# End Source File
# Begin Source File

SOURCE=\usr\include\stdio.h
# End Source File
# Begin Source File

SOURCE=\usr\include\stdlib.h
# End Source File
# Begin Source File

SOURCE=\usr\include\sndfile.h
# End Source File
# Begin Source File

SOURCE=\usr\include\unistd.h
# End Source File
# Begin Source File

SOURCE=\usr\include\string.h
# End Source File
# Begin Source File

SOURCE=\usr\include\fcntl.h
# End Source File
# Begin Source File

SOURCE=\usr\include\assert.h
# End Source File
# Begin Group "asm-generic Headers"

# Begin Source File

SOURCE=\usr\include\asm-generic\errno-base.h
# End Source File
# Begin Source File

SOURCE=\usr\include\asm-generic\errno.h
# End Source File
# End Group
# Begin Group "asm Headers"

# Begin Source File

SOURCE=\usr\include\asm\errno.h
# End Source File
# End Group
# Begin Group "linux Headers"

# Begin Source File

SOURCE=\usr\include\linux\errno.h
# End Source File
# End Group
# Begin Source File

SOURCE=\usr\include\errno.h
# End Source File
# Begin Group "alsa Headers"

# Begin Source File

SOURCE=\usr\include\alsa\asoundef.h
# End Source File
# Begin Source File

SOURCE=\usr\include\alsa\version.h
# End Source File
# Begin Source File

SOURCE=\usr\include\alsa\global.h
# End Source File
# Begin Source File

SOURCE=\usr\include\alsa\input.h
# End Source File
# Begin Source File

SOURCE=\usr\include\alsa\output.h
# End Source File
# Begin Source File

SOURCE=\usr\include\alsa\error.h
# End Source File
# Begin Source File

SOURCE=\usr\include\alsa\conf.h
# End Source File
# Begin Source File

SOURCE=\usr\include\alsa\pcm.h
# End Source File
# Begin Source File

SOURCE=\usr\include\alsa\rawmidi.h
# End Source File
# Begin Source File

SOURCE=\usr\include\alsa\timer.h
# End Source File
# Begin Source File

SOURCE=\usr\include\alsa\hwdep.h
# End Source File
# Begin Source File

SOURCE=\usr\include\alsa\control.h
# End Source File
# Begin Source File

SOURCE=\usr\include\alsa\mixer.h
# End Source File
# Begin Source File

SOURCE=\usr\include\alsa\seq_event.h
# End Source File
# Begin Source File

SOURCE=\usr\include\alsa\seq.h
# End Source File
# Begin Source File

SOURCE=\usr\include\alsa\seqmid.h
# End Source File
# Begin Source File

SOURCE=\usr\include\alsa\seq_midi_event.h
# End Source File
# Begin Source File

SOURCE=\usr\include\alsa\conv.h
# End Source File
# Begin Source File

SOURCE=\usr\include\alsa\instr.h
# End Source File
# Begin Source File

SOURCE=\usr\include\alsa\asoundlib.h
# End Source File
# End Group
# Begin Group "ogg Headers"

# Begin Source File

SOURCE=\usr\include\ogg\config_types.h
# End Source File
# Begin Source File

SOURCE=\usr\include\ogg\os_types.h
# End Source File
# Begin Source File

SOURCE=\usr\include\ogg\ogg.h
# End Source File
# End Group
# Begin Group "vorbis Headers"

# Begin Source File

SOURCE=\usr\include\vorbis\codec.h
# End Source File
# Begin Source File

SOURCE=\usr\include\vorbis\vorbisfile.h
# End Source File
# Begin Source File

SOURCE=\usr\include\vorbis\vorbisenc.h
# End Source File
# End Group
# Begin Source File

SOURCE=\usr\include\mad.h
# End Source File
# Begin Source File

SOURCE=\usr\include\limits.h
# End Source File
# Begin Group "id3 Headers"

# Begin Source File

SOURCE=\usr\include\id3\sized_types.h
# End Source File
# Begin Source File

SOURCE=\usr\include\id3\globals.h
# End Source File
# Begin Source File

SOURCE=\usr\include\id3\id3lib_frame.h
# End Source File
# Begin Source File

SOURCE=\usr\include\id3\field.h
# End Source File
# Begin Source File

SOURCE=\usr\include\id3\tag.h
# End Source File
# End Group
# End Group
# End Group
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
# Begin Group "Standard Headers"

# Begin Source File

SOURCE=..\..\..\src\Standard\Array.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Standard\FastRounding.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Standard\Text.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Standard\Filename.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Standard\Enum.hxx
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

SOURCE=..\..\..\src\Data\BasicProcessing\Audio.hxx
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
# End Group
# Begin Group "Ports Headers"

# Begin Source File

SOURCE=..\..\..\src\Flow\Ports\Port.hxx
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

SOURCE=..\..\..\src\Flow\Nodes\InplaceStreamRegion.hxx
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
# Begin Group "AudioFileIO Headers"

# Begin Source File

SOURCE=..\..\..\src\Processing\AudioFileIO\MultiChannelAudioFileReaderConfig.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Processing\AudioFileIO\MultiChannelAudioFileReader.hxx
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
# Begin Group "Linux Headers"

# Begin Source File

SOURCE=..\..\..\src\Tools\AudioIO\Linux\SndPcm.hxx
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\..\src\Tools\AudioIO\AudioDeviceList.hxx
# End Source File
# Begin Group "RtAudio Headers"

# Begin Source File

SOURCE=..\..\..\src\Tools\AudioIO\RtAudio\RtAudio.hxx
# End Source File
# End Group
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
# End Group
# Begin Group "Qt .ui Files"

# End Group
# Begin Source File

SOURCE=settings.cfg

!IF  "$(CFG)" == "FilePlayback - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build
InputPath=settings.cfg

"buildstamp" : $(SOURCE) "$(INTDIR)"
	srcdeps.exe settings.cfg

# End Custom Build

!ELSEIF  "$(CFG)" == "FilePlayback - Win32 Debug"

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
