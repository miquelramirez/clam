#ifndef __MIDIMELODYPLAYER__
#define __MIDIMELODYPLAYER__

#include <map>
#include <vector>
#include <string>
#include <CLAM/MIDIMelody.hxx>
#include <CLAM/Player.hxx>

namespace CLAM
{
    namespace VM
    {
		class MIDIMelodyPlayer : public Player
		{
			typedef std::map<std::string,unsigned> IndexTable;
			typedef std::vector<MIDIMelody>        Melodies;

		public:
			MIDIMelodyPlayer();
			~MIDIMelodyPlayer();

			void SetMIDIDevice(const std::string& device);
			void SetMIDIProgram(const int& program);
	    
			void AddData(const std::string& key, const MIDIMelody& melody, const TData& dur);
			MIDIMelody& GetMIDIMelody(const std::string& key);

			void UpdateNoteKey(const TIndex& index, const int& newKey);
			void UpdateNoteDuration(const TIndex& index, const TData& beginTime);

			void AddNote(const TIndex& index, const TData& beginTime, const int& key);
			void RemoveNote(const TIndex& index);

			void SetDuration(const TData& dur);
			void SetCurrent(const std::string& current);

		protected:
			void thread_code();

		private:
			IndexTable  mIndexTable;
			Melodies    mMelodies;
			std::string mCurrent;
			std::string mMIDIDevice;
			std::string mEnqueuedDevice;
			bool        mHasEnqueuedDevice;
			int         mMIDIProgram;
			TData       mDuration;

			unsigned GetTime();
			TIndex GetNoteIndex(bool first=true);

			void InitTables();
			void AddMelody(const std::string& key, const MIDIMelody& melody);
			bool HaveKey(const std::string& key);
			unsigned GetMelodyIndex(const std::string& key);
	    
		};
    }	 
 }

#endif



