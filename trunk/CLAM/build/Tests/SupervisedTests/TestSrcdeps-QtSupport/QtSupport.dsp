# Microsoft Developer Studio Project File - Name="QtSupport" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=QtSupport - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "QtSupport.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "QtSupport.mak" CFG="QtSupport - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "QtSupport - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "QtSupport - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "empty"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "QtSupport - Win32 Release"

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
# ADD CPP /nologo /MD /W3 /GR /GX /O2 /FD /c /Zm1000 /D "NDEBUG" /D "HAVE_STANDARD_SSTREAM" /D "HAVE_STANDARD_SSTREAM_STR" /D "HAVE_STANDARD_VECTOR_AT" /D "HAVE_STANDARD_UNICODE" /D "WIN32" /D "_MBCS" /D "_CONSOLE" /D "CLAM_FLOAT" /D "CLAM_USE_XML" /D "ID3LIB_LINKOPTION=1" /D "QT_DLL" /D "QT_THREAD_SUPPORT" /D "VISUAL" /I "..\..\..\src\Defines" /FI"preinclude.hxx" /I ".\uic"
# ADD BASE RSC /l 0xc0a /d "NDEBUG"
# ADD RSC /l 0xc0a /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32
# ADD LINK32 /libpath:"..\..\..\..\fftw\lib" /libpath:"..\..\..\..\xercesc\lib" /libpath:"..\..\..\..\pthreads\lib" /libpath:"..\..\..\..\libsndfile\lib" /libpath:"..\..\..\..\oggvorbis\lib" /libpath:"..\..\..\..\libmad\lib" /libpath:"..\..\..\..\id3lib\lib" /libpath:"C:\apps\Qt\3.2.2\lib" RFFTW2st.lib FFTW2st.lib xerces-c_2.lib pthreadVC.lib libsndfile.lib ogg_static.lib vorbis_static.lib vorbisenc_static.lib vorbisfile_static.lib id3lib.lib libmad.lib C:\apps\Qt\3.2.2/lib/qt-mt322.lib C:\apps\Qt\3.2.2/lib/qtmain.lib opengl32.lib glu32.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib imm32.lib winmm.lib wsock32.lib winspool.lib delayimp.lib /nologo /subsystem:console /NODEFAULTLIB:MSVCRTD /machine:I386
# SUBTRACT LINK32 /nologo /verbose /pdb:none /debug

!ELSEIF  "$(CFG)" == "QtSupport - Win32 Debug"

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
# ADD CPP /nologo /MDd /W3 /GR /GX /ZI /Od /D "_DEBUG" /FD /GZ /Zm1000 /c /D "HAVE_STANDARD_SSTREAM" /D "HAVE_STANDARD_SSTREAM_STR" /D "HAVE_STANDARD_VECTOR_AT" /D "HAVE_STANDARD_UNICODE" /D "WIN32" /D "_MBCS" /D "_CONSOLE" /D "CLAM_FLOAT" /D "CLAM_USE_XML" /D "ID3LIB_LINKOPTION=1" /D "QT_DLL" /D "QT_THREAD_SUPPORT" /D "VISUAL" /I "..\..\..\src\Defines" /FI"preinclude.hxx" /I ".\uic"
# ADD BASE RSC /l 0xc0a /d "_DEBUG"
# ADD RSC /l 0xc0a
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32
# ADD LINK32 /libpath:"..\..\..\..\fftw\lib" /libpath:"..\..\..\..\xercesc\lib" /libpath:"..\..\..\..\pthreads\lib" /libpath:"..\..\..\..\libsndfile\lib" /libpath:"..\..\..\..\oggvorbis\lib" /libpath:"..\..\..\..\libmad\lib" /libpath:"..\..\..\..\id3lib\lib" /libpath:"C:\apps\Qt\3.2.2\lib" FFTW2std.lib RFFTW2std.lib xerces-c_2d.lib pthreadVC.lib libsndfiled.lib ogg_static_d.lib vorbis_static_d.lib vorbisenc_static_d.lib vorbisfile_static_d.lib id3libd.lib libmadd.lib C:\apps\Qt\3.2.2/lib/qt-mt322.lib C:\apps\Qt\3.2.2/lib/qtmain.lib opengl32.lib glu32.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib imm32.lib winmm.lib wsock32.lib winspool.lib delayimp.lib /nologo /subsystem:console /NODEFAULTLIB:MSVCRT /debug /machine:I386
# SUBTRACT LINK32 /nologo /verbose /pdb:none

!ENDIF 

# Begin Target

# Name "QtSupport - Win32 Release"
# Name "QtSupport - Win32 Debug"
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Group "Source Files"

# Begin Group ". Sources"

# Begin Source File

SOURCE=.\main.cxx
# End Source File
# Begin Group "uic Sources"

# Begin Source File

SOURCE=.\uic\TestControlPanel.cxx
# End Source File
# End Group
# Begin Group "moc Sources"

# Begin Source File

SOURCE=.\moc\TestControlPanel_moc.cxx
# End Source File
# End Group
# End Group
# End Group
# Begin Group "Header Files"

# Begin Group ". Headers"

# Begin Group "uic Headers"

# Begin Source File

SOURCE=.\uic\TestControlPanel.h
# End Source File
# End Group
# End Group
# End Group
# Begin Group "Qt .ui Files"

# Begin Group ". Qt Files"

#Begin Source File

SOURCE=".\TestControlPanel.ui"

!IF "$(CFG)" == "QtSupport - Win32 Release "

# PROP Ignore_Default_Tool 1
# Begin Custom Build - UICing TestControlPanel.ui...

BuildCmds= \
	C:\apps\Qt\3.2.2\bin\uic.exe .\TestControlPanel.ui -o .\uic\TestControlPanel.h \
	C:\apps\Qt\3.2.2\bin\uic.exe .\TestControlPanel.ui -i TestControlPanel.h -o .\uic\TestControlPanel.cxx \
	C:\apps\Qt\3.2.2\bin\moc.exe .\uic\TestControlPanel.h -o .\moc\TestControlPanel_moc.cxx \


".\uic\TestControlPanel.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
".\uic\TestControlPanel.cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
".\moc\TestControlPanel_moc.cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

# End Custom Build 

!ELSEIF "$(CFG)" == "QtSupport - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - UICing TestControlPanel.ui...

BuildCmds= \
	C:\apps\Qt\3.2.2\bin\uic.exe .\TestControlPanel.ui -o .\uic\TestControlPanel.h \
	C:\apps\Qt\3.2.2\bin\uic.exe .\TestControlPanel.ui -i TestControlPanel.h -o .\uic\TestControlPanel.cxx \
	C:\apps\Qt\3.2.2\bin\moc.exe .\uic\TestControlPanel.h -o .\moc\TestControlPanel_moc.cxx \


".\uic\TestControlPanel.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
".\uic\TestControlPanel.cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
".\moc\TestControlPanel_moc.cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

# End Custom Build 

!ENDIF

#End Source File
# End Group
# End Group
# Begin Source File

SOURCE=settings.cfg

!IF  "$(CFG)" == "QtSupport - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build
InputPath=settings.cfg

"buildstamp" : $(SOURCE) "$(INTDIR)"
	srcdeps.exe settings.cfg

# End Custom Build

!ELSEIF  "$(CFG)" == "QtSupport - Win32 Debug"

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
