
#include "InControlPresentation.hxx"
#include "ControlModel.hxx"

namespace NetworkGUI
{

InControlPresentation::InControlPresentation()
	: mName( "unnamed in control" )
{
	SetName.Wrap( this, &InControlPresentation::OnNewName );
}

InControlPresentation::~InControlPresentation()
{
}

void InControlPresentation::AttachTo( CLAMVM::ControlModel & m )
{
	m.AcquireName.Connect(SetName);
}

} // namespace NetworkGUI
