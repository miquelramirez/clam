# Microsoft Developer Studio Project File - Name="LPC" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=LPC - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "LPC.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "LPC.mak" CFG="LPC - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "LPC - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "LPC - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "CLAM_Skeleton_project"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "LPC - Win32 Release"

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
# ADD CPP /nologo /MD /W3 /GR /GX /O2 /I "..\..\..\src\Defines" /I "..\..\..\externals\CbLib" /I "..\..\..\..\fftw\include" /I "..\..\..\..\fltk\include" /I "..\..\..\..\pthreads\include" /I "..\..\..\src\Visualization\Widget" /I "..\..\..\src\Visualization\Widget\FLTK" /I "..\..\..\src\Visualization\View" /I "..\..\..\src\Visualization\Util" /I "..\..\..\src\Visualization\Presentation\FLTK" /I "..\..\..\src\Visualization\Presentation\Base" /I "..\..\..\src\Visualization\GL" /I "..\..\..\src\Visualization\Base" /I "..\..\..\src\Visualization\Plot" /I "..\..\..\src\Tools\AudioIO" /I "..\..\..\src\Tools\AudioFileIO" /I "..\..\..\src\System\Threads" /I "..\..\..\src\Storage\XML" /I "..\..\..\src\Storage\Base" /I "..\..\..\src\Standard" /I "..\..\..\src\Processing\Synthesis" /I "..\..\..\src\Processing\Base" /I "..\..\..\src\Processing\AudioIO" /I "..\..\..\src\Processing\AudioFileIO" /I "..\..\..\src\Processing\ArithOps" /I "..\..\..\src\Processing\Analysis" /I "..\..\..\src\Flow\Ports" /I "..\..\..\src\Flow\Nodes" /I "..\..\..\src\Flow\Controls" /I "..\..\..\src\Errors" /I "..\..\..\src\Defines\Windows" /I "..\..\..\src\Data\BasicProcessing" /I "..\..\..\src\Data\Base" /I "..\..\..\src\Base" /I "..\..\..\..\xercesc\include" /FI"preinclude.hxx" /D "HAVE_STANDARD_SSTREAM" /D "HAVE_STANDARD_SSTREAM_STR" /D "HAVE_STANDARD_VECTOR_AT" /D "WIN32" /D "_MBCS" /D "_CONSOLE" /D "CLAM_DOUBLE" /D "CLAM_USE_XML" /FD /Zm1000 /c
# ADD BASE RSC /l 0xc0a /d "NDEBUG"
# ADD RSC /l 0xc0a /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 RFFTW2st.lib FFTW2st.lib fltk_1_1.lib fltkforms_1_1d.lib fltkgl_1_1d.lib fltkimages_1_1d.lib comctl32.lib kernel32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib wsock32.lib user32.lib gdi32.lib opengl32.lib glu32.lib xerces-c_1.lib pthreadVC.lib /subsystem:console /machine:I386 /libpath:"..\..\..\..\fftw\lib" /libpath:"..\..\..\..\fltk\lib" /libpath:"..\..\..\..\xercesc\lib" /libpath:"..\..\..\..\pthreads\lib"
# SUBTRACT LINK32 /nologo /verbose /pdb:none /debug

!ELSEIF  "$(CFG)" == "LPC - Win32 Debug"

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
# ADD CPP /nologo /MDd /W3 /GR /GX /ZI /Od /I "..\..\..\src\Defines" /I "..\..\..\externals\CbLib" /I "..\..\..\..\fftw\include" /I "..\..\..\..\fltk\include" /I "..\..\..\..\pthreads\include" /I "..\..\..\src\Visualization\Widget" /I "..\..\..\src\Visualization\Widget\FLTK" /I "..\..\..\src\Visualization\View" /I "..\..\..\src\Visualization\Util" /I "..\..\..\src\Visualization\Presentation\FLTK" /I "..\..\..\src\Visualization\Presentation\Base" /I "..\..\..\src\Visualization\GL" /I "..\..\..\src\Visualization\Base" /I "..\..\..\src\Visualization\Plot" /I "..\..\..\src\Tools\AudioIO" /I "..\..\..\src\Tools\AudioFileIO" /I "..\..\..\src\System\Threads" /I "..\..\..\src\Storage\XML" /I "..\..\..\src\Storage\Base" /I "..\..\..\src\Standard" /I "..\..\..\src\Processing\Synthesis" /I "..\..\..\src\Processing\Base" /I "..\..\..\src\Processing\AudioIO" /I "..\..\..\src\Processing\AudioFileIO" /I "..\..\..\src\Processing\ArithOps" /I "..\..\..\src\Processing\Analysis" /I "..\..\..\src\Flow\Ports" /I "..\..\..\src\Flow\Nodes" /I "..\..\..\src\Flow\Controls" /I "..\..\..\src\Errors" /I "..\..\..\src\Defines\Windows" /I "..\..\..\src\Data\BasicProcessing" /I "..\..\..\src\Data\Base" /I "..\..\..\src\Base" /I "..\..\..\..\xercesc\include" /FI"preinclude.hxx" /D "_DEBUG" /D "HAVE_STANDARD_SSTREAM" /D "HAVE_STANDARD_SSTREAM_STR" /D "HAVE_STANDARD_VECTOR_AT" /D "WIN32" /D "_MBCS" /D "_CONSOLE" /D "CLAM_DOUBLE" /D "CLAM_USE_XML" /FD /GZ /Zm1000 /c
# ADD BASE RSC /l 0xc0a /d "_DEBUG"
# ADD RSC /l 0xc0a /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 FFTW2std.lib RFFTW2std.lib fltk_1_1d.lib fltkforms_1_1d.lib fltkgl_1_1d.lib fltkimages_1_1d.lib comctl32.lib kernel32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib wsock32.lib user32.lib gdi32.lib opengl32.lib glu32.lib xerces-c_1d.lib pthreadVC.lib /subsystem:console /debug /machine:I386 /libpath:"..\..\..\..\fftw\lib" /libpath:"..\..\..\..\fltk\lib" /libpath:"..\..\..\..\xercesc\lib" /libpath:"..\..\..\..\pthreads\lib"
# SUBTRACT LINK32 /nologo /verbose /pdb:none

!ENDIF 

# Begin Target

# Name "LPC - Win32 Release"
# Name "LPC - Win32 Debug"
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Group "Source Files"

# PROP Default_Filter ""
# Begin Group "draft Sources"

# PROP Default_Filter ""
# Begin Group "LPC Sources"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\draft\LPC\AutocorrelationEvaluator.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\draft\LPC\Correlation.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\draft\LPC\Levinson_Durbin.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\draft\LPC\LPC.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\draft\LPC\LPC_Example.cxx
# End Source File
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

SOURCE=..\..\..\src\Errors\ErrGUI.cxx
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
# Begin Group "Storage Sources"

# PROP Default_Filter ""
# Begin Group "XML Sources"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\src\Storage\XML\BasicXMLable.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Storage\XML\XercesDomPrinter.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Storage\XML\XMLAdapter.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Storage\XML\XMLComponentAdapter.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Storage\XML\XMLStaticAdapter.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Storage\XML\XMLStorage.cxx
# End Source File
# End Group
# End Group
# Begin Group "Base Sources"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\src\Base\DynamicType.cxx
# End Source File
# End Group
# Begin Group "Data Sources"

# PROP Default_Filter ""
# Begin Group "BasicProcessing Sources"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\src\Data\BasicProcessing\Audio.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Data\BasicProcessing\Frame.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Data\BasicProcessing\Fundamental.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Data\BasicProcessing\Segment.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Data\BasicProcessing\SpectralPeak.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Data\BasicProcessing\SpectralPeakArray.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Data\BasicProcessing\Spectrum.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Data\BasicProcessing\SpectrumConversions.cxx
# End Source File
# End Group
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

SOURCE=..\..\..\src\Flow\Ports\InPort.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Flow\Ports\OutPort.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Flow\Ports\Port.cxx
# End Source File
# End Group
# Begin Group "Nodes Sources"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\src\Flow\Nodes\AddStreamRegion.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Flow\Nodes\DelayStreamRegion.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Flow\Nodes\InplaceStreamRegion.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Flow\Nodes\Node.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Flow\Nodes\ReadStreamRegion.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Flow\Nodes\SourceStreamRegion.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Flow\Nodes\StreamRegion.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Flow\Nodes\StreamRegionContainer.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Flow\Nodes\WriteStreamRegion.cxx
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
# Begin Group "AudioFileIO Sources"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\src\Processing\AudioFileIO\AudioFile.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Processing\AudioFileIO\AudioFileIn.cxx
# End Source File
# End Group
# Begin Group "AudioIO Sources"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\src\Processing\AudioIO\AudioIn.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Processing\AudioIO\AudioOut.cxx
# End Source File
# End Group
# Begin Group "Analysis Sources"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\src\Processing\Analysis\CircularShift.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Processing\Analysis\FFT.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Processing\Analysis\FFT_rfftw.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Processing\Analysis\SpectralAnalysis.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Processing\Analysis\WindowGenerator.cxx
# End Source File
# End Group
# Begin Group "ArithOps Sources"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\src\Processing\ArithOps\AudioMultiplier.cxx
# End Source File
# End Group
# Begin Group "Synthesis Sources"

# PROP Default_Filter ""
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
# End Group
# End Group
# Begin Group "Standard Sources"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\src\Standard\BPF.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Standard\Complex.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Standard\Enum.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Standard\Flags.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Standard\GlobalEnums.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Standard\Point.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Standard\Polar.cxx
# End Source File
# End Group
# Begin Group "Tools Sources"

# PROP Default_Filter ""
# Begin Group "AudioIO Sources No. 1"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\src\Tools\AudioIO\AudioDevice.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Tools\AudioIO\AudioDeviceList.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Tools\AudioIO\AudioManager.cxx
# End Source File
# End Group
# Begin Group "AudioFileIO Sources No. 1"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\src\Tools\AudioFileIO\AIFFFileIO.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Tools\AudioFileIO\extended.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Tools\AudioFileIO\SoundFileIO.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Tools\AudioFileIO\SoundHeader.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Tools\AudioFileIO\WaveFileIO.cxx
# End Source File
# End Group
# End Group
# Begin Group "Visualization Sources"

# PROP Default_Filter ""
# Begin Group "Plot Sources"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\src\Visualization\Plot\Plot.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\Plot\PlotAudio.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\Plot\PlotSinTracks.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\Plot\PlotSpectralPeaks.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\Plot\PlotSpectrum.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\Plot\Plotter.cxx
# End Source File
# End Group
# Begin Group "Base Sources No. 2"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\src\Visualization\Base\Connection.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\Base\DrawingSurface2D.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\Base\ModelAdapter.cxx
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
# Begin Source File

SOURCE=..\..\..\src\Visualization\Base\WidgetTKWrapper.cxx
# End Source File
# End Group
# Begin Group "View Sources"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\src\Visualization\View\AudioAdapter.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\View\AudioModel.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\View\LogMagSpectrumAdapter.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\View\SinTracksAdapter.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\View\SinTracksModel.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\View\SpectralPeakArrayAdapter.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\View\SpectralPeaksModel.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\View\SpectrumAdapter.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\View\SpectrumModel.cxx
# End Source File
# End Group
# Begin Group "Presentation Sources"

# PROP Default_Filter ""
# Begin Group "Base Sources No. 3"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\src\Visualization\Presentation\Base\AudioPresentation.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\Presentation\Base\SinTracksPresentation.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\Presentation\Base\SpectralPeaksPresentation.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\Presentation\Base\SpectrumPresentation.cxx
# End Source File
# End Group
# Begin Group "FLTK Sources"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\src\Visualization\Presentation\FLTK\Fl_Audio.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\Presentation\FLTK\Fl_SinTracks.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\Presentation\FLTK\Fl_Spectrum.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\Presentation\FLTK\Fl_SpectrumPeaks.cxx
# End Source File
# End Group
# End Group
# Begin Group "GL Sources"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\src\Visualization\GL\AudioRenderingManager.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\GL\GLMaxMinArrays.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\GL\GLPointsArrays.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\GL\GLRenderer.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\GL\GLRenderingManager.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\GL\GLSampleDetailArray.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\GL\GLSineTracks.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\GL\GLStraightLineArray.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\GL\PeaksRenderingManager.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\GL\SinTracksRenderingManager.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\GL\SpectrumRenderingManager.cxx
# End Source File
# End Group
# Begin Group "Util Sources"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\src\Visualization\Util\SinTrackHorClipper.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\Util\SinTrackListBuilder.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\Util\SinTrackVerClipper.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\Util\TimeFreqMagPoint.cxx
# End Source File
# End Group
# Begin Group "Widget Sources"

# PROP Default_Filter ""
# Begin Group "FLTK Sources No. 1"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\src\Visualization\Widget\FLTK\Fl_Axis.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\Widget\FLTK\Fl_Gl_2DSurface.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\Widget\FLTK\Fl_Gl_Multi_Display.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\Widget\FLTK\Fl_Gl_Single_Display.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\Widget\FLTK\Fl_GridLayout.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\Widget\FLTK\Fl_Palette.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\Widget\FLTK\Fl_X_Axis.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\Widget\FLTK\Fl_Y_Axis.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\Widget\FLTK\Fl_ZoomSlider.cxx
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\..\src\Visualization\Widget\FLTKWrapper.cxx
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

SOURCE=..\..\..\src\Errors\ErrGUI.hxx
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

SOURCE=..\..\..\src\Defines\CLAMGL.hxx
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
# Begin Group "Storage Headers"

# PROP Default_Filter ""
# Begin Group "Base Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\src\Storage\Base\Storable.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Storage\Base\Storage.hxx
# End Source File
# End Group
# Begin Group "XML Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\src\Storage\XML\BasicXMLable.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Storage\XML\XercesDomPrinter.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Storage\XML\XMLable.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Storage\XML\XMLAdapter.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Storage\XML\XMLArrayAdapter.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Storage\XML\XMLComponentAdapter.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Storage\XML\XMLIterableAdapter.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Storage\XML\XMLStaticAdapter.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Storage\XML\XMLStorage.hxx
# End Source File
# End Group
# End Group
# Begin Group "Base Headers No. 1"

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
# Begin Group "Standard Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\src\Standard\Array.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Standard\ArrayToBPFCnv.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Standard\BPF.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Standard\BPFTmpl.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Standard\BPFTmplDec.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Standard\BPFTmplDef.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Standard\CircularBuffer.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Standard\CommonOps.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Standard\Complex.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Standard\ComplexTmpl.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Standard\ComplexTmplDec.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Standard\ComplexTmplDef.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Standard\ComplexToPolarCnv.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Standard\Enum.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Standard\Flags.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Standard\GlobalEnums.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Standard\IndexArray.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Standard\List.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Standard\PhantomBuffer.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Standard\Point.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Standard\PointTmpl.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Standard\PointTmplDec.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Standard\PointTmplDef.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Standard\Polar.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Standard\PolarTmpl.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Standard\PolarTmplDec.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Standard\PolarTmplDef.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Standard\Search.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Standard\SearchArray.hxx
# End Source File
# End Group
# Begin Group "Data Headers"

# PROP Default_Filter ""
# Begin Group "Base Headers No. 2"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\src\Data\Base\ProcessingData.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Data\Base\ProcessingDataConfig.hxx
# End Source File
# End Group
# Begin Group "BasicProcessing Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\src\Data\BasicProcessing\Audio.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Data\BasicProcessing\AudioCircularBuffer.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Data\BasicProcessing\Frame.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Data\BasicProcessing\Fundamental.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Data\BasicProcessing\Segment.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Data\BasicProcessing\SpectralPeak.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Data\BasicProcessing\SpectralPeakArray.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Data\BasicProcessing\Spectrum.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Data\BasicProcessing\SpectrumConfig.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Data\BasicProcessing\SpectrumConversions.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Data\BasicProcessing\SpecTypeFlags.hxx
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

SOURCE=..\..\..\src\Flow\Ports\AudioInPortTmpl.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Flow\Ports\AudioOutPortTmpl.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Flow\Ports\InPort.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Flow\Ports\InPortTmpl.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Flow\Ports\OutPort.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Flow\Ports\OutPortTmpl.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Flow\Ports\Port.hxx
# End Source File
# End Group
# Begin Group "Nodes Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\src\Flow\Nodes\AddStreamRegion.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Flow\Nodes\AudioStreamBuffer.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Flow\Nodes\CircularStreamImpl.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Flow\Nodes\DelayStreamRegion.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Flow\Nodes\InplaceStreamRegion.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Flow\Nodes\Node.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Flow\Nodes\ReadStreamRegion.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Flow\Nodes\SourceStreamRegion.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Flow\Nodes\StreamBuffer.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Flow\Nodes\StreamRegion.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Flow\Nodes\StreamRegionContainer.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Flow\Nodes\WriteStreamRegion.hxx
# End Source File
# End Group
# End Group
# Begin Group "Processing Headers"

# PROP Default_Filter ""
# Begin Group "Base Headers No. 3"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\src\Processing\Base\Processing.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Processing\Base\ProcessingComposite.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Processing\Base\ProcessingConfig.hxx
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
# Begin Group "AudioFileIO Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\src\Processing\AudioFileIO\AudioFile.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Processing\AudioFileIO\AudioFileIn.hxx
# End Source File
# End Group
# Begin Group "AudioIO Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\src\Processing\AudioIO\AudioIn.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Processing\AudioIO\AudioIO.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Processing\AudioIO\AudioOut.hxx
# End Source File
# End Group
# Begin Group "Analysis Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\src\Processing\Analysis\CircularShift.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Processing\Analysis\CircularShiftConfig.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Processing\Analysis\FFT.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Processing\Analysis\FFT_rfftw.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Processing\Analysis\FFTConfig.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Processing\Analysis\SpectralAnalysis.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Processing\Analysis\SpectralAnalysisConfig.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Processing\Analysis\WindowGenerator.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Processing\Analysis\WindowGeneratorConfig.hxx
# End Source File
# End Group
# Begin Group "ArithOps Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\src\Processing\ArithOps\AudioMultiplier.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Processing\ArithOps\BinaryAudioOp.hxx
# End Source File
# End Group
# Begin Group "Synthesis Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\src\Processing\Synthesis\IFFT.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Processing\Synthesis\IFFT_rfftw.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Processing\Synthesis\IFFTConfig.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Processing\Synthesis\OverlapAdd.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Processing\Synthesis\OverlapAddConfig.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Processing\Synthesis\SpectralSynthesis.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Processing\Synthesis\SpectralSynthesisConfig.hxx
# End Source File
# End Group
# End Group
# Begin Group "Tools Headers"

# PROP Default_Filter ""
# Begin Group "AudioIO Headers No. 1"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\src\Tools\AudioIO\AudioDevice.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Tools\AudioIO\AudioDeviceList.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Tools\AudioIO\AudioManager.hxx
# End Source File
# End Group
# Begin Group "AudioFileIO Headers No. 1"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\src\Tools\AudioFileIO\AIFFFileIO.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Tools\AudioFileIO\extended.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Tools\AudioFileIO\SoundFileIO.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Tools\AudioFileIO\SoundHeader.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Tools\AudioFileIO\WaveFileIO.hxx
# End Source File
# End Group
# End Group
# Begin Group "draft Headers"

# PROP Default_Filter ""
# Begin Group "LPC Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\draft\LPC\AutocorrelationEvaluator.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\draft\LPC\Correlation.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\draft\LPC\Levinson_Durbin.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\draft\LPC\LPC.hxx
# End Source File
# End Group
# End Group
# Begin Group "xercesc Headers"

# PROP Default_Filter ""
# Begin Group "include Headers"

# PROP Default_Filter ""
# Begin Group "xercesc Headers No. 1"

# PROP Default_Filter ""
# Begin Group "util Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\xercesc\include\xercesc\util\PlatformUtils.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\xercesc\include\xercesc\util\TranscodingException.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\xercesc\include\xercesc\util\XercesDefs.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\xercesc\include\xercesc\util\XMLString.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\xercesc\include\xercesc\util\XMLUniDefs.hpp
# End Source File
# End Group
# Begin Group "parsers Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\xercesc\include\xercesc\parsers\DOMParser.hpp
# End Source File
# End Group
# Begin Group "dom Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\xercesc\include\xercesc\dom\DOM.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\xercesc\include\xercesc\dom\DOM_Attr.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\xercesc\include\xercesc\dom\DOM_CDATASection.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\xercesc\include\xercesc\dom\DOM_CharacterData.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\xercesc\include\xercesc\dom\DOM_Comment.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\xercesc\include\xercesc\dom\DOM_Document.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\xercesc\include\xercesc\dom\DOM_DocumentFragment.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\xercesc\include\xercesc\dom\DOM_DocumentType.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\xercesc\include\xercesc\dom\DOM_DOMException.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\xercesc\include\xercesc\dom\DOM_DOMImplementation.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\xercesc\include\xercesc\dom\DOM_Element.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\xercesc\include\xercesc\dom\DOM_Entity.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\xercesc\include\xercesc\dom\DOM_EntityReference.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\xercesc\include\xercesc\dom\DOM_NamedNodeMap.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\xercesc\include\xercesc\dom\DOM_Node.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\xercesc\include\xercesc\dom\DOM_NodeFilter.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\xercesc\include\xercesc\dom\DOM_NodeIterator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\xercesc\include\xercesc\dom\DOM_NodeList.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\xercesc\include\xercesc\dom\DOM_Notation.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\xercesc\include\xercesc\dom\DOM_ProcessingInstruction.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\xercesc\include\xercesc\dom\DOM_Range.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\xercesc\include\xercesc\dom\DOM_Text.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\xercesc\include\xercesc\dom\DOM_TreeWalker.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\xercesc\include\xercesc\dom\DOM_XMLDecl.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\xercesc\include\xercesc\dom\DOMString.hpp
# End Source File
# End Group
# Begin Group "framework Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\xercesc\include\xercesc\framework\XMLFormatter.hpp
# End Source File
# End Group
# End Group
# End Group
# End Group
# Begin Group "fftw Headers"

# PROP Default_Filter ""
# Begin Group "include Headers No. 1"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\fftw\include\fftw.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\fftw\include\rfftw.h
# End Source File
# End Group
# End Group
# Begin Group "Visualization Headers"

# PROP Default_Filter ""
# Begin Group "Plot Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\src\Visualization\Plot\Plot.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\Plot\PlotAudio.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\Plot\PlotSinTracks.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\Plot\PlotSpectralPeaks.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\Plot\PlotSpectrum.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\Plot\Plotter.hxx
# End Source File
# End Group
# Begin Group "Base Headers No. 4"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\src\Visualization\Base\Connection.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\Base\ConnectionHandler.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\Base\DrawingSurface2D.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\Base\ModelAdapter.hxx
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

SOURCE=..\..\..\src\Visualization\Base\Signalv4.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\Base\Signalv4ImplSerious.hxx
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
# Begin Source File

SOURCE=..\..\..\src\Visualization\Base\Slotv4.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\Base\SupportedToolkits.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\Base\WidgetTKWrapper.hxx
# End Source File
# End Group
# Begin Group "View Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\src\Visualization\View\AudioAdapter.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\View\AudioModel.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\View\LogMagSpectrumAdapter.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\View\Partial.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\View\ProcessingDataAdapter.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\View\SinTracksAdapter.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\View\SinTracksModel.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\View\SpectralPeakArrayAdapter.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\View\SpectralPeaksModel.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\View\SpectrumAdapter.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\View\SpectrumModel.hxx
# End Source File
# End Group
# Begin Group "Presentation Headers"

# PROP Default_Filter ""
# Begin Group "Base Headers No. 5"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\src\Visualization\Presentation\Base\AudioPresentation.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\Presentation\Base\SinTracksPresentation.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\Presentation\Base\SpectralPeaksPresentation.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\Presentation\Base\SpectrumPresentation.hxx
# End Source File
# End Group
# Begin Group "FLTK Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\src\Visualization\Presentation\FLTK\Fl_Audio.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\Presentation\FLTK\Fl_SinTracks.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\Presentation\FLTK\Fl_Spectrum.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\Presentation\FLTK\Fl_SpectrumPeaks.hxx
# End Source File
# End Group
# End Group
# Begin Group "Util Headers No. 1"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\src\Visualization\Util\BoundingBoxes.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\Util\SineTracksDef.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\Util\SinTrackHorClipper.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\Util\SinTrackListBuilder.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\Util\SinTrackVerClipper.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\Util\TimeFreqMagPoint.hxx
# End Source File
# End Group
# Begin Group "GL Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\src\Visualization\GL\AudioRenderingManager.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\GL\GLMaxMinArrays.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\GL\GLPointsArrays.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\GL\GLRenderer.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\GL\GLRenderingManager.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\GL\GLSampleDetailArray.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\GL\GLSineTracks.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\GL\GLStraightLineArray.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\GL\PeaksRenderingManager.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\GL\SinTracksRenderingManager.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\GL\SpectrumRenderingManager.hxx
# End Source File
# End Group
# Begin Group "Widget Headers"

# PROP Default_Filter ""
# Begin Group "FLTK Headers No. 1"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\src\Visualization\Widget\FLTK\Fl_Axis.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\Widget\FLTK\Fl_Axis_Enumerations.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\Widget\FLTK\Fl_Gl_2DSurface.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\Widget\FLTK\Fl_Gl_Multi_Display.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\Widget\FLTK\Fl_Gl_Single_Display.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\Widget\FLTK\Fl_GridLayout.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\Widget\FLTK\Fl_Palette.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\Widget\FLTK\Fl_X_Axis.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\Widget\FLTK\Fl_Y_Axis.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\Widget\FLTK\Fl_ZoomSlider.hxx
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\..\src\Visualization\Widget\FLTKWrapper.hxx
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
# Begin Group "fltk Headers No. 2"

# PROP Default_Filter ""
# Begin Group "include Headers No. 2"

# PROP Default_Filter ""
# Begin Group "FL Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\fltk\include\FL\Enumerations.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\fltk\include\FL\Fl.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\fltk\include\FL\Fl_Box.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\fltk\include\FL\fl_draw.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\fltk\include\FL\Fl_Export.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\fltk\include\FL\Fl_Gl_Window.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\fltk\include\FL\Fl_Group.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\fltk\include\FL\Fl_Tooltip.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\fltk\include\FL\Fl_Valuator.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\fltk\include\FL\Fl_Widget.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\fltk\include\FL\Fl_Window.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\fltk\include\FL\gl.h
# End Source File
# End Group
# Begin Group "Fl Headers No. 1"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\fltk\include\Fl\Fl_Group.H
# End Source File
# End Group
# End Group
# End Group
# Begin Group "pthreads Headers"

# PROP Default_Filter ""
# Begin Group "include Headers No. 3"

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

!IF  "$(CFG)" == "LPC - Win32 Release"

# Begin Custom Build
InputPath=settings.cfg

"buildstamp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	srcdeps.exe settings.cfg LPC.dsp

# End Custom Build

!ELSEIF  "$(CFG)" == "LPC - Win32 Debug"

# Begin Custom Build
InputPath=settings.cfg

"buildstamp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	srcdeps.exe settings.cfg LPC.dsp

# End Custom Build

!ENDIF 

# End Source File
# End Target
# End Project
