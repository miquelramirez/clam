#include "MelodyTranslator.hxx"

namespace CLAM
{
	void MelodyTranslatorConfig::DefaultInit(void)
	{
		AddAll();
		UpdateData();
	}

	MelodyTranslator::MelodyTranslator():
	mVelocityOut("VelocityOut", this ),
	mNoteOut( "NoteOut", this ),
	mStartMelody( true )
	{
		MelodyTranslatorConfig cfg;

		Configure( cfg );
	}

	MelodyTranslator::MelodyTranslator( const MelodyTranslatorConfig& cfg ):
	mVelocityOut("VelocityOut", this ),
	mNoteOut( "NoteOut", this ),
	mStartMelody( true )
	{
		Configure( cfg );
	}

	MelodyTranslator::~MelodyTranslator()
	{
	}

	bool MelodyTranslator::ConcreteConfigure( const ProcessingConfig& c )
	{
		CopyAsConcreteConfig(mConfig, c);

		mFileName = mConfig.GetFileName();

		return true;
	}
	
	void MelodyTranslator::LoadMelody()
	{
	
		XMLStorage x;
		x.Restore(mData,mFileName);
		mData.GetNoteArray().DoFirst();	
	}

	bool MelodyTranslator::ConcreteStart()
	{
		LoadMelody();

		return true;
	}

	bool MelodyTranslator::ConcreteStop()
	{
		return true;
	}

	bool MelodyTranslator::Do( const bool& play, TTime& currentTime )
	{
		int saltoNoteConstant=3;//Difference between real MIDI note and Salto's
		
		//current time in milliseconds
		//TTime currentTime = mEventSample/mConfig.GetSampleRate();//for the time being note time is in sec, should be in ms
		Note& currentNote = mData.GetNoteArray().Current();
		bool isLast = mData.GetNoteArray().IsLast();
		bool isFirst = mData.GetNoteArray().IsFirst();
		bool noteOff = false;

		if( mStartMelody )
		{
			mData.GetNoteArray().DoFirst();
			mStartMelody = false;
		}

		// Note On for the first note
		if(isFirst&&!play&&currentNote.GetTime().GetBegin()<currentTime/*&&currentNote.GetTime().GetEnd()>currentTime*/)
		{
			mVelocityOut.SendControl( currentNote.GetVelocity() );
			mNoteOut.SendControl( currentNote.GetNoteNumber()-saltoNoteConstant );
			//mHandleParams.NoteOn(currentNote.GetTime().GetBegin(),currentNote.GetNoteNumber()-saltoNoteConstant,currentNote.GetVelocity(),*mpParams);
		}

		if(play&&currentNote.GetTime().GetEnd()<currentTime)
		{
			// Note Off for the last note
			if(isLast)
			{
				mVelocityOut.SendControl( 0.0 );//currentNote.GetVelocity() );
				mNoteOut.SendControl( currentNote.GetNoteNumber()-saltoNoteConstant );
				mStartMelody = true;
			//	mHandleParams.NoteOff(currentNote.GetTime().GetEnd(),currentNote.GetNoteNumber()-saltoNoteConstant,currentNote.GetVelocity(),*mpParams);
				return false;
			}
			else
			{
				noteOff=true;/*now I know there is a note off but have to wait to 
				see if there is a simultaneous noteOn to simulate a transition*/
			}
		}

		if(!isLast)
		{
			mData.GetNoteArray().DoNext();
			Note& newCurrentNote = mData.GetNoteArray().Current();

			// Note On for a new note
			if(newCurrentNote.GetTime().GetBegin()<currentTime)
			{
				noteOff=false;//no noteOff, transition
				mVelocityOut.SendControl( newCurrentNote.GetVelocity() );
				mNoteOut.SendControl( newCurrentNote.GetNoteNumber()-saltoNoteConstant );
			//	mHandleParams.NoteOn(newCurrentNote.GetTime().GetBegin(),newCurrentNote.GetNoteNumber()-saltoNoteConstant,newCurrentNote.GetVelocity(),*mpParams);
			}
			else
			{
				//we found a note that still has not begun, we must wait
				mData.GetNoteArray().DoPrevious();
			}


			if(noteOff)
			{	
				//mVelocityOut.SendControl( currentNote.GetVelocity() );
				mVelocityOut.SendControl( 0.0 );
				mNoteOut.SendControl( currentNote.GetNoteNumber()-saltoNoteConstant );
				//mHandleParams.NoteOff(currentNote.GetTime().GetEnd(),currentNote.GetNoteNumber()-saltoNoteConstant,currentNote.GetVelocity(),*mpParams);
			}
		}

		return true;		
	}

	bool MelodyTranslator::Do( void )
	{
		mData.GetNoteArray().DoFirst();

		return true;
	}
};

