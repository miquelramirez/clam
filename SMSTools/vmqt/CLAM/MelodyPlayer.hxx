#ifndef __MELODYPLAYER__
#define __MELODYPLAYER__

#include <map>
#include <vector>
#include <string>
#include <CLAM/Melody.hxx>
#include <CLAM/Player.hxx>

namespace CLAM
{
	class Audio;

	namespace VM
	{
		class MelodyPlayer : public Player
		{
			typedef std::map<std::string,unsigned> IndexTable;
			typedef std::vector<Melody>            Melodies;

		public:
			MelodyPlayer();
			~MelodyPlayer();

			void AddData(const std::string& key, const Melody& melody, const TData& dur);
			Melody& GetMelody(const std::string& key);

			void UpdateNotePitch(const TIndex& index, const TData& newPitch);
			void UpdateNoteDuration(const TIndex& index, const TData& beginTime);

			void AddNote(const TIndex& index, const TData& beginTime, const TData& pitch);
			void RemoveNote(const TIndex& index);

			void SetDuration(const TData& dur);

			void SetAudioPtr(const Audio* audio);

			void PlayMelody(bool play);
			void PlayAudio(bool play);

			void SetCurrent(const std::string& current);

		protected:
			void thread_code();

		private:
			IndexTable   mIndexTable;
			Melodies     mMelodies;
			std::string  mCurrent;
			const Audio* mAudioPtr;
			TData        mSampleRate;
			TData        mDuration;
			bool         mPlayMelody;
			bool         mPlayAudio;

			TIndex GetNoteIndex(bool first=true);

			void InitTables();
			void AddMelody(const std::string& key, const Melody& melody);
			bool HaveKey(const std::string& key);
			unsigned GetMelodyIndex(const std::string& key);
	    
		};
	}	 
 }

#endif



