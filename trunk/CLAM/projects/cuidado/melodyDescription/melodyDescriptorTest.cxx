
#ifdef WIN32
#	pragma warning(disable: 4786 4503)  // for avoiding too-long-debug-lines warning.
#endif

#include <iostream>

#include <fstream>

#include "Array.hxx"

#ifdef _MTG_USEXML_
#	include "XMLStorage.hxx"
#	include "XMLStaticAdapter.hxx"
#	include "XMLComponentAdapter.hxx"
#	include "XMLable.hxx"
#endif//_MTG_USEXML_

#include "Melody.hxx"

using namespace MTG;

int main()
{
  try
	{
		int i;
		
	  Array<Note> array(5);

		for(i=0; i<5; i++) {

			MediaTimeConfig mtcnf;
			mtcnf.Settype(EMediaTimeType::eBeginDuration);
			MediaTime time(mtcnf);
			time.Setbegin(0+i);
			time.Setduration(i+0.99);

			PitchNoteConfig npcnf;
			PitchNote pitch(npcnf);
			pitch.Setpitch(EPitch::eE);
			pitch.Setoctave(7);

			NoteConfig nCnf;
			nCnf.SethasPitch(1);
			nCnf.SettimeType(EMediaTimeType::eBeginDuration);
			Note myNote(nCnf);
	
			myNote.SetfundFreq(2*i);
			myNote.Settime(time);
			myNote.SetpitchNote(pitch);

			array.AddElem(myNote);
		}

		MelodyConfig mCnf;
	
		Melody myMelody(mCnf);
		myMelody.SetnoteArray(array);
		
#ifdef _MTG_USEXML_

	  /*****Storing result to XML document*****/
		XMLStorage storage ("XML-Document");
		XMLComponentAdapter myAdapter(myMelody,"Melody",true);
					
		storage.Store(&myAdapter);
		std::fstream fileout("melody.xml", ios::out);
		
		storage.dumpOn(fileout);
	}
	
#endif//_MTG_USEXML_
	
	//}
  
  catch(Err error)
    {
      error.Print();
      std::cerr << "Abnormal Program Termination" << std::endl;
    }
  catch (std::exception e)
    {
      std::cout << e.what() << std::endl;
    }
  
  std::clog << "Finished successfully!";
  return 0;
}
