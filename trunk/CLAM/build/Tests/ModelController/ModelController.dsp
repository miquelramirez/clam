# Microsoft Developer Studio Project File - Name="ModelControllerTest" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=ModelControllerTest - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ModelController.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ModelController.mak" CFG="ModelControllerTest - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ModelControllerTest - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "ModelControllerTest - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "CLAM_Skeleton_project"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ModelControllerTest - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ""
# PROP Intermediate_Dir ""
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GR /GX /O2 /I "..\..\..\src\Defines" /I "..\..\..\src\Errors" /I "..\..\..\src\Storage\Base" /I "..\..\..\src\Base" /I "..\..\..\src\Standard" /I "..\..\..\src\Flow\Controls" /I "..\..\..\src\Processing\Base" /I "..\..\..\test\Visualization\TestModelController\Stubs" /I "..\..\..\src\Visualization\Base" /I "..\..\..\externals\CbLib" /I "..\..\..\src\Visualization\View" /I "..\..\..\src\Flow\Ports" /I "..\..\..\..\pthreads\include" /I "..\..\..\src\Defines\Windows" /I "..\..\..\src\System\Threads" /FI"preinclude.hxx" /D "HAVE_STANDARD_SSTREAM" /D "HAVE_STANDARD_SSTREAM_STR" /D "HAVE_STANDARD_VECTOR_AT" /D "WIN32" /D "_MBCS" /D "_CONSOLE" /D "CLAM_FLOAT" /FD /c
# ADD BASE RSC /l 0xc0a /d "NDEBUG"
# ADD RSC /l 0xc0a /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 pthreadVC.lib /subsystem:console /machine:I386 /libpath:"..\..\..\..\pthreads\lib"
# SUBTRACT LINK32 /nologo /verbose /pdb:none /debug

!ELSEIF  "$(CFG)" == "ModelControllerTest - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir ""
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /GR /GX /ZI /Od /I "..\..\..\src\Defines" /I "..\..\..\src\Errors" /I "..\..\..\src\Storage\Base" /I "..\..\..\src\Base" /I "..\..\..\src\Standard" /I "..\..\..\src\Flow\Controls" /I "..\..\..\src\Processing\Base" /I "..\..\..\test\Visualization\TestModelController\Stubs" /I "..\..\..\src\Visualization\Base" /I "..\..\..\externals\CbLib" /I "..\..\..\src\Visualization\View" /I "..\..\..\src\Flow\Ports" /I "..\..\..\..\pthreads\include" /I "..\..\..\src\Defines\Windows" /I "..\..\..\src\System\Threads" /FI"preinclude.hxx" /D "_DEBUG" /D "HAVE_STANDARD_SSTREAM" /D "HAVE_STANDARD_SSTREAM_STR" /D "HAVE_STANDARD_VECTOR_AT" /D "WIN32" /D "_MBCS" /D "_CONSOLE" /D "CLAM_FLOAT" /FD /GZ /Zm1000 /c
# ADD BASE RSC /l 0xc0a /d "_DEBUG"
# ADD RSC /l 0xc0a /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 pthreadVC.lib /subsystem:console /machine:I386 /libpath:"..\..\..\..\pthreads\lib"
# SUBTRACT LINK32 /nologo /verbose /pdb:none

!ENDIF 

# Begin Target

# Name "ModelControllerTest - Win32 Release"
# Name "ModelControllerTest - Win32 Debug"
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Group "Source Files"

# PROP Default_Filter ""
# Begin Group "test Sources"

# PROP Default_Filter ""
# Begin Group "Visualization Sources"

# PROP Default_Filter ""
# Begin Group "TestModelController Sources"

# PROP Default_Filter ""
# Begin Group "TestSources Sources"

# PROP Default_Filter ""
# Begin Group "ModelController Sources"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\test\Visualization\TestModelController\TestSources\ModelController\TestCases.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\test\Visualization\TestModelController\TestSources\ModelController\TestUnit.cxx
# End Source File
# End Group
# End Group
# Begin Group "Stubs Sources"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\test\Visualization\TestModelController\Stubs\FakeCompositeControlWidget.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\test\Visualization\TestModelController\Stubs\FakeControlWidget.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\test\Visualization\TestModelController\Stubs\ProcFeatSeveralControls.cxx
# End Source File
# End Group
# End Group
# End Group
# End Group
# Begin Group "Errors Sources"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\src\Errors\Err.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Errors\ErrDynamicType.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Errors\ErrOutOfMemory.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Errors\ErrProcessingObj.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Errors\ErrSystem.cxx
# End Source File
# End Group
# Begin Group "Defines Sources"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\src\Defines\Assert.cxx
# End Source File
# End Group
# Begin Group "Base Sources"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\src\Base\DynamicType.cxx
# End Source File
# End Group
# Begin Group "Flow Sources"

# PROP Default_Filter ""
# Begin Group "Controls Sources"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\src\Flow\Controls\InControl.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Flow\Controls\OutControl.cxx
# End Source File
# End Group
# Begin Group "Ports Sources"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\src\Flow\Ports\Port.cxx
# End Source File
# End Group
# End Group
# Begin Group "Processing Sources"

# PROP Default_Filter ""
# Begin Group "Base Sources No. 1"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\src\Processing\Base\Processing.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Processing\Base\ProcessingComposite.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Processing\Base\PublishedInControls.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Processing\Base\PublishedInPorts.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Processing\Base\PublishedOutControls.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Processing\Base\PublishedOutPorts.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Processing\Base\TopLevelProcessing.cxx
# End Source File
# End Group
# End Group
# Begin Group "Visualization Sources No. 1"

# PROP Default_Filter ""
# Begin Group "Base Sources No. 2"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\src\Visualization\Base\Connection.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\Base\ModelAdapter.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\Base\ModelController.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\Base\Presentation.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\Base\Signal.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\Base\Slot.cxx
# End Source File
# End Group
# Begin Group "View Sources"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\src\Visualization\View\ControlAdapter.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\View\InputControlModel.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\View\ProcessingController.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\View\ProcessingInControlSet.cxx
# End Source File
# End Group
# End Group
# Begin Group "System Sources"

# PROP Default_Filter ""
# Begin Group "Threads Sources"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\src\System\Threads\Mutex.cxx
# End Source File
# End Group
# End Group
# End Group
# Begin Group "Header Files"

# PROP Default_Filter ""
# Begin Group "Errors Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\src\Errors\Err.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Errors\ErrDynamicType.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Errors\ErrOutOfMemory.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Errors\ErrProcessingObj.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Errors\ErrSystem.hxx
# End Source File
# End Group
# Begin Group "Defines Headers"

# PROP Default_Filter ""
# Begin Group "Windows Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\src\Defines\Windows\CLAM_windows.h
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\..\src\Defines\Assert.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Defines\ByteOrder.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Defines\CLAM_Math.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Defines\DataTypes.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Defines\EDataFormat.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Defines\mtgsstream.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Defines\OSDefines.hxx
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
# End Group
# Begin Group "test Headers"

# PROP Default_Filter ""
# Begin Group "Visualization Headers"

# PROP Default_Filter ""
# Begin Group "TestModelController Headers"

# PROP Default_Filter ""
# Begin Group "TestSources Headers"

# PROP Default_Filter ""
# Begin Group "ModelController Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\test\Visualization\TestModelController\TestSources\ModelController\TestCases.hxx
# End Source File
# End Group
# End Group
# Begin Group "Stubs Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\test\Visualization\TestModelController\Stubs\FakeCompositeControlWidget.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\test\Visualization\TestModelController\Stubs\FakeControlWidget.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\test\Visualization\TestModelController\Stubs\ProcFeatSeveralControls.hxx
# End Source File
# End Group
# End Group
# End Group
# End Group
# Begin Group "Base Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\src\Base\Component.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Base\DynamicType.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Base\DynamicTypeMacros.hxx
# End Source File
# End Group
# Begin Group "Storage Headers"

# PROP Default_Filter ""
# Begin Group "Base Headers No. 1"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\src\Storage\Base\Storable.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Storage\Base\Storage.hxx
# End Source File
# End Group
# End Group
# Begin Group "Flow Headers"

# PROP Default_Filter ""
# Begin Group "Controls Headers"

# PROP Default_Filter ""
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

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\src\Flow\Ports\InPort.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Flow\Ports\OutPort.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Flow\Ports\Port.hxx
# End Source File
# End Group
# End Group
# Begin Group "Standard Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\src\Standard\Array.hxx
# End Source File
# End Group
# Begin Group "Processing Headers"

# PROP Default_Filter ""
# Begin Group "Base Headers No. 2"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\src\Processing\Base\Processing.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Processing\Base\ProcessingComposite.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Processing\Base\PublishedInControls.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Processing\Base\PublishedInPorts.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Processing\Base\PublishedOutControls.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Processing\Base\PublishedOutPorts.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Processing\Base\TopLevelProcessing.hxx
# End Source File
# End Group
# End Group
# Begin Group "Visualization Headers No. 1"

# PROP Default_Filter ""
# Begin Group "Base Headers No. 3"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\src\Visualization\Base\Connection.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\Base\ConnectionHandler.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\Base\ModelAdapter.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\Base\ModelController.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\Base\Presentation.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\Base\Signal.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\Base\Signalv1.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\Base\Signalv1ImplSerious.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\Base\Signalv2.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\Base\Signalv2ImplSerious.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\Base\Slot.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\Base\Slotv1.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\Base\Slotv2.hxx
# End Source File
# End Group
# Begin Group "View Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\src\Visualization\View\ControlAdapter.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\View\InputControlModel.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\View\ProcessingController.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\View\ProcessingInControlSet.hxx
# End Source File
# End Group
# End Group
# Begin Group "externals Headers"

# PROP Default_Filter ""
# Begin Group "CbLib Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\externals\CbLib\CBL.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\externals\CbLib\CBLMacros.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\externals\CbLib\Functor0.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\externals\CbLib\Functor0R.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\externals\CbLib\Functor1.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\externals\CbLib\Functor1R.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\externals\CbLib\Functor2.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\externals\CbLib\Functor2R.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\externals\CbLib\Functor3.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\externals\CbLib\Functor3R.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\externals\CbLib\Functor4.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\externals\CbLib\Functor4R.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\externals\CbLib\FunctorBase.hxx
# End Source File
# End Group
# End Group
# Begin Group "pthreads Headers"

# PROP Default_Filter ""
# Begin Group "include Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\pthreads\include\pthread.h
# End Source File
# End Group
# End Group
# Begin Group "System Headers"

# PROP Default_Filter ""
# Begin Group "Threads Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\src\System\Threads\Lock.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\System\Threads\Mutex.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\System\Threads\xtime.hxx
# End Source File
# End Group
# End Group
# End Group
# Begin Source File

SOURCE=settings.cfg

!IF  "$(CFG)" == "ModelControllerTest - Win32 Release"

# Begin Custom Build
InputPath=settings.cfg

"buildstamp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	srcdeps.exe settings.cfg ModelControllerTest.dsp

# End Custom Build

!ELSEIF  "$(CFG)" == "ModelControllerTest - Win32 Debug"

# Begin Custom Build
InputPath=settings.cfg

"buildstamp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	srcdeps.exe settings.cfg ModelControllerTest.dsp

# End Custom Build

!ENDIF 

# End Source File
# End Target
# End Project
