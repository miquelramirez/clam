#ifndef _CSaltoHandleParamChanges_
#define _CSaltoHandleParamChanges_

#include "Parameters.hxx"
#include "InControl.hxx"


namespace CLAM{

	class CSaltoHandleParamChangesConfig: public ProcessingConfig
	{
	public:
		DYNAMIC_TYPE_USING_INTERFACE (CSaltoHandleParamChangesConfig, 3, ProcessingConfig);
		DYN_ATTRIBUTE (0, public, std::string, Name);
		DYN_ATTRIBUTE (1, public, TIndex, Transpose);
		DYN_ATTRIBUTE (2, public, double, PitchModRange);
	protected:
		void DefaultInit(void);
	};

	class CSaltoHandleParamChanges:public Processing
	{
	public:	
	//	CSaltoHandleParamChanges(CSaltoParameter* pParams);
		CSaltoHandleParamChanges();

		CSaltoHandleParamChanges( const CSaltoHandleParamChangesConfig& cfg );

		virtual ~CSaltoHandleParamChanges();

		const ProcessingConfig &GetConfig() const { return mConfig; }

		bool ConcreteConfigure( const ProcessingConfig& cfg ) throw(std::bad_cast);


	//	void NoteOn(TTime timetag, TData pitch,TData velocity,Parameters& params);
	//	void NoteOff(TTime timetag, TData pitch,TData velocity,Parameters& params);
		bool Do(void) { return true; }

		bool Do( Parameters& params );

	private:
		CSaltoHandleParamChangesConfig				mConfig;
		InControlTmpl< CSaltoHandleParamChanges >	mVelocityIn;
		InControlTmpl< CSaltoHandleParamChanges >	mNoteIn;
		InControlTmpl< CSaltoHandleParamChanges >	mPitchBendIn;
		TControlData								mVelocity;
		TControlData								mNote;
		TControlData								mNoteOff;
		TControlData								mPitchBend;
		TIndex										mTranspose;
		double										mPitchModRange;
		int											mLastPitch;
		enum Status {
			eNoteOff = 0,
			eNoteOn  = 1,
		}   mStatus;

	protected:

		void NoteOn(Parameters& params);
		void NoteOff(Parameters& params);

		void ScaleNote( TControlData& note )
		{
		    note += mTranspose;

			if (note<=50) 
				note = 0;       //we work with two octaves 
			else if (note >=75)
				note =24;
			else              // 0 is lowest pitch
				note -= 50;
		}

		int UpdateVelocity( TControlData velocity )
		{
			mVelocity = velocity;

			if( mVelocity == 0 )
				mStatus = eNoteOff;
			else
				mStatus = eNoteOn;

			return 0;
		}

		int UpdateNote ( TControlData note )
		{
			ScaleNote( note );

			if( mStatus == eNoteOn )
			{
				mNote = note;
				mLastPitch = 0.0;
			}
			else if( note == mNote )
				mNoteOff = note;
			
			return 0;
		}

		int UpdatePitchBend( TControlData value )
		{
			mPitchBend = ((double) value - 70 ) / 70.0 * mPitchModRange + 1.0;
			 //params.SetPitchModFactor(
			//	((double)bytes[2]-70)/70.0*params.GetPitchModRange()+1.0);
			return 0;
		}
	};

};
#endif //_CSaltoHandleParamChanges_

