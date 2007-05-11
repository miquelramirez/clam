/*
 *	V2MFacade.cxx
 *	Implementation for V2MFacade class.
 *
 *	Ismael Mosquera Rivera PFC Voice2MIDI UPF 2004
*/
#include <vector>
#include <CLAM/MIDIManager.hxx>
#include "MIDISettings.hxx"
#include "V2MFacade.hxx"

#ifdef USE_PORTMIDI
#include <portmidi.h>
#endif
using namespace CLAM;

V2MFacade::V2MFacade(Slotv0& slot0,Slotv1<int>& slot1,Slotv1<DataArray>& slot2) 
                     : mStopSlot(slot0),mDynamicReceptor(slot2), 
		     audioPlayer(0), fundPlayer(0), fundSegPlayer(0), recorder(0),
		     midiPlayer(0), _needUpdatedMelody(false) 
{
	mRequestUpdateProgressBar.Connect(slot1);
	mFrame.AddAudioFrame();
	mFrame.UpdateData();

	mSegment.AddAudio();
	mSegment.UpdateData();

	InitMIDI();
	_midi_device = "default";
	_midi_program = 0;
}

V2MFacade::~V2MFacade()
{ 
	if(audioPlayer) delete audioPlayer;
	if(fundPlayer) delete fundPlayer;
	if(fundSegPlayer) delete fundSegPlayer;
	if(recorder) delete recorder;
	if(midiPlayer) delete midiPlayer;
}

void V2MFacade::Play(pMode mode)
{
	switch(mode)
	{
		case pmAudio:						
	                audioPlayer = new V2M::AudioPlayer(mSegment.GetAudio(),mStopSlot,mDynamicReceptor);
			break;
		case pmFundamental:	
			fundPlayer = new V2M::FundPlayer(mSegment,mStopSlot,mDynamicReceptor);
			break;
		case pmFundamentalSeg:
			if(!NeedUpdatedMelody())
				fundSegPlayer = new V2M::FundSegPlayer(GetMelody(),
									mSegment.GetAudio().GetSampleRate(),
									mSegment.GetAudio().GetSize(),
									mStopSlot,mDynamicReceptor); 
			else
							fundSegPlayer = new V2M::FundSegPlayer(_melodyTmp,
								                              mSegment.GetAudio().GetSampleRate(),
															  mSegment.GetAudio().GetSize(),
															  mStopSlot,mDynamicReceptor); 
						break;

		case pmMIDI:
						if(!NeedUpdatedMelody())
							midiPlayer = new V2M::MIDIMelodyPlayer(GetMIDIMelody(),
															  _midi_device,
															  _midi_program,
															  mStopSlot);
						else
							midiPlayer = new V2M::MIDIMelodyPlayer(_midiMelodyTmp,
															  _midi_device,
															  _midi_program,
															  mStopSlot);

						break;
		case pmNone:
			            // do nothing
						break;
	}
}

void V2MFacade::Stop()
{
	if(audioPlayer)
	{
		audioPlayer->Stop();
		delete audioPlayer;
		audioPlayer = NULL;
	}
	if(fundPlayer)
	{
		fundPlayer->Stop();
		delete fundPlayer;
		fundPlayer = NULL;
	}
	if(fundSegPlayer)
	{
		fundSegPlayer->Stop();
		delete fundSegPlayer;
		fundSegPlayer = NULL;
	}
	if(midiPlayer)
	{
		midiPlayer->Stop();
		delete midiPlayer;
		midiPlayer = NULL;
	}

	if(recorder)
	{
		AttachAudioFromRecorder();
		recorder->Stop();
		delete recorder;
		recorder = NULL;
	}
}

void V2MFacade::Rec()
{
	recorder = new V2M::AudioRecorder(mDynamicReceptor); 
}

void V2MFacade::AttachAudioFromRecorder()
{
	// get audio from recorder
	mSegment.SetAudio(recorder->GetAudio()); 
}

void V2MFacade::Analyze()
{
	InitSegment();

	TSize wSize = 513;
	TSize hopSize = 256;
	TData sampleRate = mSegment.GetSamplingRate();
	
	cfg.SetWindowSize(wSize);
	cfg.SetZeroPadding(2);
	cfg.SetSamplingRate(sampleRate);
	
	analyzer.Configure(cfg);

	analyzer.Start();

	TSize nSamples = mSegment.GetAudio().GetSize();
	TSize frameSize = cfg.GetWindowSize();

	mFrame.SetDuration(TData(hopSize/sampleRate));
	mFrame.GetAudioFrame().SetSize(frameSize);    
  
	TIndex leftIndex = 0;
	TIndex rightIndex = frameSize;
	
	while(leftIndex < nSamples)
	{
		mSegment.GetAudio().GetAudioChunk(leftIndex,rightIndex,mFrame.GetAudioFrame()); // get audio chunk
		analyzer.Do(mFrame);          // process
		mSegment.AddFrame(mFrame);    // add frame to segment
		leftIndex += hopSize;
		rightIndex = leftIndex + frameSize;

		// signal for update the progress bar
		mRequestUpdateProgressBar.Emit(int(leftIndex));
	}
         
	analyzer.Stop();
	// update segment
	mSegment.SetEndTime(mSegment.GetAudio().GetSize()/mSegment.GetAudio().GetSampleRate());
}

void V2MFacade::InitSegment()
{
	if(mSegment.GetnFrames() > 0)
	{
		mSegment.RemoveFramesArray();
		mSegment.UpdateData();
	}
 
	mSegment.AddFramesArray();
	mSegment.UpdateData();

	mSegment.SetBeginTime(0);
	mSegment.SetSamplingRate(mSegment.GetAudio().GetSampleRate());
}

void V2MFacade::AnalyzeMelody()
{
	melodyAnalyzer.AnalyzeMelody(mSegment,analyzer.GetDescriptors());
}

void V2MFacade::StoreMelody(const std::string& filename)
{
	melodyAnalyzer.StoreMelody(filename);
}
void V2MFacade::StoreMIDIMelody(const std::string& filename)
{
	melodyAnalyzer.StoreMIDIMelody(filename);
}

int V2MFacade::LoadAudio(const char* filename)
{
	return wFileIO.Load(filename,mSegment.GetAudio());
}

int V2MFacade::SaveAudio(const char* filename)
{
	return wFileIO.Save(filename,mSegment.GetAudio());
}

const std::list<DisplayData> V2MFacade::GetDisplayData(bool audio)
{
    TData sampleRate = mSegment.GetAudio().GetSampleRate();
    std::list<DisplayData> data;
    DisplayData displayData;
	displayData.left = 0.0;
    if(audio)
    {
		// audio
		displayData.data = mSegment.GetAudio().GetBuffer();
		displayData.right = mSegment.GetAudio().GetSize();
		displayData.bottom = -1.0;
		displayData.top = 1.0;
		displayData.sampleRate = sampleRate;
		data.push_back(displayData);
    }
    
    if(!audio)
    {
		int i,j;
		int nFrames = mSegment.GetnFrames();
		TSize nSamples = mSegment.GetAudio().GetSize();
		TSize step = nSamples/nFrames;
		// energy
		SegmentDescriptors descriptors = analyzer.GetDescriptors();
		TData energy;
		DataArray values;
		values.Resize(nSamples);  
		values.SetSize(nSamples);  
		TData max = 0.0;
		displayData.top = 0.0;
		int k=0;
		for(i=0;i < mSegment.GetnFrames();i++)
		{
			energy = descriptors.GetFrameD(i).GetSpectrumD().GetEnergy();
			for(j = 0; j < step;j++)
			{
				values[k++]=energy;
			} 
			max = (max < energy) ? energy : max;
		}
		displayData.data = values;
		displayData.right = nSamples;
		displayData.bottom = 0.0;
		displayData.top = max*1.5;
		data.push_back(displayData);
		// fundamental
		TData freq = 0.0;
		max = 0.0;
		k=0;
		for(i=0;i < nFrames;i++)
		{
			freq = mSegment.GetFrame(i).GetFundamental().GetFreq(0);
			for(j = 0; j < step;j++)
			{
				values[k++]=freq;
			}
			max = (max < freq) ? freq : max;
		}
		displayData.data = values;
		displayData.top = max*1.2;
		data.push_back(displayData);	
	}
 
   return data;
}

Melody V2MFacade::GetMelody()
{
	return melodyAnalyzer.GetMelody();
}
void V2MFacade::SetMelody(const Melody& melody)
{
	melodyAnalyzer.SetMelody(melody);
	_needUpdatedMelody = false;
}
MIDIMelody V2MFacade::GetMIDIMelody()
{
	return melodyAnalyzer.GetMIDIMelody();
}
TSize V2MFacade::GetAnalysisSteps()
{
	return mSegment.GetAudio().GetSize();
}
bool V2MFacade::NeedUpdatedMelody()
{
	return _needUpdatedMelody;
}
void V2MFacade::NeedUpdatedMelody(bool up)
{
	_needUpdatedMelody = up;
}
void V2MFacade::SetMelodyTmp(const Melody& melodyTmp)
{
	_melodyTmp = melodyTmp;
	_needUpdatedMelody = true;
}
void V2MFacade::SetMIDIMelodyTmp(const MIDIMelody& midiMelodyTmp)
{
	_midiMelodyTmp = midiMelodyTmp;
	_needUpdatedMelody = true;
}
void V2MFacade::GetMIDIDevices()
{
	CLAM::VM::MIDISettings settings;
	std::vector<std::string> midiDevices = settings.GetMIDIDevices();
	if(midiDevices.size())
	{
    		int i = 0;
		int num_devices = 0;

		std::vector<std::string>::iterator v_it = midiDevices.begin();
		for(;v_it != midiDevices.end();v_it++,i++) 
		{
#ifdef USE_PORTMIDI
        		const PmDeviceInfo *info = Pm_GetDeviceInfo(i);
			if(info->output)
			{
				midi_dev[num_devices++]=(*v_it);
				_midi_device_list.push_back(info->name);
			}		
#else
			midi_dev[num_devices++]=(*v_it);
			_midi_device_list.push_back((*v_it));
#endif
		}
	}

	/*	
	std::cout << "BEGIN GET MIDI DEVICES" << std::endl;
	MIDIManager manager;	
	std::cout << "midi manager created" << std::endl;
	MIDIDeviceList* _devList = manager.FindList("default");
	std::cout << "midi device list found: " << _devList << std::endl;
	std::vector<std::string> lst = _devList->AvailableDevices();
	std::cout << "getting available devices" << std::endl;
	std::vector<std::string>::iterator v_it = lst.begin();
	for(;v_it != lst.end();v_it++,i++) 
	{
#ifdef USE_PORTMIDI			
        const PmDeviceInfo *info = Pm_GetDeviceInfo(i);
		if(info->output)
		{
			midi_dev[num_devices++]=(*v_it);
			_midi_device_list.push_back(info->name);
		}		
#else
		midi_dev[num_devices++]=(*v_it);
		_midi_device_list.push_back((*v_it));
#endif
	}
	*/
	std::cout << "END GET MIDI DEVICES" << std::endl;
}
const std::list<std::string> V2MFacade::GetMIDIDeviceNames()
{
	return _midi_device_list;	
}
void V2MFacade::SetMIDIDevice(int index)
{
	_midi_device = midi_dev[index];
}
void V2MFacade::SetMIDIProgram(int index)
{
	_midi_program = available_midi_programs[index];
}
void V2MFacade::GetMIDIPrograms()
{
	available_midi_programs[0] =  0; // Piano
	available_midi_programs[1] = 16; // Hammond Organ
	available_midi_programs[2] = 40; // Violin
	available_midi_programs[3] = 56; // Trumpet
	available_midi_programs[4] = 66; // Sax
	available_midi_programs[5] = 68; // Oboe
	available_midi_programs[6] = 75; // Flute
}
void V2MFacade::InitMIDI()
{
	GetMIDIDevices();
	GetMIDIPrograms();
}
const std::list<std::string> V2MFacade::GetAvailableMIDIPrograms()
{
	std::list<std::string> progList;
	progList.push_back("Piano");
	progList.push_back("Hammond Organ");
	progList.push_back("Violin");
	progList.push_back("Trumpet");
	progList.push_back("Sax");
	progList.push_back("Oboe");
	progList.push_back("Flute");
	return progList;
}

void V2MFacade::SaveMIDI(const std::string& filename,int prog)
{
	mfWriter.Write(GetMIDIMelody(),available_midi_programs[prog],filename);
}

// END

