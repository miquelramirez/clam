#ifndef __MIDIMELODYPLAYER__
#define __MIDIMELODYPLAYER__

#include <string>
#include "MIDIMelody.hxx"
#include "Player.hxx"

namespace CLAM
{
    namespace VM
    {
	class MIDIMelodyPlayer : public Player
	{
	public:
	    MIDIMelodyPlayer();
	    ~MIDIMelodyPlayer();

	    void SetMIDIDevice(const std::string& device);
	    void SetMIDIProgram(const int& program);
	    
	    void SetData(const MIDIMelody& melody, const std::string& device, const int& program, const TData& dur);
	    MIDIMelody& GetMIDIMelody();

	    void UpdateNoteKey(const TIndex& index, const int& newKey);
	    void UpdateNoteDuration(const TIndex& index, const TData& beginTime);

	    void AddNote(const TIndex& index, const TData& beginTime, const int& key);
	    void RemoveNote(const TIndex& index);

	    void SetDuration(const TData& dur);

	protected:
	    void thread_code();

	private:
	    MIDIMelody mMIDIMelody;
	    std::string mMIDIDevice;
	    std::string mEnqueuedDevice;
	    bool mHasEnqueuedDevice;
	    int mMIDIProgram;
	    TData mDuration;

	    unsigned GetTime();
	    TIndex GetNoteIndex(bool first=true);
	    
	};
    }	 
 }

#endif



