#ifndef __ZEROPADDER__
#define __ZEROPADDER__

#include "Processing.hxx"
#include "ProcessingConfig.hxx"
#include "Audio.hxx"
#include "InPortTmpl.hxx"
#include "OutPortTmpl.hxx"

namespace CLAM
{
	class ZeroPadderConfig
		: public ProcessingConfig
	{
		DYNAMIC_TYPE_USING_INTERFACE( ZeroPadderConfig, 0, ProcessingConfig );
	};

	class ZeroPadder : public Processing
	{
	protected: // attributes

		ZeroPadderConfig   mConfig;

		InPortTmpl< Audio >    mInput;
		OutPortTmpl< Audio >   mOutput;

		bool ConcreteStart();
		bool ConcreteStop();

	public:
		ZeroPadder();
		virtual ~ZeroPadder();

		bool Do();	       

		const char* GetClassName() const
		{
			return "ZeroPadder";
		}

		const ProcessingConfig& GetConfig() const 
		{
			return mConfig;
		}

	protected:
		bool ConcreteConfigure( const ProcessingConfig& c );
		
	};

}

#endif // ZeroPadder.hxx
