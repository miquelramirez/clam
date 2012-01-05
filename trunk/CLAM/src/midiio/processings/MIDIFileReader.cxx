#include "MIDITrack.hxx"
#include "MIDISong.hxx"
#include "MIDIReader.hxx"
#include "MIDITempo.hxx"
#include "MIDIFileReader.hxx"

namespace CLAM
{
	MIDIFileReader::MIDIFileReader()
		: mOutput("MIDISong Output",this)
	{
		MIDIFileIOConfig cfg;
		Configure(cfg);
	}

	MIDIFileReader::MIDIFileReader(const MIDIFileIOConfig& cfg)
		: mOutput("MIDISong Output",this)
	{
		Configure(cfg);
	}

	MIDIFileReader::~MIDIFileReader()
	{
	}

	bool MIDIFileReader::Do()
	{
		bool result = Do(mOutput.GetData());
		mOutput.Produce();
		return result;
	}

	bool MIDIFileReader::Do(MIDISong& out)
	{
		MIDI::Reader r(mConfig.GetFileName().c_str());
		MIDI::Song s;
		r.Read(s);
		MIDI::Tempo t(&s);
		static int nbytesPerChnMsg[7] = { 3,3,3,3,2,3,3 };
		bool onset=true;
		int nTracks = s.Tracks();
		for(int i=0; i < nTracks; i++)
		{
			MIDI::Track* track = s.GetTrack(i);
			MIDI::Track::EventIterator it = track->Begin();
			MIDITrack  midiTrack;
			for(;it != track->End(); it++)
			{
				const MIDI::Event &ev = **it;
				int type = ((ev[0]&0xF0)>>4)-8;
				if(type==7) continue;
				if(((ev[0]&0xF0)==0x90) || ((ev[0]&0xF0)==0x80))
				{
					onset = ((ev[0]&0xF0)==0x90) ? (ev[2] > 0) : false; // velocity > 0 -> onset
					if(onset)
					{
						midiTrack.GetChannels().AddElem((int)(ev[0]&0x0F));
						MIDINote note;
						note.GetTime().SetBegin(TData(t.TicksToTime(ev.GetTicks())/1000.0));
						note.GetTime().SetEnd(TData(-1.0));
						note.SetKey((int)ev[1]);
						note.SetVelocity((int)ev[2]);
						midiTrack.GetTrackMelody().GetNoteArray().AddElem(note);
						midiTrack.GetTrackMelody().GetNumberOfNotes()++;
					}
					else
					{
						for(int n=0; n < midiTrack.GetTrackMelody().GetNumberOfNotes(); n++)
						{
							if((midiTrack.GetTrackMelody().GetNoteArray()[n].GetKey() == (int)(ev[1])) && 
							   (midiTrack.GetTrackMelody().GetNoteArray()[n].GetTime().GetEnd()==TData(-1.0)))
							{
								midiTrack.GetTrackMelody().GetNoteArray()[n].GetTime().SetEnd(TData(t.TicksToTime(ev.GetTicks())/1000.0));
								break;
							}
						}
					}
				}
				else
				{
					MIDIEvent e;
					e.SetName(GetEventName((ev[0]&0xF0)));
					e.SetTime((TTime)(t.TicksToTime(ev.GetTicks())/1000.0));
					int len = nbytesPerChnMsg[type];
					for(int k=0; k < len; k++)
					{
						e.GetMessage().AddElem(ev[k]);
					}
					midiTrack.GetEventInfo().AddElem(e);
				}			
			}			
			if(midiTrack.GetTrackMelody().GetNumberOfNotes() > 0)
			{
				out.GetTracks().AddElem(midiTrack);
				out.GetNumberOfTracks()++;
			}
		}
		return true;
	}

	bool MIDIFileReader::ConcreteConfigure(const ProcessingConfig& c)
	{
		CopyAsConcreteConfig(mConfig, c);
		return true;
	}

	std::string MIDIFileReader::GetEventName(TMIDIByte b)
	{
		switch(b)
		{
			case 0xA0:
				return "PolyAftertouch";
				break;
			case 0xB0:
				return "ControlChange";
				break;
			case 0xC0:
				return "ProgramChange";
				break;
			case 0xD0:
				return "ChnAftertouch";
			case 0xE0:
				return "PitchBend";
				break;
			default:
				return "Unknow";
		}
	}
	   
}

// END

