
#include "InitAudioIO.hxx"
#include <iostream>

#include <CLAM/AudioIn.hxx>
#include <CLAM/AudioOut.hxx>
#include <CLAM/MonoAudioFileReader.hxx>
#include <CLAM/MonoAudioFileWriter.hxx>
#include <CLAM/MultiChannelAudioFileReader.hxx>
#include <CLAM/MultiChannelAudioFileWriter.hxx>
#include "RtAAudioDevice.hxx"

namespace CLAM
{
	void AudioIOModule::init()
	{

		std::cout << "Initiating audio module..."; 

		RtAAudioDeviceList::init();
		
		MonoAudioFileReader monoReader;
		MultiChannelAudioFileReader multiReader;
		MultiChannelAudioFileWriter multiWriter;
		MonoAudioFileWriter monoWriter;
		AudioIn audioIn;
		AudioOut audioOut;

		// crashes
		//CLAM::MIDIDispatcher midiDispatcher;
		//CLAM::MIDIInControl midiInControl;
		//CLAM::MIDIKeyboard midiKeyboard;
		std::cout << "done!" << std::endl;
	}
}

