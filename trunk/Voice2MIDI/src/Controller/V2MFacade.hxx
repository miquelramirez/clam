/*
 * V2MFacade.hxx
 * Interface for V2MFacade class
 *
 * This class implements a facade controller (see Facade pattern),
 * which allows the Model-View separation.
 *
 * Ismael Mosquera Rivera PFC Voice2MIDI UPF 2004
*/
#ifndef _V2MFACADE_
#define _V2MFACADE_

#include <list>
#include <string>
#include "V2MAnalysis.hxx"
#include "V2MAudioIO.hxx"
#include "V2MMIDIOut.hxx"
#include "SharedTypes.hxx"
#include <CLAM/Signalv1.hxx>
#include <CLAM/Slotv1.hxx>
#include "FundPlayer.hxx"
#include "MIDIMelodyPlayer.hxx"
#include "AudioPlayer.hxx"
#include "MIDIFileWriter.hxx"

using SigSlot::Signalv1;
using SigSlot::Slotv1;

enum pMode{ pmAudio = 0, pmMIDI, pmFundamental, pmFundamentalSeg, pmNone };

class V2MFacade
{
public:
	V2MFacade(Slotv0& slot0,
		      Slotv1<int>& slot1,
			  Slotv1<DataArray>& slot2);
	~V2MFacade();


	int LoadAudio(const char* filename);
	int SaveAudio(const char* filename);

	void Play(pMode);
	void Stop();
	void Rec();

	void Analyze();
	void AnalyzeMelody();
        
	void StoreMelody(const std::string& filename);

	TSize GetAnalysisSteps();
	const std::list<DisplayData> GetDisplayData(bool audio = true);
    Melody GetMelody();
	void SetMelody(const Melody& melody);
	MIDIMelody GetMIDIMelody();
	void SetMelodyTmp(const Melody& melodyTmp);
	void SetMIDIMelodyTmp(const MIDIMelody& midiMelodyTmp);
	void NeedUpdatedMelody(bool up);

	const std::list<std::string> GetMIDIDeviceNames();
	const std::list<std::string> GetAvailableMIDIPrograms();
	void SetMIDIDevice(int index);
	void SetMIDIProgram(int index);

	void SaveMIDI(const std::string& filename,int prog);
	void StoreMIDIMelody(const std::string& filename);

private:
	VoiceAnalyzerConfig cfg;
	VoiceAnalyzer analyzer;

	MelodyAnalyzer melodyAnalyzer;
	Melody _melodyTmp;
	MIDIMelody _midiMelodyTmp;

	bool _needUpdatedMelody;

	V2M::AudioPlayer* audioPlayer;
	V2M::FundPlayer* fundPlayer;
	V2M::AudioRecorder* recorder;
	V2M::FundSegPlayer* fundSegPlayer;
	V2M::MIDIMelodyPlayer* midiPlayer;

	V2M::AudioFileIO wFileIO;

	V2M::MIDIFileWriter mfWriter;

	Segment mSegment;
	Frame mFrame;

	Slotv0 mStopSlot;
	Slotv1<DataArray> mDynamicReceptor;
	Signalv1<int> mRequestUpdateProgressBar;

	std::string midi_dev[50];
	std::list<std::string> _midi_device_list;
	std::string _midi_device;
	int available_midi_programs[7];
	int _midi_program;

	void AttachAudioFromRecorder();
	void InitSegment();
	void UpdateSegment();
	bool NeedUpdatedMelody();

	void GetMIDIDevices();
	void GetMIDIPrograms();
	void InitMIDI();
};

#endif

