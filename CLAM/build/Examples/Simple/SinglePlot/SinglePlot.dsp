# Microsoft Developer Studio Project File - Name="SinglePlot" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=SinglePlot - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "SinglePlot.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "SinglePlot.mak" CFG="SinglePlot - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "SinglePlot - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "SinglePlot - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "empty"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "SinglePlot - Win32 Release"

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
# ADD CPP /nologo /MD /W3 /GR /GX /O2 /I "..\..\..\..\src\Defines" /I "..\..\..\..\externals\CbLib" /I "..\..\..\..\..\xercesc\include" /I "..\..\..\..\..\fftw\include" /I "..\..\..\..\..\fltk\include" /I "..\..\..\..\src\Base" /I "..\..\..\..\src\Data\Base" /I "..\..\..\..\src\Data\BasicProcessing" /I "..\..\..\..\src\Defines\Windows" /I "..\..\..\..\src\Errors" /I "..\..\..\..\src\Flow\Controls" /I "..\..\..\..\src\Flow\Nodes" /I "..\..\..\..\src\Flow\Ports" /I "..\..\..\..\src\Processing\Analysis" /I "..\..\..\..\src\Processing\ArithOps" /I "..\..\..\..\src\Processing\Base" /I "..\..\..\..\src\Processing\Transformations" /I "..\..\..\..\src\Standard" /I "..\..\..\..\src\Storage\Base" /I "..\..\..\..\src\Storage\XML" /I "..\..\..\..\src\Visualization\Base" /I "..\..\..\..\src\Visualization\GL" /I "..\..\..\..\src\Visualization\Plot" /I "..\..\..\..\src\Visualization\Util" /I "..\..\..\..\src\Visualization\Widget" /I "..\..\..\..\src\Visualization\Widget\FLTK" /FI"preinclude.hxx" /D "NDEBUG" /D "HAVE_STANDARD_SSTREAM" /D "HAVE_STANDARD_SSTREAM_STR" /D "HAVE_STANDARD_VECTOR_AT" /D "WIN32" /D "_MBCS" /D "_CONSOLE" /D "CLAM_FLOAT" /D "CLAM_USE_XML" /FD /Zm1000 /c
# ADD BASE RSC /l 0xc0a /d "NDEBUG"
# ADD RSC /l 0xc0a /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /machine:IX86
# ADD LINK32 RFFTW2st.lib FFTW2st.lib fltk_1_1.lib fltkforms_1_1d.lib fltkgl_1_1d.lib fltkimages_1_1d.lib comctl32.lib kernel32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib wsock32.lib user32.lib gdi32.lib opengl32.lib glu32.lib xerces-c_2.lib pthreadVC.lib /subsystem:console /machine:I386 /libpath:"..\..\..\..\..\fftw\lib" /libpath:"..\..\..\..\..\fltk\lib" /libpath:"..\..\..\..\..\xercesc\lib" /libpath:"..\..\..\..\..\pthreads\lib"
# SUBTRACT LINK32 /nologo /verbose /pdb:none /debug

!ELSEIF  "$(CFG)" == "SinglePlot - Win32 Debug"

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
# ADD CPP /nologo /MDd /W3 /GR /GX /ZI /Od /I "..\..\..\..\src\Defines" /I "..\..\..\..\externals\CbLib" /I "..\..\..\..\..\xercesc\include" /I "..\..\..\..\..\fftw\include" /I "..\..\..\..\..\fltk\include" /I "..\..\..\..\src\Base" /I "..\..\..\..\src\Data\Base" /I "..\..\..\..\src\Data\BasicProcessing" /I "..\..\..\..\src\Defines\Windows" /I "..\..\..\..\src\Errors" /I "..\..\..\..\src\Flow\Controls" /I "..\..\..\..\src\Flow\Nodes" /I "..\..\..\..\src\Flow\Ports" /I "..\..\..\..\src\Processing\Analysis" /I "..\..\..\..\src\Processing\ArithOps" /I "..\..\..\..\src\Processing\Base" /I "..\..\..\..\src\Processing\Transformations" /I "..\..\..\..\src\Standard" /I "..\..\..\..\src\Storage\Base" /I "..\..\..\..\src\Storage\XML" /I "..\..\..\..\src\Visualization\Base" /I "..\..\..\..\src\Visualization\GL" /I "..\..\..\..\src\Visualization\Plot" /I "..\..\..\..\src\Visualization\Util" /I "..\..\..\..\src\Visualization\Widget" /I "..\..\..\..\src\Visualization\Widget\FLTK" /FI"preinclude.hxx" /D "_DEBUG" /D "HAVE_STANDARD_SSTREAM" /D "HAVE_STANDARD_SSTREAM_STR" /D "HAVE_STANDARD_VECTOR_AT" /D "WIN32" /D "_MBCS" /D "_CONSOLE" /D "CLAM_FLOAT" /D "CLAM_USE_XML" /FD /GZ /Zm1000 /c
# ADD BASE RSC /l 0xc0a /d "_DEBUG"
# ADD RSC /l 0xc0a
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /machine:IX86
# ADD LINK32 FFTW2std.lib RFFTW2std.lib fltk_1_1d.lib fltkforms_1_1d.lib fltkgl_1_1d.lib fltkimages_1_1d.lib comctl32.lib kernel32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib wsock32.lib user32.lib gdi32.lib opengl32.lib glu32.lib xerces-c_2d.lib pthreadVC.lib /subsystem:console /debug /machine:I386 /libpath:"..\..\..\..\..\fftw\lib" /libpath:"..\..\..\..\..\fltk\lib" /libpath:"..\..\..\..\..\xercesc\lib" /libpath:"..\..\..\..\..\pthreads\lib"
# SUBTRACT LINK32 /nologo /verbose /pdb:none

!ENDIF 

# Begin Target

# Name "SinglePlot - Win32 Release"
# Name "SinglePlot - Win32 Debug"
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Group "Source Files"

# PROP Default_Filter ""
# Begin Group "examples Sources"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\examples\PlotExamplesHelper.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\examples\SinglePlot_example.cxx
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

SOURCE=..\..\..\..\src\Errors\ErrGUI.cxx
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

SOURCE=..\..\..\..\src\Storage\XML\XercesDomWriter.cxx
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
# Begin Group "Visualization Sources"

# PROP Default_Filter ""
# Begin Group "Plot Sources"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\src\Visualization\Plot\GenericPlot.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Visualization\Plot\Plot.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Visualization\Plot\PlotBase.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Visualization\Plot\SystemPlots.cxx
# End Source File
# End Group
# Begin Group "Base Sources No. 1"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\src\Visualization\Base\Connection.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Visualization\Base\DrawingSurface2D.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Visualization\Base\Presentation.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Visualization\Base\Signal.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Visualization\Base\Slot.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Visualization\Base\TooltipTracker2D.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Visualization\Base\WidgetTKWrapper.cxx
# End Source File
# End Group
# Begin Group "Widget Sources"

# PROP Default_Filter ""
# Begin Group "FLTK Sources"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\src\Visualization\Widget\FLTK\Fl_Axis.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Visualization\Widget\FLTK\Fl_Gl_2DSurface.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Visualization\Widget\FLTK\Fl_Gl_Single_Display.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Visualization\Widget\FLTK\Fl_Plot.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Visualization\Widget\FLTK\Fl_SinglePlot.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Visualization\Widget\FLTK\Fl_X_Axis.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Visualization\Widget\FLTK\Fl_Y_Axis.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Visualization\Widget\FLTK\Fl_ZoomSlider.cxx
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\..\..\src\Visualization\Widget\FLTKWrapper.cxx
# End Source File
# End Group
# Begin Group "GL Sources"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\src\Visualization\GL\GLRenderer.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Visualization\GL\GLRenderingManager.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Visualization\GL\GLSampleDetailArray.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Visualization\GL\GLStraightLineArray.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Visualization\GL\SequenceRenderingManager.cxx
# End Source File
# End Group
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
# Begin Group "Base Sources No. 2"

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
# Begin Source File

SOURCE=..\..\..\..\src\Processing\Analysis\WindowGenerator.cxx
# End Source File
# End Group
# Begin Group "ArithOps Sources"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\src\Processing\ArithOps\AudioMultiplier.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\ArithOps\SpectrumProduct.cxx
# End Source File
# End Group
# Begin Group "Transformations Sources"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\src\Processing\Transformations\FDFilterGen.cxx
# End Source File
# End Group
# End Group
# End Group
# Begin Group "Header Files"

# PROP Default_Filter ""
# Begin Group "Defines Headers"

# PROP Default_Filter ""
# Begin Group "Windows Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\src\Defines\Windows\CLAM_windows.h
# End Source File
# End Group
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

SOURCE=..\..\..\..\src\Defines\CLAMGL.hxx
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
# Begin Group "Errors Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\src\Errors\Err.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Errors\ErrDynamicType.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Errors\ErrGUI.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Errors\ErrOutOfMemory.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Errors\ErrProcessingObj.hxx
# End Source File
# End Group
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

SOURCE=..\..\..\..\src\Storage\XML\XercesDomReader.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Storage\XML\XercesDomReadingContext.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Storage\XML\XercesDomWriter.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Storage\XML\XercesDomWritingContext.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Storage\XML\XercesEncodings.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Storage\XML\XercesInitializer.hxx
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

SOURCE=..\..\..\..\src\Standard\CommonOps.hxx
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
# Begin Group "Data Headers"

# PROP Default_Filter ""
# Begin Group "Base Headers No. 2"

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
# Begin Group "Visualization Headers"

# PROP Default_Filter ""
# Begin Group "Util Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\src\Visualization\Util\BoundingBoxes.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Visualization\Util\Color.hxx
# End Source File
# End Group
# Begin Group "Plot Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\src\Visualization\Plot\GenericPlot.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Visualization\Plot\Plot.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Visualization\Plot\PlotBase.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Visualization\Plot\SystemPlots.hxx
# End Source File
# End Group
# Begin Group "Base Headers No. 3"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\src\Visualization\Base\Connection.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Visualization\Base\ConnectionHandler.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Visualization\Base\DrawingSurface2D.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Visualization\Base\Presentation.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Visualization\Base\Signal.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Visualization\Base\Signalv2.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Visualization\Base\Signalv2ImplSerious.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Visualization\Base\Slot.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Visualization\Base\Slotv1.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Visualization\Base\Slotv2.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Visualization\Base\Slotv4.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Visualization\Base\SupportedToolkits.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Visualization\Base\TooltipTracker2D.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Visualization\Base\WidgetTKWrapper.hxx
# End Source File
# End Group
# Begin Group "Widget Headers"

# PROP Default_Filter ""
# Begin Group "FLTK Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\src\Visualization\Widget\FLTK\Fl_Axis.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Visualization\Widget\FLTK\Fl_Axis_Enumerations.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Visualization\Widget\FLTK\Fl_Gl_2DSurface.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Visualization\Widget\FLTK\Fl_Gl_Single_Display.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Visualization\Widget\FLTK\Fl_Plot.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Visualization\Widget\FLTK\Fl_SinglePlot.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Visualization\Widget\FLTK\Fl_X_Axis.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Visualization\Widget\FLTK\Fl_Y_Axis.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Visualization\Widget\FLTK\Fl_ZoomSlider.hxx
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\..\..\src\Visualization\Widget\FLTKWrapper.hxx
# End Source File
# End Group
# Begin Group "GL Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\src\Visualization\GL\GLRenderer.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Visualization\GL\GLRenderingManager.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Visualization\GL\GLSampleDetailArray.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Visualization\GL\GLStraightLineArray.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Visualization\GL\SequenceRenderingManager.hxx
# End Source File
# End Group
# End Group
# Begin Group "examples Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\examples\PlotExamplesHelper.hxx
# End Source File
# End Group
# Begin Group "fltk Headers No. 1"

# PROP Default_Filter ""
# Begin Group "include Headers"

# PROP Default_Filter ""
# Begin Group "Fl Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\..\fltk\include\Fl\Enumerations.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\fltk\include\Fl\Fl_Box.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\fltk\include\Fl\Fl_Export.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\fltk\include\Fl\Fl_Group.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\fltk\include\Fl\Fl_Widget.H
# End Source File
# End Group
# Begin Group "FL Headers No. 1"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\..\fltk\include\FL\Enumerations.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\fltk\include\FL\Fl.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\fltk\include\FL\Fl_Box.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\fltk\include\FL\fl_draw.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\fltk\include\FL\Fl_Export.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\fltk\include\FL\Fl_Gl_Window.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\fltk\include\FL\Fl_Group.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\fltk\include\FL\Fl_Tooltip.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\fltk\include\FL\Fl_Valuator.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\fltk\include\FL\Fl_Widget.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\fltk\include\FL\Fl_Window.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\fltk\include\FL\gl.h
# End Source File
# End Group
# End Group
# End Group
# Begin Group "externals Headers"

# PROP Default_Filter ""
# Begin Group "CbLib Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\externals\CbLib\CBL.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\externals\CbLib\CBLMacros.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\externals\CbLib\Functor0.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\externals\CbLib\Functor0R.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\externals\CbLib\Functor1.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\externals\CbLib\Functor1R.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\externals\CbLib\Functor2.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\externals\CbLib\Functor2R.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\externals\CbLib\Functor3.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\externals\CbLib\Functor3R.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\externals\CbLib\Functor4.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\externals\CbLib\Functor4R.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\externals\CbLib\FunctorBase.hxx
# End Source File
# End Group
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
# Begin Group "Processing Headers"

# PROP Default_Filter ""
# Begin Group "Base Headers No. 4"

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
# Begin Source File

SOURCE=..\..\..\..\src\Processing\Analysis\WindowGenerator.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\Analysis\WindowGeneratorConfig.hxx
# End Source File
# End Group
# Begin Group "ArithOps Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\src\Processing\ArithOps\AudioMultiplier.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\ArithOps\BinaryAudioOp.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\ArithOps\SpectrumProduct.hxx
# End Source File
# End Group
# Begin Group "Transformations Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\src\Processing\Transformations\FDFilterGen.hxx
# End Source File
# End Group
# End Group
# Begin Group "fftw Headers"

# PROP Default_Filter ""
# Begin Group "include Headers No. 1"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\..\fftw\include\fftw.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\fftw\include\rfftw.h
# End Source File
# End Group
# End Group
# Begin Group "xercesc Headers"

# PROP Default_Filter ""
# Begin Group "include Headers No. 2"

# PROP Default_Filter ""
# Begin Group "xercesc Headers No. 1"

# PROP Default_Filter ""
# Begin Group "util Headers No. 1"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\..\xercesc\include\xercesc\util\PlatformUtils.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\xercesc\include\xercesc\util\XercesDefs.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\xercesc\include\xercesc\util\XMemory.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\xercesc\include\xercesc\util\XMLString.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\xercesc\include\xercesc\util\XMLUni.hpp
# End Source File
# End Group
# Begin Group "parsers Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\..\xercesc\include\xercesc\parsers\XercesDOMParser.hpp
# End Source File
# End Group
# Begin Group "framework Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\..\xercesc\include\xercesc\framework\MemBufFormatTarget.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\xercesc\include\xercesc\framework\MemBufInputSource.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\xercesc\include\xercesc\framework\XMLFormatter.hpp
# End Source File
# End Group
# Begin Group "sax Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\..\xercesc\include\xercesc\sax\DocumentHandler.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\xercesc\include\xercesc\sax\DTDHandler.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\xercesc\include\xercesc\sax\EntityResolver.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\xercesc\include\xercesc\sax\ErrorHandler.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\xercesc\include\xercesc\sax\HandlerBase.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\xercesc\include\xercesc\sax\SAXException.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\xercesc\include\xercesc\sax\SAXParseException.hpp
# End Source File
# End Group
# Begin Group "dom Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\..\xercesc\include\xercesc\dom\DOMCharacterData.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\xercesc\include\xercesc\dom\DOMDocument.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\xercesc\include\xercesc\dom\DOMDocumentRange.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\xercesc\include\xercesc\dom\DOMDocumentTraversal.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\xercesc\include\xercesc\dom\DOMElement.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\xercesc\include\xercesc\dom\DOMErrorHandler.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\xercesc\include\xercesc\dom\DOMException.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\xercesc\include\xercesc\dom\DOMImplementation.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\xercesc\include\xercesc\dom\DOMImplementationLS.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\xercesc\include\xercesc\dom\DOMImplementationRegistry.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\xercesc\include\xercesc\dom\DOMNamedNodeMap.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\xercesc\include\xercesc\dom\DOMNode.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\xercesc\include\xercesc\dom\DOMNodeFilter.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\xercesc\include\xercesc\dom\DOMNodeList.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\xercesc\include\xercesc\dom\DOMRangeException.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\xercesc\include\xercesc\dom\DOMText.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\xercesc\include\xercesc\dom\DOMWriter.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\xercesc\include\xercesc\dom\DOMWriterFilter.hpp
# End Source File
# End Group
# End Group
# End Group
# End Group
# End Group
# Begin Source File

SOURCE=settings.cfg

!IF  "$(CFG)" == "SinglePlot - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "SinglePlot - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# End Target
# End Project
