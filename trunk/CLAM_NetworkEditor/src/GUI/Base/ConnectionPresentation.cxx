
#include "ConnectionPresentation.hxx"
#include "ConnectionModel.hxx"

namespace NetworkGUI
{

ConnectionPresentation::ConnectionPresentation()
	: mInName( "unnamed inport" ),
	  mOutName( "unnamed outport" )
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

void ConnectionPresentation::OnNewNames(const std::string & out, const std::string & in)
{
	mInName = in;
	mOutName = out;
}

} // namespace NetworkGUI
