# Microsoft Developer Studio Project File - Name="SpectralDelay" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=SpectralDelay - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "SpectralDelay.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "SpectralDelay.mak" CFG="SpectralDelay - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "SpectralDelay - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "SpectralDelay - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "empty"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "SpectralDelay - Win32 Release"

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
# ADD CPP /nologo /MD /W3 /GR /GX /O2 /FD /c /Zm1000 /D "NDEBUG" /D "HAVE_STANDARD_SSTREAM" /D "HAVE_STANDARD_SSTREAM_STR" /D "HAVE_STANDARD_VECTOR_AT" /D "HAVE_STANDARD_UNICODE" /D "WIN32" /D "_MBCS" /D "_CONSOLE" /D "CLAM_FLOAT" /D "CLAM_USE_XML" /D "__WINDOWS_DS__" /D "ID3LIB_LINKOPTION=1" /I "..\..\..\src\Defines" /FI"preinclude.hxx" /I "..\..\..\externals\CbLib" /I "..\..\..\..\xercesc\include" /I "..\..\..\..\fftw\include" /I "..\..\..\..\fltk\include" /I "..\..\..\..\pthreads\include" /I "..\..\..\..\dxsdk\include" /I "..\..\..\examples\SpectralDelay\App" /I "..\..\..\examples\SpectralDelay\GUI" /I "..\..\..\examples\SpectralDelay\GUI\FLTK" /I "..\..\..\examples\SpectralDelay\GUI\Presentation" /I "..\..\..\examples\SpectralDelay\GUI\View" /I "..\..\..\examples\SpectralDelay\Processing" /I "..\..\..\src\Base" /I "..\..\..\src\Data\Base" /I "..\..\..\src\Data\BasicProcessing" /I "..\..\..\src\Defines" /I "..\..\..\src\Defines\Windows" /I "..\..\..\src\Errors" /I "..\..\..\src\Flow\Controls" /I "..\..\..\src\Flow\Nodes" /I "..\..\..\src\Flow\Ports" /I "..\..\..\src\Processing\Analysis" /I "..\..\..\src\Processing\ArithOps" /I "..\..\..\src\Processing\AudioIO" /I "..\..\..\src\Processing\Base" /I "..\..\..\src\Processing\Synthesis" /I "..\..\..\src\Processing\Transformations" /I "..\..\..\src\Processing\AudioFileIO" /I "..\..\..\src\Standard" /I "..\..\..\src\Storage\Base" /I "..\..\..\src\Storage\XML" /I "..\..\..\src\System\Application" /I "..\..\..\src\System\Threads" /I "..\..\..\src\Tools\AudioFileIO" /I "..\..\..\src\Tools\AudioIO" /I "..\..\..\src\Visualization\Base" /I "..\..\..\src\Visualization\GL" /I "..\..\..\src\Visualization\Presentation\Base" /I "..\..\..\src\Visualization\Util" /I "..\..\..\src\Visualization\View" /I "..\..\..\src\Visualization\Widget" /I "..\..\..\src\Visualization\Widget\FLTK" /I "..\..\..\externals"
# ADD BASE RSC /l 0xc0a /d "NDEBUG"
# ADD RSC /l 0xc0a /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32
# ADD LINK32 /libpath:"..\..\..\..\fftw\lib" /libpath:"..\..\..\..\fltk\lib" /libpath:"..\..\..\..\xercesc\lib" /libpath:"..\..\..\..\pthreads\lib" /libpath:"..\..\..\..\dxsdk\lib" /libpath:"..\..\..\..\libsndfile\lib" /libpath:"..\..\..\..\oggvorbis\lib" /libpath:"..\..\..\..\libmad\lib" /libpath:"..\..\..\..\id3lib\lib" RFFTW2st.lib FFTW2st.lib fltk_1_1.lib fltkforms_1_1d.lib fltkgl_1_1d.lib fltkimages_1_1d.lib comctl32.lib kernel32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib wsock32.lib user32.lib gdi32.lib opengl32.lib glu32.lib xerces-c_2.lib dsound.lib dxerr8.lib user32.lib gdi32.lib pthreadVC.lib libsndfile.lib ogg_static.lib vorbis_static.lib vorbisenc_static.lib vorbisfile_static.lib id3lib.lib libmad.lib /nologo /subsystem:console /machine:I386
# SUBTRACT LINK32 /nologo /verbose /pdb:none /debug

!ELSEIF  "$(CFG)" == "SpectralDelay - Win32 Debug"

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
# ADD CPP /nologo /MDd /W3 /GR /GX /ZI /Od /D "_DEBUG" /FD /GZ /Zm1000 /c /D "HAVE_STANDARD_SSTREAM" /D "HAVE_STANDARD_SSTREAM_STR" /D "HAVE_STANDARD_VECTOR_AT" /D "HAVE_STANDARD_UNICODE" /D "WIN32" /D "_MBCS" /D "_CONSOLE" /D "CLAM_FLOAT" /D "CLAM_USE_XML" /D "__WINDOWS_DS__" /D "ID3LIB_LINKOPTION=1" /I "..\..\..\src\Defines" /FI"preinclude.hxx" /I "..\..\..\externals\CbLib" /I "..\..\..\..\xercesc\include" /I "..\..\..\..\fftw\include" /I "..\..\..\..\fltk\include" /I "..\..\..\..\pthreads\include" /I "..\..\..\..\dxsdk\include" /I "..\..\..\examples\SpectralDelay\App" /I "..\..\..\examples\SpectralDelay\GUI" /I "..\..\..\examples\SpectralDelay\GUI\FLTK" /I "..\..\..\examples\SpectralDelay\GUI\Presentation" /I "..\..\..\examples\SpectralDelay\GUI\View" /I "..\..\..\examples\SpectralDelay\Processing" /I "..\..\..\src\Base" /I "..\..\..\src\Data\Base" /I "..\..\..\src\Data\BasicProcessing" /I "..\..\..\src\Defines" /I "..\..\..\src\Defines\Windows" /I "..\..\..\src\Errors" /I "..\..\..\src\Flow\Controls" /I "..\..\..\src\Flow\Nodes" /I "..\..\..\src\Flow\Ports" /I "..\..\..\src\Processing\Analysis" /I "..\..\..\src\Processing\ArithOps" /I "..\..\..\src\Processing\AudioIO" /I "..\..\..\src\Processing\Base" /I "..\..\..\src\Processing\Synthesis" /I "..\..\..\src\Processing\Transformations" /I "..\..\..\src\Processing\AudioFileIO" /I "..\..\..\src\Standard" /I "..\..\..\src\Storage\Base" /I "..\..\..\src\Storage\XML" /I "..\..\..\src\System\Application" /I "..\..\..\src\System\Threads" /I "..\..\..\src\Tools\AudioFileIO" /I "..\..\..\src\Tools\AudioIO" /I "..\..\..\src\Visualization\Base" /I "..\..\..\src\Visualization\GL" /I "..\..\..\src\Visualization\Presentation\Base" /I "..\..\..\src\Visualization\Util" /I "..\..\..\src\Visualization\View" /I "..\..\..\src\Visualization\Widget" /I "..\..\..\src\Visualization\Widget\FLTK" /I "..\..\..\externals"
# ADD BASE RSC /l 0xc0a /d "_DEBUG"
# ADD RSC /l 0xc0a
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32
# ADD LINK32 /libpath:"..\..\..\..\fftw\lib" /libpath:"..\..\..\..\fltk\lib" /libpath:"..\..\..\..\xercesc\lib" /libpath:"..\..\..\..\pthreads\lib" /libpath:"..\..\..\..\dxsdk\lib" /libpath:"..\..\..\..\libsndfile\lib" /libpath:"..\..\..\..\oggvorbis\lib" /libpath:"..\..\..\..\libmad\lib" /libpath:"..\..\..\..\id3lib\lib" FFTW2std.lib RFFTW2std.lib fltk_1_1d.lib fltkforms_1_1d.lib fltkgl_1_1d.lib fltkimages_1_1d.lib comctl32.lib kernel32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib wsock32.lib user32.lib gdi32.lib opengl32.lib glu32.lib xerces-c_2d.lib dsound.lib dxerr8.lib user32.lib gdi32.lib pthreadVC.lib libsndfile.lib ogg_static_d.lib vorbis_static_d.lib vorbisenc_static_d.lib vorbisfile_static_d.lib id3libd.lib libmadd.lib /nologo /subsystem:console /debug /machine:I386
# SUBTRACT LINK32 /nologo /verbose /pdb:none

!ENDIF 

# Begin Target

# Name "SpectralDelay - Win32 Release"
# Name "SpectralDelay - Win32 Debug"
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Group "Source Files"

# Begin Group "examples Sources"

# Begin Group "SpectralDelay Sources"

# Begin Group "App Sources"

# Begin Source File

SOURCE=..\..\..\examples\SpectralDelay\App\mainSpectralDelayFL.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\examples\SpectralDelay\App\FLSpectralDelayApp.cxx
# End Source File
# End Group
# Begin Group "Processing Sources"

# Begin Source File

SOURCE=..\..\..\examples\SpectralDelay\Processing\DelayNFilter.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\examples\SpectralDelay\Processing\DummyTest.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\examples\SpectralDelay\Processing\SpectralDelayKernel.cxx
# End Source File
# End Group
# Begin Group "GUI Sources"

# Begin Group "View Sources"

# Begin Source File

SOURCE=..\..\..\examples\SpectralDelay\GUI\View\AudioFileView.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\examples\SpectralDelay\GUI\View\ViewRegistry.cxx
# End Source File
# End Group
# Begin Group "FLTK Sources"

# Begin Source File

SOURCE=..\..\..\examples\SpectralDelay\GUI\FLTK\FLGeoMgr.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\examples\SpectralDelay\GUI\FLTK\FLFileIOPres.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\examples\SpectralDelay\GUI\FLTK\FLSpectralDelayGUI.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\examples\SpectralDelay\GUI\FLTK\FLFileIOWidget.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\examples\SpectralDelay\GUI\FLTK\FLGridLayout.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\examples\SpectralDelay\GUI\FLTK\Fl_SpectrumAnalyzer.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\examples\SpectralDelay\GUI\FLTK\FLDelayPres.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\examples\SpectralDelay\GUI\FLTK\FLFilterBankPres.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\examples\SpectralDelay\GUI\FLTK\MBEditorUtils.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\examples\SpectralDelay\GUI\FLTK\FLMultiBandEditor.cxx
# End Source File
# End Group
# Begin Group "Presentation Sources"

# Begin Source File

SOURCE=..\..\..\examples\SpectralDelay\GUI\Presentation\BaseAudioFilePres.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\examples\SpectralDelay\GUI\Presentation\BaseFilterBankPres.cxx
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\..\examples\SpectralDelay\GUI\MultiBandProxy.cxx
# End Source File
# End Group
# End Group
# End Group
# Begin Group "Tools Sources"

# Begin Group "AudioIO Sources"

# Begin Group "Windows Sources"

# Begin Source File

SOURCE=..\..\..\src\Tools\AudioIO\Windows\DirectXAudioDevice.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Tools\AudioIO\Windows\DXFullDuplex.cxx
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

SOURCE=..\..\..\src\Tools\AudioFileIO\SoundHeader.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Tools\AudioFileIO\SoundFileIO.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Tools\AudioFileIO\AIFFFileIO.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Tools\AudioFileIO\WaveFileIO.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Tools\AudioFileIO\extended.cxx
# End Source File
# End Group
# End Group
# Begin Group "System Sources"

# Begin Group "Application Sources"

# Begin Source File

SOURCE=..\..\..\src\System\Application\ProcessingKernel.cxx
# End Source File
# End Group
# Begin Group "Threads Sources"

# Begin Source File

SOURCE=..\..\..\src\System\Threads\Condition.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\System\Threads\Thread.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\System\Threads\Mutex.cxx
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

SOURCE=..\..\..\src\Errors\ErrSoundFileIO.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Errors\ErrDynamicType.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Errors\ErrSystem.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Errors\ErrGUI.cxx
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

SOURCE=..\..\..\src\Storage\XML\XercesDomWriter.cxx
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
# Begin Source File

SOURCE=..\..\..\src\Data\BasicProcessing\Spectrum.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Data\BasicProcessing\Fundamental.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Data\BasicProcessing\SpectralPeak.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Data\BasicProcessing\SpectralPeakArray.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Data\BasicProcessing\Frame.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Data\BasicProcessing\Segment.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Data\BasicProcessing\SpectrumConversions.cxx
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

SOURCE=..\..\..\src\Flow\Ports\OutPort.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Flow\Ports\InPort.cxx
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

SOURCE=..\..\..\src\Flow\Nodes\AddStreamRegion.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Flow\Nodes\InplaceStreamRegion.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Flow\Nodes\StreamRegionContainer.cxx
# End Source File
# End Group
# End Group
# Begin Group "Processing Sources No. 1"

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
# Begin Group "AudioFileIO Sources No. 1"

# Begin Source File

SOURCE=..\..\..\src\Processing\AudioFileIO\AudioFileConfig.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Processing\AudioFileIO\AudioFileIn.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Processing\AudioFileIO\AudioFileOut.cxx
# End Source File
# End Group
# Begin Group "AudioIO Sources No. 1"

# Begin Source File

SOURCE=..\..\..\src\Processing\AudioIO\AudioIn.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Processing\AudioIO\AudioOut.cxx
# End Source File
# End Group
# Begin Group "Analysis Sources"

# Begin Source File

SOURCE=..\..\..\src\Processing\Analysis\WindowGenerator.cxx
# End Source File
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
# End Group
# Begin Group "ArithOps Sources"

# Begin Source File

SOURCE=..\..\..\src\Processing\ArithOps\AudioMultiplier.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Processing\ArithOps\SpectrumAdder.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Processing\ArithOps\SpectrumProduct.cxx
# End Source File
# End Group
# Begin Group "Synthesis Sources"

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
# Begin Group "Transformations Sources"

# Begin Source File

SOURCE=..\..\..\src\Processing\Transformations\TokenDelay.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Processing\Transformations\FDFilterGen.cxx
# End Source File
# End Group
# End Group
# Begin Group "Standard Sources"

# Begin Source File

SOURCE=..\..\..\src\Standard\Enum.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Standard\GlobalEnums.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Standard\Flags.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Standard\Complex.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Standard\Polar.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Standard\Point.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Standard\BPF.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Standard\Text.cxx
# End Source File
# End Group
# Begin Group "Visualization Sources"

# Begin Group "Base Sources No. 2"

# Begin Source File

SOURCE=..\..\..\src\Visualization\Base\ModelAdapter.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\Base\Signal.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\Base\Connection.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\Base\Slot.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\Base\ModelController.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\Base\Presentation.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\Base\DrawingSurface2D.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\Base\WidgetTKWrapper.cxx
# End Source File
# End Group
# Begin Group "View Sources No. 1"

# Begin Source File

SOURCE=..\..\..\src\Visualization\View\SpectrumModel.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\View\SpectrumAdapter.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\View\InputControlModel.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\View\ControlAdapter.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\View\ProcessingInControlSet.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\View\ProcessingController.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\View\FDFilterController.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\View\TokenDelayController.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\View\LogMagSpectrumAdapter.cxx
# End Source File
# End Group
# Begin Group "Presentation Sources No. 1"

# Begin Group "Base Sources No. 3"

# Begin Source File

SOURCE=..\..\..\src\Visualization\Presentation\Base\SpectrumPresentation.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\Presentation\Base\InControlPresentation.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\Presentation\Base\ProcessingControlsPresentation.cxx
# End Source File
# End Group
# End Group
# Begin Group "GL Sources"

# Begin Source File

SOURCE=..\..\..\src\Visualization\GL\GLRenderingManager.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\GL\GLRenderer.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\GL\GLStraightLineArray.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\GL\GLSampleDetailArray.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\GL\SpectrumRenderingManager.cxx
# End Source File
# End Group
# Begin Group "Widget Sources"

# Begin Group "FLTK Sources No. 1"

# Begin Source File

SOURCE=..\..\..\src\Visualization\Widget\FLTK\Fl_GridLayout.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\Widget\FLTK\Fl_Gl_2DSurface.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\Widget\FLTK\Fl_Gl_Single_Display.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\Widget\FLTK\FLKnob.cxx
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\..\src\Visualization\Widget\FLTKWrapper.cxx
# End Source File
# End Group
# End Group
# End Group
# Begin Group "Header Files"

# Begin Group "System Headers"

# Begin Group "Application Headers"

# Begin Source File

SOURCE=..\..\..\src\System\Application\ProcessingKernel.hxx
# End Source File
# End Group
# Begin Group "Threads Headers"

# Begin Source File

SOURCE=..\..\..\src\System\Threads\xtime.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\System\Threads\Lock.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\System\Threads\Condition.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\System\Threads\Thread.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\System\Threads\Mutex.hxx
# End Source File
# End Group
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

SOURCE=..\..\..\src\Storage\XML\XercesEncodings.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Storage\XML\XercesInitializer.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Storage\XML\XercesDomReader.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Storage\XML\XercesDomWriter.hxx
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

SOURCE=..\..\..\src\Errors\ErrSoundFileIO.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Errors\ErrDynamicType.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Errors\ErrSystem.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Errors\ErrGUI.hxx
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
# Begin Group "Windows Headers"

# Begin Source File

SOURCE=..\..\..\src\Defines\Windows\CLAM_windows.h
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\..\src\Defines\CLAMGL.hxx
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
# Begin Group "Standard Headers"

# Begin Source File

SOURCE=..\..\..\src\Standard\Array.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Standard\Enum.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Standard\Filename.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Standard\FastRounding.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Standard\GlobalEnums.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Standard\CommonOps.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Standard\Flags.hxx
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

SOURCE=..\..\..\src\Standard\PointTmplDec.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Standard\PointTmplDef.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Standard\PointTmpl.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Standard\SearchArray.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Standard\BPFTmplDec.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Standard\Point.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Standard\BPF.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Standard\IndexArray.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Standard\List.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Standard\Search.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Standard\PhantomBuffer.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Standard\CircularBuffer.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Standard\Text.hxx
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

SOURCE=..\..\..\src\Standard\BPFTmplDef.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Standard\BPFTmpl.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Standard\ArrayToBPFCnv.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Standard\ComplexToPolarCnv.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Standard\TabFunct.hxx
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
# Begin Source File

SOURCE=..\..\..\src\Data\BasicProcessing\SpecTypeFlags.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Data\BasicProcessing\Spectrum.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Data\BasicProcessing\SpectrumConfig.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Data\BasicProcessing\Fundamental.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Data\BasicProcessing\SpectralPeak.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Data\BasicProcessing\SpectralPeakArray.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Data\BasicProcessing\Frame.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Data\BasicProcessing\Segment.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Data\BasicProcessing\AudioCircularBuffer.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Data\BasicProcessing\SpectrumConversions.hxx
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

SOURCE=..\..\..\src\Flow\Ports\OutPort.hxx
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

SOURCE=..\..\..\src\Flow\Ports\OutPortTmpl.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Flow\Ports\AudioOutPortTmpl.hxx
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

SOURCE=..\..\..\src\Flow\Nodes\AddStreamRegion.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Flow\Nodes\InplaceStreamRegion.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Flow\Nodes\StreamRegionContainer.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Flow\Nodes\AudioStreamBuffer.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Flow\Nodes\StreamBuffer.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Flow\Nodes\CircularStreamImpl.hxx
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
# Begin Group "AudioFileIO Headers"

# Begin Source File

SOURCE=..\..\..\src\Processing\AudioFileIO\AudioFileConfig.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Processing\AudioFileIO\AudioFileIn.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Processing\AudioFileIO\AudioFileOut.hxx
# End Source File
# End Group
# Begin Group "AudioIO Headers"

# Begin Source File

SOURCE=..\..\..\src\Processing\AudioIO\AudioIO.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Processing\AudioIO\AudioIn.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Processing\AudioIO\AudioOut.hxx
# End Source File
# End Group
# Begin Group "Analysis Headers"

# Begin Source File

SOURCE=..\..\..\src\Processing\Analysis\WindowGeneratorConfig.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Processing\Analysis\WindowGenerator.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Processing\Analysis\CircularShiftConfig.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Processing\Analysis\CircularShift.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Processing\Analysis\FFTConfig.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Processing\Analysis\SpectralAnalysisConfig.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Processing\Analysis\FFT.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Processing\Analysis\FFT_rfftw.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Processing\Analysis\SpectralAnalysis.hxx
# End Source File
# End Group
# Begin Group "ArithOps Headers"

# Begin Source File

SOURCE=..\..\..\src\Processing\ArithOps\BinaryAudioOp.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Processing\ArithOps\AudioMultiplier.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Processing\ArithOps\SpectrumAdder.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Processing\ArithOps\SpectrumProduct.hxx
# End Source File
# End Group
# Begin Group "Synthesis Headers"

# Begin Source File

SOURCE=..\..\..\src\Processing\Synthesis\IFFTConfig.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Processing\Synthesis\IFFT.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Processing\Synthesis\IFFT_rfftw.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Processing\Synthesis\OverlapAddConfig.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Processing\Synthesis\OverlapAdd.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Processing\Synthesis\SpectralSynthesisConfig.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Processing\Synthesis\SpectralSynthesis.hxx
# End Source File
# End Group
# Begin Group "Transformations Headers"

# Begin Source File

SOURCE=..\..\..\src\Processing\Transformations\TokenDelay.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Processing\Transformations\FDFilterGen.hxx
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
# Begin Group "Windows Headers No. 1"

# Begin Source File

SOURCE=..\..\..\src\Tools\AudioIO\Windows\DXFullDuplex.hxx
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

SOURCE=..\..\..\src\Tools\AudioFileIO\SoundHeader.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Tools\AudioFileIO\SoundFileIO.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Tools\AudioFileIO\AIFFFileIO.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Tools\AudioFileIO\WaveFileIO.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Tools\AudioFileIO\extended.hxx
# End Source File
# End Group
# End Group
# Begin Group "fftw Headers"

# Begin Group "include Headers"

# Begin Source File

SOURCE=..\..\..\..\fftw\include\fftw.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\fftw\include\rfftw.h
# End Source File
# End Group
# End Group
# Begin Group "externals Headers"

# Begin Source File

SOURCE=..\..\..\externals\deque
# End Source File
# Begin Group "CbLib Headers"

# Begin Source File

SOURCE=..\..\..\externals\CbLib\FunctorBase.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\externals\CbLib\Functor1.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\externals\CbLib\Functor2.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\externals\CbLib\Functor0.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\externals\CbLib\Functor3.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\externals\CbLib\Functor4.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\externals\CbLib\Functor0R.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\externals\CbLib\Functor1R.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\externals\CbLib\Functor2R.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\externals\CbLib\Functor3R.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\externals\CbLib\Functor4R.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\externals\CbLib\CBLMacros.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\externals\CbLib\CBL.hxx
# End Source File
# End Group
# End Group
# Begin Group "examples Headers"

# Begin Group "SpectralDelay Headers"

# Begin Group "App Headers"

# Begin Source File

SOURCE=..\..\..\examples\SpectralDelay\App\DelayPool.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\examples\SpectralDelay\App\FLSpectralDelayApp.hxx
# End Source File
# End Group
# Begin Group "Processing Headers No. 1"

# Begin Source File

SOURCE=..\..\..\examples\SpectralDelay\Processing\PlanetFriendlyTokenDelay.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\examples\SpectralDelay\Processing\DelayNFilter.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\examples\SpectralDelay\Processing\DummyTest.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\examples\SpectralDelay\Processing\SpectralDelayKernel.hxx
# End Source File
# End Group
# Begin Group "GUI Headers"

# Begin Group "View Headers"

# Begin Source File

SOURCE=..\..\..\examples\SpectralDelay\GUI\View\AudioFileView.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\examples\SpectralDelay\GUI\View\ViewRegistry.hxx
# End Source File
# End Group
# Begin Group "FLTK Headers"

# Begin Source File

SOURCE=..\..\..\examples\SpectralDelay\GUI\FLTK\FLGeoMgr.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\examples\SpectralDelay\GUI\FLTK\FLFileIOPres.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\examples\SpectralDelay\GUI\FLTK\FLSpectralDelayGUI.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\examples\SpectralDelay\GUI\FLTK\FLFileIOWidget.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\examples\SpectralDelay\GUI\FLTK\FLGridLayout.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\examples\SpectralDelay\GUI\FLTK\Fl_SpectrumAnalyzer.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\examples\SpectralDelay\GUI\FLTK\FLDelayPres.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\examples\SpectralDelay\GUI\FLTK\FLFilterBankPres.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\examples\SpectralDelay\GUI\FLTK\MBEditorUtils.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\examples\SpectralDelay\GUI\FLTK\FLMultiBandEditor.hxx
# End Source File
# End Group
# Begin Group "Presentation Headers"

# Begin Source File

SOURCE=..\..\..\examples\SpectralDelay\GUI\Presentation\BaseAudioFilePres.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\examples\SpectralDelay\GUI\Presentation\BaseFilterBankPres.hxx
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\..\examples\SpectralDelay\GUI\MultiBandProxy.hxx
# End Source File
# End Group
# End Group
# End Group
# Begin Group "Visualization Headers"

# Begin Group "Base Headers No. 4"

# Begin Source File

SOURCE=..\..\..\src\Visualization\Base\ModelAdapter.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\Base\Signal.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\Base\Connection.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\Base\Slot.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\Base\Slotv1.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\Base\ConnectionHandler.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\Base\Signalv1ImplSerious.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\Base\Signalv1.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\Base\Slotv2.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\Base\Signalv2ImplSerious.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\Base\Signalv2.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\Base\ModelController.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\Base\Presentation.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\Base\Slotv0.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\Base\Signalv0ImplSerious.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\Base\Signalv0.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\Base\Slotv4.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\Base\DrawingSurface2D.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\Base\WidgetTKWrapper.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\Base\SupportedToolkits.hxx
# End Source File
# End Group
# Begin Group "View Headers No. 1"

# Begin Source File

SOURCE=..\..\..\src\Visualization\View\ProcessingDataAdapter.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\View\SpectrumModel.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\View\SpectrumAdapter.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\View\InputControlModel.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\View\ControlAdapter.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\View\ProcessingInControlSet.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\View\ProcessingController.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\View\FDFilterController.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\View\TokenDelayController.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\View\LogMagSpectrumAdapter.hxx
# End Source File
# End Group
# Begin Group "Presentation Headers No. 1"

# Begin Group "Base Headers No. 5"

# Begin Source File

SOURCE=..\..\..\src\Visualization\Presentation\Base\SpectrumPresentation.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\Presentation\Base\InControlPresentation.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\Presentation\Base\ProcessingControlsPresentation.hxx
# End Source File
# End Group
# End Group
# Begin Group "Util Headers"

# Begin Source File

SOURCE=..\..\..\src\Visualization\Util\BoundingBoxes.hxx
# End Source File
# End Group
# Begin Group "GL Headers"

# Begin Source File

SOURCE=..\..\..\src\Visualization\GL\GLRenderingManager.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\GL\GLRenderer.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\GL\GLStraightLineArray.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\GL\GLSampleDetailArray.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\GL\SpectrumRenderingManager.hxx
# End Source File
# End Group
# Begin Group "Widget Headers"

# Begin Group "FLTK Headers No. 1"

# Begin Source File

SOURCE=..\..\..\src\Visualization\Widget\FLTK\Fl_GridLayout.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\Widget\FLTK\Fl_Gl_2DSurface.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\Widget\FLTK\Fl_Gl_Single_Display.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\Widget\FLTK\FLKnob.hxx
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\..\src\Visualization\Widget\FLTKWrapper.hxx
# End Source File
# End Group
# End Group
# Begin Group "fltk Headers"

# Begin Group "include Headers No. 1"

# Begin Group "FL Headers"

# Begin Source File

SOURCE=..\..\..\..\fltk\include\FL\Fl_Export.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\fltk\include\FL\Enumerations.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\fltk\include\FL\Fl_Widget.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\fltk\include\FL\Fl_Group.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\fltk\include\FL\Fl_Window.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\fltk\include\FL\Fl.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\fltk\include\FL\win32.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\fltk\include\FL\x.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\fltk\include\FL\fl_ask.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\fltk\include\FL\Fl_Button.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\fltk\include\FL\Fl_Box.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\fltk\include\FL\Fl_Image.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\fltk\include\FL\Fl_Menu_Item.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\fltk\include\FL\Fl_Menu_.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\fltk\include\FL\Fl_Choice.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\fltk\include\FL\Fl_Menu_Button.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\fltk\include\FL\Fl_Preferences.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\fltk\include\FL\Fl_Tile.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\fltk\include\FL\Fl_Valuator.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\fltk\include\FL\Fl_Slider.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\fltk\include\FL\Fl_Scrollbar.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\fltk\include\FL\Fl_Browser_.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\fltk\include\FL\Fl_Browser.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\fltk\include\FL\Fl_File_Icon.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\fltk\include\FL\filename.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\fltk\include\FL\dirent.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\fltk\include\FL\Fl_File_Browser.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\fltk\include\FL\Fl_Light_Button.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\fltk\include\FL\Fl_Check_Button.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\fltk\include\FL\Fl_Input_.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\fltk\include\FL\Fl_Input.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\fltk\include\FL\Fl_File_Input.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\fltk\include\FL\Fl_Return_Button.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\fltk\include\FL\fl_file_chooser.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\fltk\include\FL\Fl_Gl_Window.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\fltk\include\FL\fl_draw.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\fltk\include\FL\Fl_Output.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\fltk\include\FL\Fl_Tooltip.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\fltk\include\FL\gl.h
# End Source File
# End Group
# End Group
# End Group
# Begin Group "pthreads Headers"

# Begin Group "include Headers No. 2"

# Begin Source File

SOURCE=..\..\..\..\pthreads\include\pthread.h
# End Source File
# End Group
# End Group
# Begin Group "dxsdk Headers"

# Begin Group "include Headers No. 3"

# Begin Source File

SOURCE=..\..\..\..\dxsdk\include\dxerr8.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\dxsdk\include\dsound.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\dxsdk\include\dinput.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\dxsdk\include\basetsd.h
# End Source File
# End Group
# End Group
# Begin Group "xercesc Headers"

# Begin Group "include Headers No. 4"

# Begin Group "xercesc Headers No. 1"

# Begin Group "util Headers"

# Begin Source File

SOURCE=..\..\..\..\xercesc\include\xercesc\util\XMLString.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\xercesc\include\xercesc\util\XercesDefs.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\xercesc\include\xercesc\util\XMLUni.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\xercesc\include\xercesc\util\XMemory.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\xercesc\include\xercesc\util\PlatformUtils.hpp
# End Source File
# End Group
# Begin Group "parsers Headers"

# Begin Source File

SOURCE=..\..\..\..\xercesc\include\xercesc\parsers\XercesDOMParser.hpp
# End Source File
# End Group
# Begin Group "framework Headers"

# Begin Source File

SOURCE=..\..\..\..\xercesc\include\xercesc\framework\MemBufInputSource.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\xercesc\include\xercesc\framework\XMLFormatter.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\xercesc\include\xercesc\framework\MemBufFormatTarget.hpp
# End Source File
# End Group
# Begin Group "sax Headers"

# Begin Source File

SOURCE=..\..\..\..\xercesc\include\xercesc\sax\DocumentHandler.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\xercesc\include\xercesc\sax\DTDHandler.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\xercesc\include\xercesc\sax\EntityResolver.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\xercesc\include\xercesc\sax\ErrorHandler.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\xercesc\include\xercesc\sax\SAXException.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\xercesc\include\xercesc\sax\SAXParseException.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\xercesc\include\xercesc\sax\HandlerBase.hpp
# End Source File
# End Group
# Begin Group "dom Headers"

# Begin Source File

SOURCE=..\..\..\..\xercesc\include\xercesc\dom\DOMImplementationLS.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\xercesc\include\xercesc\dom\DOMException.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\xercesc\include\xercesc\dom\DOMRangeException.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\xercesc\include\xercesc\dom\DOMImplementation.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\xercesc\include\xercesc\dom\DOMImplementationRegistry.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\xercesc\include\xercesc\dom\DOMNode.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\xercesc\include\xercesc\dom\DOMNodeFilter.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\xercesc\include\xercesc\dom\DOMWriterFilter.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\xercesc\include\xercesc\dom\DOMErrorHandler.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\xercesc\include\xercesc\dom\DOMWriter.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\xercesc\include\xercesc\dom\DOMDocumentRange.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\xercesc\include\xercesc\dom\DOMDocumentTraversal.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\xercesc\include\xercesc\dom\DOMDocument.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\xercesc\include\xercesc\dom\DOMElement.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\xercesc\include\xercesc\dom\DOMCharacterData.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\xercesc\include\xercesc\dom\DOMText.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\xercesc\include\xercesc\dom\DOMNamedNodeMap.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\xercesc\include\xercesc\dom\DOMNodeList.hpp
# End Source File
# End Group
# End Group
# End Group
# End Group
# End Group
# Begin Source File

SOURCE=settings.cfg

!IF  "$(CFG)" == "SpectralDelay - Win32 Release"

# Begin Custom Build
InputPath=settings.cfg

"buildstamp" : $(SOURCE) "$(INTDIR)" "SpectralDelay.dsp"
	srcdeps.exe settings.cfg SpectralDelay.dsp

# End Custom Build
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "SpectralDelay - Win32 Debug"

# Begin Custom Build
InputPath=settings.cfg

"buildstamp" : $(SOURCE) "$(INTDIR)" "SpectralDelay.dsp"
	srcdeps.exe settings.cfg SpectralDelay.dsp

# End Custom Build
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# End Target
# End Project
