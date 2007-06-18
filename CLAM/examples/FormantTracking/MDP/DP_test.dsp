# Microsoft Developer Studio Project File - Name="DP_test" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=DP_test - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "DP_test.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "DP_test.mak" CFG="DP_test - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "DP_test - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "DP_test - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "DP_test - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /MD /W3 /GR /GX /Ob2 /I "../../src/Base" /I "../../src/Data" /I "../../src/Data/Base" /I "../../src/Data/BasicProcessing" /I "../../src/Data/Descriptors" /I "../../src/Defines" /I "../../src/Defines/Windows" /I "../../src/Errors" /I "../../src/Flow" /I "../../src/Flow/Controls" /I "../../src/Flow/Nodes" /I "../../src/Flow/Ports" /I "../../src/Processing" /I "../../src/Processing/Analysis" /I "../../src/Processing/ArithOps" /I "../../src/Processing/AudioFileIO" /I "../../src/Processing/AudioIO" /I "../../src/Processing/Base" /I "../../src/Processing/Controls" /I "../../src/Processing/Generators" /I "../../src/Processing/MIDIIO" /I "../../src/Processing/Synthesis" /I "../../src/Processing/Transformations" /I "../../src/Standard" /I "../../src/Storage" /I "../../src/Storage/Base" /I "../../src/Storage/XML" /I "../../src/System/Application" /I "../../src/System/Threads" /I "../../src/Tools" /I "../../src/Tools/AudioFileIO" /I "../../src/Tools/AudioIO" /I "../../src/Tools/MIDIIO" /I "../../src/Visualization" /I "../../src/Visualization/Base" /I "../../src/Visualization/GL" /I "../../src/Visualization/Presentation" /I "../../src/Visualization/Services" /I "../../src/Visualization/SysInt" /I "../../src/Visualization/Util" /I "../../src/Visualization/View" /I "../../src/Visualization/Widget" /I "../../externals/CbLib" /I "../../../fftw/include" /I "../../../dxsdk/include" /I "../../../" /I "../../../xercesc/include" /I "../../../fltk/include" /I "../../../portmidi" /I "../../../include/pthreads" /FI"config.h" /D "NDEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D "CLAM_USE_XML" /Fo"..\ReleaseFiles\obj/" /Fd"..\ReleaseFiles\obj/" /FD /GZ /c
# ADD BASE RSC /l 0xc0a /d "NDEBUG"
# ADD RSC /l 0xc0a /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib xerces-c_1D.lib rfftw2st.lib fftw2st.lib fltkd.lib wsock32.lib opengl32.lib glu32.lib dsound.lib dxerr8.lib dxguid.lib pthreadVCE.lib /subsystem:console /pdb:"..\ReleaseFiles\bin/CLAM_Skeleton_project.pdb" /machine:I386 /out:"..\ReleaseFiles\bin/CLAM_Skeleton_project.exe" /libpath:"../../../xercesc/lib" /libpath:"../../../fftw/lib" /libpath:"../../../fltk/lib" /libpath:"../../../dxsdk/lib" /libpath:"../../../pthreads/lib"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "DP_test - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "DP_test___Win32_Debug"
# PROP BASE Intermediate_Dir "DP_test___Win32_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "DP_test___Win32_Debug"
# PROP Intermediate_Dir "DP_test___Win32_Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /MDd /W3 /GR /GX /ZI /Od /I "../../src/Base" /I "../../src/Data" /I "../../src/Data/Base" /I "../../src/Data/BasicProcessing" /I "../../src/Data/Descriptors" /I "../../src/Defines" /I "../../src/Defines/Windows" /I "../../src/Errors" /I "../../src/Flow" /I "../../src/Flow/Controls" /I "../../src/Flow/Nodes" /I "../../src/Flow/Ports" /I "../../src/Processing" /I "../../src/Processing/Analysis" /I "../../src/Processing/ArithOps" /I "../../src/Processing/AudioFileIO" /I "../../src/Processing/AudioIO" /I "../../src/Processing/Base" /I "../../src/Processing/Controls" /I "../../src/Processing/Generators" /I "../../src/Processing/MIDIIO" /I "../../src/Processing/Synthesis" /I "../../src/Processing/Transformations" /I "../../src/Standard" /I "../../src/Storage" /I "../../src/Storage/Base" /I "../../src/Storage/XML" /I "../../src/System/Application" /I "../../src/System/Threads" /I "../../src/Tools" /I "../../src/Tools/AudioFileIO" /I "../../src/Tools/AudioIO" /I "../../src/Tools/MIDIIO" /I "../../src/Visualization" /I "../../src/Visualization/Base" /I "../../src/Visualization/GL" /I "../../src/Visualization/Presentation" /I "../../src/Visualization/Services" /I "../../src/Visualization/SysInt" /I "../../src/Visualization/Util" /I "../../src/Visualization/View" /I "../../src/Visualization/Widget" /I "../../externals/CbLib" /I "../../../fftw/include" /I "../../../dxsdk/include" /I "../../../" /I "../../../xercesc/include" /I "../../../fltk/include" /I "../../../portmidi" /I "../../../include/pthreads" /FI"config.h" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D "CLAM_USE_XML" /Fo"..\DebugFiles\obj/" /Fd"..\DebugFiles\obj/" /FD /GZ /c
# ADD BASE RSC /l 0xc0a /d "_DEBUG"
# ADD RSC /l 0xc0a /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib xerces-c_1D.lib rfftw2st.lib fftw2st.lib fltkd.lib wsock32.lib opengl32.lib glu32.lib dsound.lib dxerr8.lib dxguid.lib pthreadVCE.lib /subsystem:console /incremental:no /pdb:"..\DebugFiles\bin/CLAM_Skeleton_project.pdb" /debug /machine:I386 /out:"..\DebugFiles\bin/CLAM_Skeleton_project.exe" /libpath:"../../../xercesc/lib" /libpath:"../../../fftw/lib" /libpath:"../../../fltk/lib" /libpath:"../../../dxsdk/lib" /libpath:"../../../pthreads/lib"
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "DP_test - Win32 Release"
# Name "DP_test - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "CLAM Sources"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\src\Defines\Assert.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Storage\XML\BasicXMLable.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Base\DynamicType.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Errors\Err.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Errors\ErrDynamicType.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Errors\ErrOutOfMemory.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Errors\ErrProcessingObj.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Standard\Matrix.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Flow\Controls\OutControl.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Processing\Base\Processing.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Processing\Base\ProcessingComposite.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Processing\Base\PublishedInControls.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Processing\Base\PublishedInPorts.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Processing\Base\PublishedOutControls.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Processing\Base\PublishedOutPorts.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Processing\Base\TopLevelProcessing.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Storage\XML\XercesDomPrinter.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Storage\XML\XMLStorage.cxx
# End Source File
# End Group
# Begin Group "MDP Sources"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\MDP.cxx
# End Source File
# Begin Source File

SOURCE=.\MDPModeler.cxx
# End Source File
# End Group
# Begin Group "DP Sources"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\TrackFinder.cxx
# End Source File
# End Group
# Begin Source File

SOURCE=.\DP_test.cxx
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Group "CLAM Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\src\Standard\MatrixTmpl.hxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Standard\MatrixTmplDec.hxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Standard\MatrixTmplDef.hxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Processing\Base\PublishedInControls.hxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Processing\Base\PublishedInPorts.hxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Processing\Base\PublishedOutControls.hxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Processing\Base\PublishedOutPorts.hxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Storage\Xml\XMLStorage.hxx
# End Source File
# End Group
# Begin Group "MPD Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\MDP.hxx
# End Source File
# Begin Source File

SOURCE=.\MDPModeler.hxx
# End Source File
# End Group
# Begin Group "FormantExtractor Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Candidates.hxx
# End Source File
# End Group
# Begin Group "DP Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\TrackFinder.hxx
# End Source File
# End Group
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
