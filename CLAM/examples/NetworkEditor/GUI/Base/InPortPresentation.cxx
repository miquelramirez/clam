
#include "InPortPresentation.hxx"
#include "PortModel.hxx"

namespace NetworkGUI
{

InPortPresentation::InPortPresentation()
	: mName( "unnamed inport" )
//	  mDataType( "undefined data type" )
{
	SetName.Wrap( this, &InPortPresentation::OnNewName );
//	SetDataType.Wrap( this, &OutPortPresentation::OnNewDataType );
}

InPortPresentation::~InPortPresentation()
{
}

void InPortPresentation::AttachTo( CLAMVM::PortModel & m )
{
	m.AcquireName.Connect(SetName);
//	m.AcquireDataType.Connect(SetDataType);
}

} // namespace NetworkGUI
