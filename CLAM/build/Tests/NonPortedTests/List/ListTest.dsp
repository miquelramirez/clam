# Microsoft Developer Studio Project File - Name="ListTest" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=ListTest - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ListTest.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ListTest.mak" CFG="ListTest - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ListTest - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "ListTest - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "CLAM_Skeleton_project"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ListTest - Win32 Release"

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
# ADD CPP /nologo /MD /W3 /GR /GX /O2 /FD /c /Zm1000 /D "NDEBUG" /D "HAVE_STANDARD_SSTREAM" /D "HAVE_STANDARD_SSTREAM_STR" /D "HAVE_STANDARD_VECTOR_AT" /D "WIN32" /D "_MBCS" /D "_CONSOLE" /D "CLAM_FLOAT" /I "..\..\..\src\Defines" /FI"preinclude.hxx" /I "..\..\..\..\cppunit\include" /I "..\..\..\test\UnitTests\CommonHelpers" /I "..\..\..\src\Base" /I "..\..\..\src\Defines" /I "..\..\..\src\Errors" /I "..\..\..\src\Standard" /I "..\..\..\src\Storage\Base"
# ADD BASE RSC /l 0xc0a /d "NDEBUG"
# ADD RSC /l 0xc0a /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 /libpath:"..\..\..\..\cppunit\lib" cppunit_vc6.lib /nologo /subsystem:console /machine:I386
# SUBTRACT LINK32 /nologo /verbose /pdb:none /debug

!ELSEIF  "$(CFG)" == "ListTest - Win32 Debug"

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
# ADD CPP /nologo /MDd /W3 /GR /GX /ZI /Od /D "_DEBUG" /FD /GZ /Zm1000 /c /D "HAVE_STANDARD_SSTREAM" /D "HAVE_STANDARD_SSTREAM_STR" /D "HAVE_STANDARD_VECTOR_AT" /D "WIN32" /D "_MBCS" /D "_CONSOLE" /D "CLAM_FLOAT" /I "..\..\..\src\Defines" /FI"preinclude.hxx" /I "..\..\..\..\cppunit\include" /I "..\..\..\test\UnitTests\CommonHelpers" /I "..\..\..\src\Base" /I "..\..\..\src\Defines" /I "..\..\..\src\Errors" /I "..\..\..\src\Standard" /I "..\..\..\src\Storage\Base"
# ADD BASE RSC /l 0xc0a /d "_DEBUG"
# ADD RSC /l 0xc0a /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /libpath:"..\..\..\..\cppunit\lib" cppunitd_vc6.lib /nologo /subsystem:console /debug /machine:I386
# SUBTRACT LINK32 /nologo /verbose /pdb:none

!ENDIF 

# Begin Target

# Name "ListTest - Win32 Release"
# Name "ListTest - Win32 Debug"
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Group "Source Files"

# Begin Group "test Sources"

# Begin Source File

SOURCE=..\..\..\test\ListTest.cxx
# End Source File
# End Group
# Begin Group "Errors Sources"

# Begin Source File

SOURCE=..\..\..\src\Errors\Err.cxx
# End Source File
# End Group
# Begin Group "Defines Sources"

# Begin Source File

SOURCE=..\..\..\src\Defines\Assert.cxx
# End Source File
# End Group
# End Group
# Begin Group "Header Files"

# Begin Group "Errors Headers"

# Begin Source File

SOURCE=..\..\..\src\Errors\Err.hxx
# End Source File
# End Group
# Begin Group "Defines Headers"

# Begin Source File

SOURCE=..\..\..\src\Defines\Assert.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Defines\CLAM_Math.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Defines\DataTypes.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Defines\StaticBool.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Defines\TypeInfo.hxx
# End Source File
# End Group
# Begin Group "Storage Headers"

# Begin Group "Base Headers"

# Begin Source File

SOURCE=..\..\..\src\Storage\Base\Storable.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Storage\Base\Storage.hxx
# End Source File
# End Group
# End Group
# Begin Group "Base Headers No. 1"

# Begin Source File

SOURCE=..\..\..\src\Base\Component.hxx
# End Source File
# End Group
# Begin Group "Standard Headers"

# Begin Source File

SOURCE=..\..\..\src\Standard\List.hxx
# End Source File
# End Group
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

SOURCE=..\..\..\..\cppunit\include\cppunit\extensions\TestSuiteFactory.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\cppunit\include\cppunit\extensions\TestFactoryRegistry.h
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

# Begin Group "CommonHelpers Headers"

# Begin Source File

SOURCE=..\..\..\test\UnitTests\CommonHelpers\XMLTestHelper.hxx
# End Source File
# End Group
# End Group
# End Group
# End Group
# Begin Source File

SOURCE=settings.cfg

!IF  "$(CFG)" == "ListTest - Win32 Release"

# Begin Custom Build
InputPath=settings.cfg

"buildstamp" : $(SOURCE) "$(INTDIR)" "ListTest.dsp"
	srcdeps.exe settings.cfg ListTest.dsp

# End Custom Build

!ELSEIF  "$(CFG)" == "ListTest - Win32 Debug"

# Begin Custom Build
InputPath=settings.cfg

"buildstamp" : $(SOURCE) "$(INTDIR)" "ListTest.dsp"
	srcdeps.exe settings.cfg ListTest.dsp

# End Custom Build

!ENDIF 

# End Source File
# End Target
# End Project
