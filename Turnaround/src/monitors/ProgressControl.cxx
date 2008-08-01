#include "ProgressControl.hxx"
#include <CLAM/ProcessingFactory.hxx>

namespace CLAM
{

namespace Hidden
{
	static const char * metadata[] = {
		"key", "ProgressControl",
		"category", "Controls",
		"description", "ProgressControl",
		0
	};
	static FactoryRegistrator<ProcessingFactory, ProgressControl> reg = metadata;
}

	ProgressControl::ProgressControl( const Config& cfg )
	: _jumpOutput( "Progress Jump", this ),
	  _progressInput( "Progress Update", this )
	{
		Configure( cfg );
		_progressInput.SetBounds( 0.0, 1.0 );
	}

	ProgressControl::~ProgressControl()
	{
	}

	// TODO: move it to the header
	const char* ProgressControl::GetClassName() const
	{
		return "ProgressControl";
	}

								
	bool ProgressControl::Do()
	{
		return true;
	}
		
}
