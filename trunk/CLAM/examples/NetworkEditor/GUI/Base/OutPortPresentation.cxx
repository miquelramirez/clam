
#include "OutPortPresentation.hxx"
#include "PortModel.hxx"

namespace NetworkGUI
{

OutPortPresentation::OutPortPresentation()
	: mName( "unnamed inport" )
//	  mDataType( "undefined data type" )
{
	SetName.Wrap( this, &OutPortPresentation::OnNewName );
//	SetDataType.Wrap( this, &OutPortPresentation::OnNewDataType );
}

OutPortPresentation::~OutPortPresentation()
{
}

void OutPortPresentation::AttachTo( CLAMVM::PortModel & m )
{
	m.AcquireName.Connect(SetName);
//	m.AcquireDataType.Connect(SetDataType);
}

} // namespace NetworkGUI
