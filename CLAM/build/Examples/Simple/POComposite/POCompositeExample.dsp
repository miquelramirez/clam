# Microsoft Developer Studio Project File - Name="POCompositeExample" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=POCompositeExample - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "POCompositeExample.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "POCompositeExample.mak" CFG="POCompositeExample - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "POCompositeExample - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "POCompositeExample - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "empty"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "POCompositeExample - Win32 Release"

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
# ADD CPP /nologo /MD /W3 /GR /GX /O2 /I "..\..\..\..\src\Defines" /I "..\..\..\..\..\fftw\include" /I "..\..\..\..\src\Base" /I "..\..\..\..\src\Data\Base" /I "..\..\..\..\src\Data\BasicProcessing" /I "..\..\..\..\src\Errors" /I "..\..\..\..\src\Flow\Controls" /I "..\..\..\..\src\Flow\Nodes" /I "..\..\..\..\src\Flow\Ports" /I "..\..\..\..\src\Processing\Analysis" /I "..\..\..\..\src\Processing\ArithOps" /I "..\..\..\..\src\Processing\Base" /I "..\..\..\..\src\Processing\Synthesis" /I "..\..\..\..\src\Standard" /I "..\..\..\..\src\Storage\Base" /I "..\..\..\..\src\Storage\XML" /FI"preinclude.hxx" /D "NDEBUG" /D "HAVE_STANDARD_SSTREAM" /D "HAVE_STANDARD_SSTREAM_STR" /D "HAVE_STANDARD_VECTOR_AT" /D "WIN32" /D "_MBCS" /D "_CONSOLE" /D "CLAM_FLOAT" /FD /Zm1000 /c
# ADD BASE RSC /l 0xc0a /d "NDEBUG"
# ADD RSC /l 0xc0a /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /machine:IX86
# ADD LINK32 RFFTW2st.lib FFTW2st.lib /subsystem:console /machine:I386 /libpath:"..\..\..\..\..\fftw\lib"
# SUBTRACT LINK32 /nologo /verbose /pdb:none /debug

!ELSEIF  "$(CFG)" == "POCompositeExample - Win32 Debug"

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
# ADD CPP /nologo /MDd /W3 /GR /GX /ZI /Od /I "..\..\..\..\src\Defines" /I "..\..\..\..\..\fftw\include" /I "..\..\..\..\src\Base" /I "..\..\..\..\src\Data\Base" /I "..\..\..\..\src\Data\BasicProcessing" /I "..\..\..\..\src\Errors" /I "..\..\..\..\src\Flow\Controls" /I "..\..\..\..\src\Flow\Nodes" /I "..\..\..\..\src\Flow\Ports" /I "..\..\..\..\src\Processing\Analysis" /I "..\..\..\..\src\Processing\ArithOps" /I "..\..\..\..\src\Processing\Base" /I "..\..\..\..\src\Processing\Synthesis" /I "..\..\..\..\src\Standard" /I "..\..\..\..\src\Storage\Base" /I "..\..\..\..\src\Storage\XML" /FI"preinclude.hxx" /D "_DEBUG" /D "HAVE_STANDARD_SSTREAM" /D "HAVE_STANDARD_SSTREAM_STR" /D "HAVE_STANDARD_VECTOR_AT" /D "WIN32" /D "_MBCS" /D "_CONSOLE" /D "CLAM_FLOAT" /FD /GZ /Zm1000 /c
# ADD BASE RSC /l 0xc0a /d "_DEBUG"
# ADD RSC /l 0xc0a
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /machine:IX86
# ADD LINK32 FFTW2std.lib RFFTW2std.lib /subsystem:console /debug /machine:I386 /libpath:"..\..\..\..\..\fftw\lib"
# SUBTRACT LINK32 /nologo /verbose /pdb:none

!ENDIF 

# Begin Target

# Name "POCompositeExample - Win32 Release"
# Name "POCompositeExample - Win32 Debug"
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Group "Source Files"

# PROP Default_Filter ""
# Begin Group "examples Sources"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\examples\POCompositeExample.cxx
# End Source File
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
# Begin Group "Analysis Sources"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\src\Processing\Analysis\FFT.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\Analysis\FFT_rfftw.cxx
# End Source File
# End Group
# Begin Group "Synthesis Sources"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\src\Processing\Synthesis\IFFT.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\Synthesis\IFFT_rfftw.cxx
# End Source File
# End Group
# Begin Group "ArithOps Sources"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\src\Processing\ArithOps\SpectrumProduct.cxx
# End Source File
# End Group
# End Group
# Begin Group "Data Sources"

# PROP Default_Filter ""
# Begin Group "BasicProcessing Sources"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\src\Data\BasicProcessing\Audio.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Data\BasicProcessing\Spectrum.cxx
# End Source File
# End Group
# End Group
# Begin Group "Standard Sources"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\src\Standard\BPF.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Standard\Complex.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Standard\Enum.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Standard\Flags.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Standard\GlobalEnums.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Standard\Point.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Standard\Polar.cxx
# End Source File
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

SOURCE=..\..\..\..\src\Storage\XML\XercesDomReadingContext.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Storage\XML\XercesDomWritingContext.hxx
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
# End Group
# Begin Group "Defines Headers"

# PROP Default_Filter ""
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

SOURCE=..\..\..\..\src\Standard\ArrayToBPFCnv.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Standard\BPF.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Standard\BPFTmpl.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Standard\BPFTmplDec.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Standard\BPFTmplDef.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Standard\Complex.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Standard\ComplexTmpl.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Standard\ComplexTmplDec.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Standard\ComplexTmplDef.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Standard\ComplexToPolarCnv.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Standard\Enum.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Standard\FastRounding.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Standard\Flags.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Standard\GlobalEnums.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Standard\Point.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Standard\PointTmpl.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Standard\PointTmplDec.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Standard\PointTmplDef.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Standard\Polar.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Standard\PolarTmpl.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Standard\PolarTmplDec.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Standard\PolarTmplDef.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Standard\SearchArray.hxx
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
# Begin Group "Analysis Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\src\Processing\Analysis\FFT.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\Analysis\FFT_rfftw.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\Analysis\FFTConfig.hxx
# End Source File
# End Group
# Begin Group "Synthesis Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\src\Processing\Synthesis\IFFT.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\Synthesis\IFFT_rfftw.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\Synthesis\IFFTConfig.hxx
# End Source File
# End Group
# Begin Group "ArithOps Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\src\Processing\ArithOps\SpectrumProduct.hxx
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
# Begin Group "BasicProcessing Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\src\Data\BasicProcessing\Audio.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Data\BasicProcessing\Spectrum.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Data\BasicProcessing\SpectrumConfig.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Data\BasicProcessing\SpecTypeFlags.hxx
# End Source File
# End Group
# End Group
# Begin Group "fftw Headers"

# PROP Default_Filter ""
# Begin Group "include Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\..\fftw\include\fftw.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\fftw\include\rfftw.h
# End Source File
# End Group
# End Group
# End Group
# Begin Source File

SOURCE=settings.cfg

!IF  "$(CFG)" == "POCompositeExample - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "POCompositeExample - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# End Target
# End Project
