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
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
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
# ADD LINK32 kernel32.lib user32.lib gdi32.lib xerces-c_1D.lib rfftw2st.lib fftw2st.lib fltkd.lib wsock32.lib opengl32.lib glu32.lib dsound.lib dxerr8.lib dxguid.lib pthreadVCE.lib /subsystem:console /pdb:"..\ReleaseFiles\bin/LPC.pdb" /machine:I386 /out:"..\ReleaseFiles\bin/LPC.exe" /libpath:"../../../xercesc/lib" /libpath:"../../../fftw/lib" /libpath:"../../../fltk/lib" /libpath:"../../../dxsdk/lib" /libpath:"../../../pthreads/lib"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "LPC - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /MDd /W3 /GR /GX /ZI /Od /I "../../src/Base" /I "../../src/Data" /I "../../src/Data/Base" /I "../../src/Data/BasicProcessing" /I "../../src/Data/Descriptors" /I "../../src/Defines" /I "../../src/Defines/Windows" /I "../../src/Errors" /I "../../src/Flow" /I "../../src/Flow/Controls" /I "../../src/Flow/Nodes" /I "../../src/Flow/Ports" /I "../../src/Processing" /I "../../src/Processing/Analysis" /I "../../src/Processing/ArithOps" /I "../../src/Processing/AudioFileIO" /I "../../src/Processing/AudioIO" /I "../../src/Processing/Base" /I "../../src/Processing/Controls" /I "../../src/Processing/Generators" /I "../../src/Processing/MIDIIO" /I "../../src/Processing/Synthesis" /I "../../src/Processing/Transformations" /I "../../src/Standard" /I "../../src/Storage" /I "../../src/Storage/Base" /I "../../src/Storage/XML" /I "../../src/System/Application" /I "../../src/System/Threads" /I "../../src/Tools" /I "../../src/Tools/AudioFileIO" /I "../../src/Tools/AudioIO" /I "../../src/Tools/MIDIIO" /I "../../src/Visualization" /I "../../src/Visualization/Base" /I "../../src/Visualization/GL" /I "../../src/Visualization/Presentation" /I "../../src/Visualization/Services" /I "../../src/Visualization/SysInt" /I "../../src/Visualization/Util" /I "../../src/Visualization/View" /I "../../src/Visualization/Widget" /I "../../externals/CbLib" /I "../../../fftw/include" /I "../../../dxsdk/include" /I "../../../" /I "../../../xercesc/include" /I "../../../fltk/include" /I "../../../portmidi" /I "../../../include/pthreads" /I "../../src/Visualization/Plot" /I "../../../pthreads/include" /I "../../src/Visualization/Presentation/Base" /I "../../src/Visualization/Presentation/FLTK" /I "../../src/Visualization/Widget/FLTK" /FI"config.h" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D "CLAM_USE_XML" /D "CLAM_DOUBLE" /FR /Fo"..\DebugFiles\obj/" /Fd"..\DebugFiles\obj/" /FD /GZ /c
# ADD BASE RSC /l 0xc0a /d "_DEBUG"
# ADD RSC /l 0xc0a /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib xerces-c_1D.lib rfftw2st.lib fftw2st.lib fltkd.lib wsock32.lib opengl32.lib glu32.lib dsound.lib dxerr8.lib dxguid.lib pthreadVCE.lib /subsystem:console /incremental:no /pdb:"..\DebugFiles\bin/LPC.pdb" /debug /machine:I386 /out:"..\DebugFiles\bin/LPC.exe" /libpath:"../../../xercesc/lib" /libpath:"../../../fftw/lib" /libpath:"../../../fltk/lib" /libpath:"../../../dxsdk/lib" /libpath:"../../../pthreads/lib"
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "LPC - Win32 Release"
# Name "LPC - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "LPC Sources"

# PROP Default_Filter ""
# Begin Group "LPC Data Sources"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Correlation.cxx
# End Source File
# End Group
# Begin Group "LPC Processing Sources"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\AutocorrelationEvaluator.cxx
# End Source File
# Begin Source File

SOURCE=.\Levinson_Durbin.cxx
# End Source File
# Begin Source File

SOURCE=.\LPC.cxx
# End Source File
# End Group
# End Group
# Begin Group "CLAM Sources"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\src\Tools\AudioFileIO\AIFFFileIO.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Defines\Assert.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Data\BasicProcessing\Audio.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Tools\AudioIO\AudioDevice.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Tools\AudioIO\AudioDeviceList.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Processing\AudioFileIO\AudioFile.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Processing\AudioFileIO\AudioFileIn.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Processing\AudioFileIO\AudioFileOut.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Tools\AudioIO\AudioManager.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Storage\XML\BasicXMLable.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Standard\BPF.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Processing\Analysis\CircularShift.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Standard\Complex.cxx
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

SOURCE=..\..\src\Errors\ErrSystem.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Tools\AudioFileIO\extended.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Processing\Analysis\FFT.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Processing\Analysis\FFT_rfftw.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Standard\Flags.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Data\BasicProcessing\Frame.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Data\BasicProcessing\Fundamental.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Standard\GlobalEnums.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Processing\Synthesis\IFFT.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Processing\Synthesis\IFFT_rfftw.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Flow\Controls\InControl.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Flow\Ports\InPort.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\System\Threads\Mutex.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Flow\Controls\OutControl.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Flow\Ports\OutPort.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Standard\Polar.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Flow\Ports\Port.cxx
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

SOURCE=..\..\src\Data\BasicProcessing\Segment.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Tools\AudioFileIO\SoundFileIO.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Tools\AudioFileIO\SoundHeader.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Processing\Analysis\SpectralAnalysis.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Data\BasicProcessing\SpectralPeak.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Data\BasicProcessing\SpectralPeakArray.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Processing\Synthesis\SpectralSynthesis.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Data\BasicProcessing\Spectrum.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Data\BasicProcessing\SpectrumConversions.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Processing\Base\TopLevelProcessing.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Tools\AudioFileIO\WaveFileIO.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Processing\Analysis\WindowGenerator.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Storage\XML\XercesDomPrinter.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Storage\XML\XMLStorage.cxx
# End Source File
# End Group
# Begin Group "VM Sources"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\src\Visualization\View\AudioAdapter.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Visualization\View\AudioModel.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Visualization\Presentation\Base\AudioPresentation.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Visualization\GL\AudioRenderingManager.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Visualization\Base\Connection.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Visualization\Base\DrawingSurface2D.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Visualization\Presentation\FLTK\Fl_Audio.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Visualization\Widget\FLTK\Fl_Axis.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Visualization\Widget\FLTK\Fl_Gl_2DSurface.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Visualization\Widget\FLTK\Fl_Gl_Multi_Display.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Visualization\Widget\FLTK\Fl_Gl_Single_Display.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Visualization\Widget\FLTK\Fl_GridLayout.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Visualization\Widget\FLTK\Fl_Palette.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Visualization\Presentation\FLTK\Fl_Spectrum.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Visualization\Widget\FLTK\Fl_X_Axis.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Visualization\Widget\FLTK\Fl_Y_Axis.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Visualization\Widget\FLTK\Fl_ZoomSlider.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Visualization\Widget\FLTKWrapper.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Visualization\GL\GLArrayRenderer.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Visualization\GL\GLMaxMinArrays.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Visualization\GL\GLPointsArrays.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Visualization\GL\GLRenderer.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Visualization\GL\GLRenderingManager.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Visualization\GL\GLSampleDetailArray.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Visualization\GL\GLStraightLineArray.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Visualization\GL\HugeArrayRenderer.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Visualization\View\LogMagSpectrumAdapter.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Visualization\Base\ModelAdapter.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Visualization\Plot\PlotAudio.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Visualization\Plot\PlotSpectrum.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Visualization\Plot\Plotter.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Visualization\Base\Presentation.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Visualization\Base\Signal.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Visualization\Base\Slot.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Visualization\View\SpectrumAdapter.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Visualization\View\SpectrumModel.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Visualization\Presentation\Base\SpectrumPresentation.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Visualization\GL\SpectrumRenderingManager.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Visualization\Base\WidgetTKWrapper.cxx
# End Source File
# End Group
# Begin Source File

SOURCE=.\LPC_Example.cxx
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Group "LPC Headers"

# PROP Default_Filter ""
# Begin Group "LPC Data Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Correlation.hxx
# End Source File
# End Group
# Begin Group "LPC Processing Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\AutocorrelationEvaluator.hxx
# End Source File
# Begin Source File

SOURCE=.\Levinson_Durbin.hxx
# End Source File
# Begin Source File

SOURCE=.\LPC.hxx
# End Source File
# End Group
# End Group
# Begin Group "CLAM Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\src\Tools\AudioFileIO\AIFFFileIO.hxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Data\BasicProcessing\Audio.hxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Tools\AudioIO\AudioDevice.hxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Tools\AudioIO\AudioDeviceList.hxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Processing\AudioFileIO\AudioFile.hxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Processing\AudioFileIO\AudioFileIn.hxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Processing\AudioFileIO\AudioFileOut.hxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Tools\AudioIO\AudioManager.hxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Standard\BPF.hxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Processing\Analysis\CircularShift.hxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Standard\Complex.hxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Errors\ErrSystem.hxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Processing\Analysis\FFT.hxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Processing\Analysis\FFT_rfftw.hxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Standard\Flags.hxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Data\BasicProcessing\Frame.hxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Data\BasicProcessing\Fundamental.hxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Processing\Synthesis\IFFT.hxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Processing\Synthesis\IFFT_rfftw.hxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Flow\Controls\InControl.hxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Flow\Ports\InPort.hxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Flow\Ports\OutPort.hxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Standard\Polar.hxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Flow\Ports\Port.hxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Processing\Base\Processing.hxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Processing\Base\ProcessingComposite.hxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Data\Base\ProcessingData.hxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Data\Base\ProcessingDataConfig.hxx
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

SOURCE=..\..\src\Data\BasicProcessing\Segment.hxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Tools\AudioFileIO\SoundFileIO.hxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Tools\AudioFileIO\SoundHeader.hxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Processing\Analysis\SpectralAnalysis.hxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Data\BasicProcessing\SpectralPeak.hxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Data\BasicProcessing\SpectralPeakArray.hxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Processing\Synthesis\SpectralSynthesis.hxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Data\BasicProcessing\Spectrum.hxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Tools\AudioFileIO\WaveFileIO.hxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Processing\Analysis\WindowGenerator.hxx
# End Source File
# End Group
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
