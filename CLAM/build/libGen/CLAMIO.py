#!/usr/bin/python

import AutoconfTools, SettingsGen, sys, os

def makeAutoconf( outPath ) :
    outPath = "../Libs/%s"%"IO"

    if not os.path.exists( outPath ) :
        os.makedirs( outPath )

    script = AutoconfTools.AutoconfScript("CLAMIO","0.7")
    script.descriptionText = "The CLAM Framework Input/Output facilities"
    script.preinclude = "CLAM/Core/preinclude.hxx"
    script.dependsOn( "CLAMCore" )
    script.dependsOn( "CLAMProcessing" )

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


    libSndFile = AutoconfTools.Library( "sndfile", "C" )
    libSndFile.sandboxName = "sndfile"
    libSndFile.headers = "sndfile.h"
    libSndFile.libs = "sndfile"
    libSndFile.function = "sf_open"

    script.addLibrary( libSndFile )

    libMad = AutoconfTools.Library( "mad", "C" )
    libMad.sandboxName ="libmad"
    libMad.headers="mad.h"
    libMad.libs = "mad"
    libMad.function = "mad_stream_init"

    script.addLibrary( libMad )

    libOgg = AutoconfTools.Library( "ogg", "C" )
    libOgg.sandboxName = "oggvorbis"
    libOgg.headers = "ogg/ogg.h"
    libOgg.libs = "ogg"
    libOgg.function = "ogg_sync_init"

    script.addLibrary( libOgg ) 

    libVorbis = AutoconfTools.Library( "vorbis", "C" )
    libVorbis.sandboxName = "oggvorbis"
    libVorbis.headers = "vorbis/codec.h"
    libVorbis.libs = "vorbis"
    libVorbis.function = "vorbis_block_init"

    script.addLibrary( libVorbis )

    libVorbisEnc = AutoconfTools.Library( "vorbisenc", "C")
    libVorbisEnc.sandboxName = "oggvorbis"
    libVorbisEnc.headers="vorbis/vorbisenc.h"
    libVorbisEnc.libs="vorbisenc"
    libVorbisEnc.function= "vorbis_encode_init"
    
    script.addLibrary( libVorbisEnc )

    libVorbisFile = AutoconfTools.Library("vorbisfile","C")
    libVorbisFile.sandboxName="oggvorbis"
    libVorbisFile.headers="vorbis/vorbisfile.h"
    libVorbisFile.libs = "vorbisfile"
    libVorbisFile.function = "ov_open"

    script.addLibrary( libVorbisFile )

    id3lib = AutoconfTools.Library( "id3lib", "C++" )
    id3lib.sandboxName = "id3lib"
    id3lib.headers = "id3/tag.h"
    id3lib.libs = "id3"
    id3lib.function = "ID3Tag::~ID3Tag"
    id3lib.source="""
#include <id3/tag.h>
		
int main( int argc, char** argv )
{
    ID3_Tag myTag;

    return 0;
}
    
"""

    script.addLibrary( id3lib ) 
    
    if sys.platform == "linux2" :
        libALSA = AutoconfTools.Library( "alsa", "C" )
        libALSA.headers = "alsa/asoundlib.h"
        libALSA.libs = "asound"
        libALSA.function = "snd_pcm_open"
        
        script.addLibrary( libALSA )
    
    libPortmidi = AutoconfTools.Library( "portmidi", "C", False )
    libPortmidi.sandboxName = "portmidi"
    libPortmidi.headers = "portmidi.h"
    libPortmidi.libs = "portmidi"
    libPortmidi.other_libs = "-lporttime"
    libPortmidi.function = "Pm_CountDevices"
    libPortmidi.source = """
#include <portmidi.h>

int main( )
{
    Pm_CountDevices();
    return 0;
}
"""
    script.addLibrary( libPortmidi )

    script.commitToFile( outPath )
   
    script.copySupportFiles( outPath )



def makeSettings( outPath ) :
    libCLAMIO = SettingsGen.LibGenerator( "IO" )

    if not os.path.exists( outPath ) :
        os.makedirs( outPath )

    print "Generating libCLAM%s..."%libCLAMIO.libName

    libCLAMIO.addFolder( "Tools/AudioFileIO" )
    libCLAMIO.addFolder( "Processing/AudioFileIO" )


    if sys.platform == "linux2" :
        libCLAMIO.blackBall( "RtAudio" )
        libCLAMIO.blackBall( "RtAAudioDevice" )
        libCLAMIO.blackBall( "foo" )
        libCLAMIO.condAddFolder( "alsa","Tools/AudioIO/Linux" )
    else :
        libCLAMIO.blackBall( "ALSAAudioDevice" )
        libCLAMIO.blackBall( "foo" )
        libCLAMIO.blackBall( "SndPcm" )
        libCLAMIO.addFolder( "Tools/AudioIO/RtAudio" )

    libCLAMIO.addFolder( "Tools/AudioIO" )
    libCLAMIO.addFolder( "Processing/AudioIO" )

    libCLAMIO.addFolder( "Tools/MIDIIO" )
    libCLAMIO.addFolder( "Tools/MIDIIO/File" )
    libCLAMIO.addFolder( "Tools/MIDIIO/TextFile" )
    if sys.platform == "win32" :
        libCLAMIO.addFolder( "Tools/MIDIIO/Portmidi" )
    else :
        libCLAMIO.condAddFolder( "alsa","Tools/MIDIIO/Linux" )
    libCLAMIO.addFolder( "Processing/MIDIIO" )

    libCLAMIO.addFolder( "Tools/SDIF" )
    libCLAMIO.addFolder( "Processing/SDIFIO" )
    
    libCLAMIO.dependsOn( "Core" )

    libCLAMIO.generateFiles( outPath )

    print "Files are being generated on %s/%s..."%(outPath,libCLAMIO.libName)
    
if __name__ == "__main__" :
    makeAutoconf("../Libs")
    makeSettings("../Libs")


    
