#!/usr/bin/python

import SettingsGen, os
import AutoconfTools

def makeAutoconf( outPath ) :
    outPath = "../Libs/%s"%"Core"
    
    if not os.path.exists( outPath ) :
        os.makedirs( outPath )
    
    script = AutoconfTools.AutoconfScript("CLAMCore","0.7")

    script.descriptionText = "The CLAM Framework core services and classes"
    script.preinclude = "CLAM/Core/preinclude.hxx"

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

    libxercesc = AutoconfTools.Library( "xercesc", "C++" )
    libxercesc.sandboxName = "xercesc"
    libxercesc.headers="xercesc/util/PlatformUtils.hpp"
    libxercesc.libs="xerces-c"
    libxercesc.function="xercesc_2_3::XMLPlatformUtils::Initialize"
    libxercesc.source="""
#include<xercesc/util/PlatformUtils.hpp>

int main( )
{
   namespace xercesc=XERCES_CPP_NAMESPACE;
   xercesc::XMLPlatformUtils::Initialize();

   return 0;
}

"""
    script.addLibrary( libxercesc )

    script.commitToFile( outPath )
    script.copySupportFiles( outPath )

def makeSettings( outPath ) :
    if not os.path.exists( outPath ) :
        os.makedirs( outPath )

    libCLAMCore = SettingsGen.LibGenerator("Core")

    print "Generating libCLAM%s..."%libCLAMCore.libName

    # These files are blackballed since they depend on
    # some concrete ProcessingData
    libCLAMCore.blackBall( "AudioInPort" )
    libCLAMCore.blackBall( "AudioInPortPublisher" )
    libCLAMCore.blackBall( "AudioOutPort" )
    libCLAMCore.blackBall( "AudioOutPortPublisher" )
    libCLAMCore.blackBall( "DefaultProcessingFactoryRegistration" )
    libCLAMCore.blackBall( "SMSProcessingFactoryRegistration" )
    libCLAMCore.blackBall( "ProcessingChain" )
    libCLAMCore.blackBall( "ScaleCnv" )

    libCLAMCore.addFolder( "Base" )

    libCLAMCore.addFolder( "Data/Base" )

    libCLAMCore.addFolder( "Defines" )
    libCLAMCore.addFolder( "Defines/Windows" )    
    libCLAMCore.addFolder( "Defines/Unix" )

    libCLAMCore.addFolder( "Errors" )

    libCLAMCore.addFolder( "Flow/Ports" )
    libCLAMCore.addFolder( "Flow/Networks" )
    libCLAMCore.addFolder( "Flow/Controls" )

    libCLAMCore.addFolder( "Processing/Base" )
    
    libCLAMCore.addFolder( "Standard" )
    libCLAMCore.addFolder( "Storage/Base" )
    libCLAMCore.condAddFolder( "xercesc", "Storage/XML" )
    libCLAMCore.addFolder( "System" )
    libCLAMCore.addFolder( "System/Threads" )
    libCLAMCore.addFolder( "CbLib", "externals" )

    libCLAMCore.generateFiles( outPath )
    print "Files are being generated on %s/%s..."%(outPath,libCLAMCore.libName)

if __name__ == "__main__" :

    makeAutoconf("../Libs" )
    makeSettings("../Libs" )
