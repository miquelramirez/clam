
This directory contains a moderately simple spectral modeling synthesizer. It can be run both as plugin in the network editor or as a standalone command line application can be fed either notes from a score file or events from an OSC stream. 

To use this as a plugin in the NetworkEditor, type "sudo scons install" at the command line, and when inside the NetworkEditor, right click on the background until you get the "Add New Processing" dialog, and in this dialog enter the name "SDIFDatabaseProcessing". You'll need to configure the object once it appears with the name of a directory containing SDIF files and metadata about these files in XML format. Alternatively, you could also just load one of the two clam networks that were provided as examples in this directory.

To run this code as a standalone, type "scons" to build the executable, and then ./ContinuousExcitationSynthesizer  The application itself should give you instructions on what arguments it takes.

Download or checkout using subversion the following directory:
http://clam-project.org/clam_data/trunk/OboeSDIF/

Alternatively, you can also make your own database of SDIF files using the tool loopMaker in CLAM/examples. This creates the SDIF files as well as the XML metadata. See that application's README for more information.
Or yet another alternative is to create your own SDIF files using some other SDIF tool. (My personal favorite is SPEAR.) And to create the metadata by hand by editing the metadata template included in this directory.



