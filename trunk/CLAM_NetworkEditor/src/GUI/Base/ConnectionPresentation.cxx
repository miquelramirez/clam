
#include "ConnectionPresentation.hxx"
#include "ConnectionModel.hxx"
#include "Processing.hxx"

namespace NetworkGUI
{

ConnectionPresentation::ConnectionPresentation()
	: mInName( "unnamed inport" ),
	  mOutName( "unnamed outport" )
//	  mOutProc(0),
//	  mInProc(0)
{
	SetNames.Wrap( this, &ConnectionPresentation::OnNewNames );
}

ConnectionPresentation::~ConnectionPresentation()
{
}

void ConnectionPresentation::AttachTo( CLAMVM::ConnectionModel & m )
{
	m.AcquireNames.Connect(SetNames);
}

void ConnectionPresentation::OnNewNames( const std::string & portOut, const std::string & portIn )
{
//	mInProc = in;
//	mOutProc = out;
	mInName = portIn;
	mOutName = portOut;
}

} // namespace NetworkGUI
