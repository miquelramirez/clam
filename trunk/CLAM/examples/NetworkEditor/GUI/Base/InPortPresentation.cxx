
#include "InPortPresentation.hxx"
#include "PortModel.hxx"

namespace NetworkGUI
{

InPortPresentation::InPortPresentation()
	: mName( "unnamed in port" )
{
	SetName.Wrap( this, &InPortPresentation::OnNewName );
}

InPortPresentation::~InPortPresentation()
{
}

void InPortPresentation::AttachTo( CLAMVM::PortModel & m )
{
	m.AcquireName.Connect(SetName);
}

} // namespace NetworkGUI
