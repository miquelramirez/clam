#!/usr/bin/python

import AutoconfTools, SettingsGen, sys, os

def makeAutoconf( outPath ) :
    outPath = "../Libs/%s"%"VM"
    
    if not os.path.exists( outPath ) :
        os.makedirs( outPath )
    
    script = AutoconfTools.AutoconfScript("CLAMVM","0.7")

    script.descriptionText = "The CLAM Framework Visualization Module"
    script.preinclude = "/CLAM/Core/preinclude.hxx"
    script.dependsOn( "CLAMProcessing" )
    script.dependsOn( "CLAMIO" )

    xml = AutoconfTools.Feature( "xml" )
    xml.addEffectWhenEnabled( "-DCLAM_USE_XML" )
    script.addFeature( xml )

    double = AutoconfTools.Feature( "double", False )
    double.addEffectWhenEnabled( "-DCLAM_DOUBLE" )
    double.addEffectWhenDisabled( "-DCLAM_FLOAT" )
    script.addFeature( double )

    checks = AutoconfTools.Feature( "checks" )
    checks.addEffectWhenDisabled( "-DCLAM_DISABLE_CHECKS" )
    script.addFeature( checks )

    releaseAsserts = AutoconfTools.Feature( "release_asserts", False )
    releaseAsserts.addEffectWhenEnabled( "-DCLAM_USE_RELEASE_ASSERTS" )
    script.addFeature( releaseAsserts )

    libfltk = AutoconfTools.Library("fltk", "C++")
    libfltk.sandboxName="fltk"
    libfltk.headers="FL/Fl.H"
    libfltk.libs="fltk"
    libfltk.other_libs="-lGL -lGLU -lz -ljpeg -lpng -lfltk_forms -lfltk_images -lfltk_gl"
    libfltk.function="fl_yes"
    libfltk.source="""
#include <FL/Fl.H>

int main( )
{
    Fl::run();
    return 0;
}
"""
    script.addLibrary( libfltk )

    script.commitToFile( outPath )
    script.copySupportFiles( outPath )


def makeSettings( outPath ) :
    libCLAMVM = SettingsGen.LibGenerator( "VM" )

    print "Generating libCLAM%s..."%libCLAMVM.libName

    # Files that do not compile
    libCLAMVM.blackBall( "TmplSlice" )
    libCLAMVM.blackBall( "SupportedToolkits" )
    libCLAMVM.blackBall( "Fl_Browsable_Playable_Audio" )
    libCLAMVM.blackBall( "Fl_Range" )
    # Deprecated stuff
    libCLAMVM.blackBall( "GLLinearSpRenderer" )
    libCLAMVM.blackBall( "GLLogPowSpRenderer" )
    libCLAMVM.blackBall( "GLLogSpRenderer" )
    libCLAMVM.blackBall( "GLPhaseSpRenderer" )
    libCLAMVM.blackBall( "GLSpectralBlock" )
    libCLAMVM.blackBall( "GLSprite" )
    libCLAMVM.blackBall( "GLTF2DRenderer" )
    libCLAMVM.blackBall( "GLWaveRenderer" )
    

    libCLAMVM.addFolder( "Visualization/Base" )
    libCLAMVM.addFolder( "Visualization/GL" )
    libCLAMVM.addFolder( "Visualization/Presentation" )
    libCLAMVM.addFolder( "Visualization/Plot" )
    libCLAMVM.addFolder( "Visualization/Presentation/Base" )
    libCLAMVM.condAddFolder( "fltk","Visualization/Presentation/FLTK" )
    libCLAMVM.addFolder( "Visualization/View" )
    libCLAMVM.addFolder( "Visualization/Util" )
    libCLAMVM.condAddFolder( "fltk", "Visualization/Widget/FLTK" )
    libCLAMVM.condAddFile( "fltk", "FLTKWrapper", "Visualization/Widget" )

    libCLAMVM.dependsOn( "Core" )
    libCLAMVM.dependsOn( "Processing" )
    libCLAMVM.dependsOn( "IO" )

    libCLAMVM.generateFiles(outPath)

    print "Files are being generated on %s/%s..."%(outPath,libCLAMVM.libName)

if __name__ == "__main__" :
    makeAutoconf( "../Libs" )
    makeSettings( "../Libs" )
