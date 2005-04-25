#ifndef __MELODYPLAYER__
#define __MELODYPLAYER__

 #include "Melody.hxx"
 #include "Player.hxx"

 namespace CLAM
 {
     namespace VM
     {
	 class MelodyPlayer : public Player
	 {
	 public:
	     MelodyPlayer();
	     ~MelodyPlayer();

	     void SetData(const Melody& melody);
	     Melody& GetMelody();

	     void UpdateNotePitch(const TIndex& index, const TData& newPitch);
	     void UpdateNoteDuration(const TIndex& index, const TData& beginTime);

	     void AddNote(const TIndex& index, const TData& beginTime, const TData& pitch);
	     void RemoveNote(const TIndex& index);

	 protected:
	     void thread_code();

	 private:
	     Melody mMelody;
	     TData mSampleRate;

	     TIndex GetNoteIndex(bool first=true);
	    
	 };
     }	 
 }

#endif



