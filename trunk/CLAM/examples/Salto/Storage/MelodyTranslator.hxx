#ifndef _MelodyTranslator_
#define _MelodyTranslator_

#include "Processing.hxx"
#include "XMLStorage.hxx"
#include "Melody.hxx"

namespace CLAM
{
	class MelodyTranslatorConfig: public ProcessingConfig
	{
	public:
		DYNAMIC_TYPE_USING_INTERFACE (MelodyTranslatorConfig, 2, ProcessingConfig);
		DYN_ATTRIBUTE (0, public, std::string, Name);
		DYN_ATTRIBUTE (1, public, std::string, FileName);
	protected:
		void DefaultInit(void);
	};

	class MelodyTranslator: public Processing
	{
	public:
		MelodyTranslator();

		MelodyTranslator( const MelodyTranslatorConfig& cfg );

		~MelodyTranslator();

		const char * GetClassName() const {return "MelodyTranslator";}

		const ProcessingConfig &GetConfig() const { return mConfig; }

		bool Do( const bool& play, TTime& currentTime );

		bool Do( void );

		bool IsRunning(void) 
		{ 
			if( /*Processing::*/GetExecState() == Running ) return  true;
			else						  return false;
		}

	protected:
		bool ConcreteConfigure( const ProcessingConfig& c );
		
		bool ConcreteStart();

		bool ConcreteStop();

		void LoadMelody();

	private:
		MelodyTranslatorConfig	mConfig;
		std::string				mFileName; 
		Melody					mData;
		OutControl				mVelocityOut;
		OutControl				mNoteOut;
		bool					mStartMelody;
	};
};

#endif
