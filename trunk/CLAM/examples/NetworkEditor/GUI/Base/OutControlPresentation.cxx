
#include "OutControlPresentation.hxx"
#include "ControlModel.hxx"

namespace NetworkGUI
{

OutControlPresentation::OutControlPresentation()
	: mName( "unnamed out control" )
{
	SetName.Wrap( this, &OutControlPresentation::OnNewName );
}

OutControlPresentation::~OutControlPresentation()
{
}

void OutControlPresentation::AttachTo( CLAMVM::ControlModel & m )
{
	m.AcquireName.Connect(SetName);
}

} // namespace NetworkGUI
