# Microsoft Developer Studio Project File - Name="MagnitudeFR" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=MagnitudeFR - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "MagnitudeFR.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "MagnitudeFR.mak" CFG="MagnitudeFR - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "MagnitudeFR - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "MagnitudeFR - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "CLAM_Skeleton_project"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "MagnitudeFR - Win32 Release"

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
# ADD CPP /nologo /MD /W3 /GR /GX /O2 /FD /c /Zm1000 /D "HAVE_STANDARD_SSTREAM" /D "HAVE_STANDARD_SSTREAM_STR" /D "HAVE_STANDARD_VECTOR_AT" /D "WIN32" /D "_MBCS" /D "_CONSOLE" /D "CLAM_FLOAT" /D "CLAM_USE_XML" /I "..\..\..\src\Defines" /FI"preinclude.hxx" /I "..\..\..\src\Storage\XML" /I "..\..\..\src\Storage\Base" /I "..\..\..\src\Standard" /I "..\..\..\src\Processing\Base" /I "..\..\..\src\Flow\Ports" /I "..\..\..\src\Flow\Controls" /I "..\..\..\src\Errors" /I "..\..\..\src\Defines" /I "..\..\..\src\Data\Base" /I "..\..\..\src\Base" /I "..\..\..\..\xercesc\include"
# ADD BASE RSC /l 0xc0a /d "NDEBUG"
# ADD RSC /l 0xc0a /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 /libpath:"..\..\..\..\xercesc\lib" /libpath:"..\..\..\..\pthreads\lib" xerces-c_1.lib pthreadVC.lib /nologo /subsystem:console /machine:I386
# SUBTRACT LINK32 /nologo /verbose /pdb:none /debug

!ELSEIF  "$(CFG)" == "MagnitudeFR - Win32 Debug"

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
# ADD CPP /nologo /MDd /W3 /GR /GX /ZI /Od /D "_DEBUG" /FD /GZ /Zm1000 /c /D "HAVE_STANDARD_SSTREAM" /D "HAVE_STANDARD_SSTREAM_STR" /D "HAVE_STANDARD_VECTOR_AT" /D "WIN32" /D "_MBCS" /D "_CONSOLE" /D "CLAM_FLOAT" /D "CLAM_USE_XML" /I "..\..\..\src\Defines" /FI"preinclude.hxx" /I "..\..\..\src\Storage\XML" /I "..\..\..\src\Storage\Base" /I "..\..\..\src\Standard" /I "..\..\..\src\Processing\Base" /I "..\..\..\src\Flow\Ports" /I "..\..\..\src\Flow\Controls" /I "..\..\..\src\Errors" /I "..\..\..\src\Defines" /I "..\..\..\src\Data\Base" /I "..\..\..\src\Base" /I "..\..\..\..\xercesc\include"
# ADD BASE RSC /l 0xc0a /d "_DEBUG"
# ADD RSC /l 0xc0a /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /libpath:"..\..\..\..\xercesc\lib" /libpath:"..\..\..\..\pthreads\lib" xerces-c_1d.lib pthreadVC.lib /nologo /subsystem:console /debug /machine:I386
# SUBTRACT LINK32 /nologo /verbose /pdb:none

!ENDIF 

# Begin Target

# Name "MagnitudeFR - Win32 Release"
# Name "MagnitudeFR - Win32 Debug"
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Group "Source Files"

# Begin Group "draft Sources"

# Begin Group "TDFilterGenerator Sources"

# Begin Source File

SOURCE=..\..\..\draft\TDFilterGenerator\MagnitudeFR_test.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\draft\TDFilterGenerator\PolynomialRootsFinder.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\draft\TDFilterGenerator\Polynomial.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\draft\TDFilterGenerator\TDFilterKernel.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\draft\TDFilterGenerator\TDFilterGen.cxx
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
# Begin Source File

SOURCE=..\..\..\src\Storage\XML\XMLStaticAdapter.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Storage\XML\XercesDomPrinter.cxx
# End Source File
# End Group
# End Group
# Begin Group "Base Sources"

# Begin Source File

SOURCE=..\..\..\src\Base\DynamicType.cxx
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
# End Group
# Begin Group "Standard Sources"

# Begin Source File

SOURCE=..\..\..\src\Standard\Enum.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Standard\Complex.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Standard\Polar.cxx
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

SOURCE=..\..\..\src\Flow\Ports\OutPort.hxx
# End Source File
# End Group
# End Group
# Begin Group "Standard Headers"

# Begin Source File

SOURCE=..\..\..\src\Standard\Array.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Standard\Enum.hxx
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
# End Group
# Begin Group "draft Headers"

# Begin Group "TDFilterGenerator Headers"

# Begin Source File

SOURCE=..\..\..\draft\TDFilterGenerator\PolynomialRootsFinder.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\draft\TDFilterGenerator\Polynomial.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\draft\TDFilterGenerator\TDFilterKernel.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\draft\TDFilterGenerator\TDFilterGen.hxx
# End Source File
# End Group
# End Group
# Begin Group "xercesc Headers"

# Begin Group "include Headers"

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

!IF  "$(CFG)" == "MagnitudeFR - Win32 Release"

# Begin Custom Build
InputPath=settings.cfg

"buildstamp" : $(SOURCE) "$(INTDIR)" "MagnitudeFR.dsp"
	srcdeps.exe settings.cfg MagnitudeFR.dsp

# End Custom Build

!ELSEIF  "$(CFG)" == "MagnitudeFR - Win32 Debug"

# Begin Custom Build
InputPath=settings.cfg

"buildstamp" : $(SOURCE) "$(INTDIR)" "MagnitudeFR.dsp"
	srcdeps.exe settings.cfg MagnitudeFR.dsp

# End Custom Build

!ENDIF 

# End Source File
# End Target
# End Project
