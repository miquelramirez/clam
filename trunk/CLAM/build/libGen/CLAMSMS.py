#!/usr/bin/python

import SettingsGen, sys, os, AutoconfTools

def makeAutoconf( outPath ) :
    outPath = "../Libs/%s"%"SMS"

    if not os.path.exists( outPath ) :
        os.makedirs( outPath )

    script = AutoconfTools.AutoconfScript("CLAMSMS","0.7")

    script.descriptionText = "The CLAM Framework SMS Analysis/Synthesis/Transformation package"
    script.preinclude = "CLAM/Core/preinclude.hxx"
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

    script.commitToFile( outPath )
   
    script.copySupportFiles( outPath )


def makeSettings( outPath) :
    if not os.path.exists( outPath ) :
        os.makedirs( outPath )

    libCLAMSMS = SettingsGen.LibGenerator( "SMS" )

    print "Generating libCLAM%s..."%libCLAMSMS.libName

    libCLAMSMS.addFile( "SMSAnalysisCore", "Processing/Analysis" )
    libCLAMSMS.addFile( "SMSAnalysisConfig","Processing/Analysis")
    libCLAMSMS.addFile( "SMSAnalysis", "Processing/Analysis" )
    libCLAMSMS.addFile( "SMSSynthesisConfig", "Processing/Synthesis" )
    libCLAMSMS.addFile( "SMSSynthesis", "Processing/Synthesis" )

    libCLAMSMS.addFile( "SMSProcessingFactoryRegistration", "Processing/Base" )
    libCLAMSMS.addFile( "ProcessingChain", "Processing/Base" )
    
    libCLAMSMS.addFolder( "Processing/Transformations/SMS" )

    libCLAMSMS.dependsOn( "Core" )
    libCLAMSMS.dependsOn( "Processing" )
    libCLAMSMS.dependsOn( "IO" )

    libCLAMSMS.generateFiles( outPath )

    print "Files are being generated on %s/%s..."%(outPath,libCLAMSMS.libName)

if __name__ == "__main__" :
    makeSettings("../Libs")
