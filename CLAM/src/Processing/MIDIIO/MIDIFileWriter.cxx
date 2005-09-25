#include "MIDITrack.hxx"
#include "MIDISong.hxx"
#include "MIDIWriter.hxx"
#include "MIDIFileWriter.hxx"

namespace CLAM
{
	MIDIFileWriter::MIDIFileWriter()
		: mInput("MIDISong Input",this)
	{
		MIDIFileIOConfig cfg;
		Configure(cfg);
	}

	MIDIFileWriter::MIDIFileWriter(const MIDIFileIOConfig& cfg)
		: mInput("MIDISong Input",this)
	{
		Configure(cfg);
	}

	MIDIFileWriter::~MIDIFileWriter()
	{
	}

	bool MIDIFileWriter::Do()
	{
		bool result = Do(mInput.GetData());
		mInput.Consume();
		return result;
	}

	bool MIDIFileWriter::Do(const MIDISong& in)
	{
		MIDI::Song s;
		s.SetTicksPerQ(96);
		// add tempo 
		MIDI::Track* tempo_track = new MIDI::Track;
		s.AddTrack(tempo_track);
		MIDI::MetaEvent* m_ev = new MIDI::MetaEvent(MIDI::Message(0xFF,0x51,0x03),0,3);
		m_ev->mData[0]=0x07; m_ev->mData[1]=0xA1; m_ev->mData[2]=0x20;
		tempo_track->Add(m_ev);
		
		int nTracks = in.GetNumberOfTracks();
		for(int i=0; i < nTracks; i++)
		{
			MIDI::Track* track = new MIDI::Track;
			s.AddTrack(track);
			BuildEventList(in.GetTracks()[i]);
			std::list<MIDIEvent>::const_iterator it = mEventList.begin();
			for(; it != mEventList.end(); it++)
			{
				const MIDIEvent &ev = *it;
				// add event to current track
				if(ev.Length()==2)
				{
					track->Add(new MIDI::Event(MIDI::Message(ev.GetMessage()[0], ev.GetMessage()[1]), ev.Ticks()));
				}
				else
				{
					track->Add(new MIDI::Event(MIDI::Message(ev.GetMessage()[0],ev.GetMessage()[1],ev.GetMessage()[2]), ev.Ticks()));
				}
			}
		}
		// write midi file
		MIDI::Writer w(mConfig.GetFileName().c_str());
		w.Write(s);
		return true;
	}

	bool MIDIFileWriter::ConcreteConfigure(const ProcessingConfig& c)
	{
		CopyAsConcreteConfig(mConfig, c);
		return true;
	}

	void MIDIFileWriter::BuildEventList(const MIDITrack& trk)
	{
		mEventList.clear();
		int nEvents = trk.GetEventInfo().Size();
		for(int i=0; i < nEvents; i++)
		{
			mEventList.push_back(trk.GetEventInfo()[i]);
		}
		TData t;
		int channel=0;
		MIDIEvent ev;
		ev.GetMessage().Resize(3);
		ev.GetMessage().SetSize(3);
		int nNotes = trk.GetTrackMelody().GetNumberOfNotes();
		for(int i=0; i < nNotes; i++)
		{
			if(i < trk.GetChannels().Size()) channel = trk.GetChannels()[i];
			ev.GetMessage()[0] = (0x90 | channel);
			t = trk.GetTrackMelody().GetNoteArray()[i].GetTime().GetBegin();
			int key = trk.GetTrackMelody().GetNoteArray()[i].GetKey();
			int vel = trk.GetTrackMelody().GetNoteArray()[i].GetVelocity();
			ev.SetTime(t);
			ev.GetMessage()[1]=key;
			ev.GetMessage()[2]=vel;
			mEventList.push_back(ev);
			t = trk.GetTrackMelody().GetNoteArray()[i].GetTime().GetEnd();
			ev.SetTime(t);
			ev.GetMessage()[2]=0;
			mEventList.push_back(ev);
		}
		mEventList.sort();
	}
}

// END

