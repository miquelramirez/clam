#!/usr/bin/python

import AutoconfTools, SettingsGen, sys, os

def makeAutoconf( outPath ) :
    outPath = "../Libs/%s"%"Processing"

    if not os.path.exists( outPath ) :
        os.makedirs( outPath )

    script = AutoconfTools.AutoconfScript("CLAMProcessing","0.7")

    script.descriptionText = "The CLAM Framework DSP algorithms repository"
    script.preinclude = "CLAM/Core/preinclude.hxx"
    script.dependsOn( "CLAMCore" )

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


    libfftw = AutoconfTools.Library("fftw","C")
    libfftw.sandboxName="fftw"
    libfftw.headers="fftw.h"
    libfftw.libs="fftw"
    libfftw.other_libs="-lrfftw"
    libfftw.function="fftw_sizeof_fftw_real"
    libfftw.source="""
#include <fftw.h>
int main()
{
    fftw_sizeof_fftw_real();
    return 0;
}
    """
    script.addLibrary( libfftw )

    
    script.commitToFile( outPath )
    script.copySupportFiles( outPath )


def makeSettings( outPath ) :
    if not os.path.exists( outPath ) :
        os.makedirs( outPath )

    libCLAMProcessing = SettingsGen.LibGenerator( "Processing" )

    print "Generating libCLAM%s..."%libCLAMProcessing.libName
    # Depends on CUIDADO code
    libCLAMProcessing.blackBall( "OnsetDetector" )
    libCLAMProcessing.blackBall( "OnsetDetectorConfig" )
    libCLAMProcessing.blackBall( "GT_FilterBank" )
    libCLAMProcessing.blackBall( "ERB_Space_Gen" )
    libCLAMProcessing.blackBall( "SMSAnalysis" )
    libCLAMProcessing.blackBall( "SMSAnalysisConfig" )
    libCLAMProcessing.blackBall( "SMSAnalysisCore" )
    libCLAMProcessing.blackBall( "SMSSynthesis" )
    libCLAMProcessing.blackBall( "SMSSynthesisConfig" )
    
    # This cannot work on a pre-compiled library...
    # as it only works/makes sense when converting CLAM
    # processings into LADSPA plug-ins
    libCLAMProcessing.blackBall( "LadspaBridge" )

    libCLAMProcessing.addFile( "DefaultProcessingFactoryRegistration", "Processing/Base" )
    libCLAMProcessing.addFile( "ScaleCnv", "Standard" )
    libCLAMProcessing.addFolder( "Data/BasicProcessing" )
    libCLAMProcessing.addFolder( "Data/Descriptors" )
    libCLAMProcessing.addFolder( "Data/Descriptors/Melody" )

    libCLAMProcessing.addFolder( "Processing/Analysis" )
    libCLAMProcessing.addFolder( "Processing/ArithOps" )
    libCLAMProcessing.addFolder( "Processing/Controls" )
    libCLAMProcessing.addFolder( "Processing/Generators" )
    libCLAMProcessing.addFolder( "Processing/Plugins" )
    libCLAMProcessing.addFolder( "Processing/Synthesis" )
    libCLAMProcessing.addFolder( "Processing/Transformations" )
    
    libCLAMProcessing.addFolder( "Flow/Ports")
    libCLAMProcessing.addFolder( "numrec", "externals")

    libCLAMProcessing.dependsOn( "Core" )

    libCLAMProcessing.generateFiles(outPath)
    print "Files are being generated on %s/%s..."%(outPath,libCLAMProcessing.libName)

if __name__ == "__main__" :
    makeAutoconf("../Libs")
    makeSettings("../Libs")
