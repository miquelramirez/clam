#include "PlotBase.hxx"
#include "SystemPlots.hxx"

namespace CLAMVM
{

	void PlotBase::SetName( std::string name )
	{
		if ( SystemPlots::IsNameInUse( name ) )
			mName = SystemPlots::GenerateNewUniqueName( );
		mName = name;
	}

	PlotBase::~PlotBase()
	{
		SystemPlots::UnregisterWidget( GetName() );
	}


}
