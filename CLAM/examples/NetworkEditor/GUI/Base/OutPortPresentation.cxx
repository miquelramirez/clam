
#include "OutPortPresentation.hxx"
#include "PortModel.hxx"

namespace NetworkGUI
{

OutPortPresentation::OutPortPresentation()
	: mName( "unnamed out port" )
{
	SetName.Wrap( this, &OutPortPresentation::OnNewName );
}

OutPortPresentation::~OutPortPresentation()
{
}

void OutPortPresentation::AttachTo( CLAMVM::PortModel & m )
{
	m.AcquireName.Connect(SetName);
}

} // namespace NetworkGUI
