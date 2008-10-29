#ifndef __ProgressControl_hxx__
#define __ProgressControl_hxx__

#include "Processing.hxx"
#include "ProcessingConfig.hxx"
#include "OutControl.hxx"
#include "InControl.hxx"

namespace CLAM
{

	class ProgressControl : public Processing
	{
	public:
	
		ProgressControl( const Config& cfg = Config() );
	
		virtual const char* GetClassName() const;
		virtual bool Do();
		virtual ~ProgressControl();
		
	protected: // attributes
		
		Config       _config;
		FloatOutControl   _jumpOutput;
		FloatInControl    _progressInput;
};

} //namespace CLAM

#endif //__ProgressControl_hxx__
