#include "MyProcessingWithSimpleControls.hxx"

namespace CLAM {

void MyProcessingWithSimpleControls::InitChildrenPOs()
{
	for(int i=0; i!=NControlsArray; i++) {
		subPOsTransformation.push_back(
			new TransformationDummy () );
	}
}

void MyProcessingWithSimpleControls::InitAInControlArray()
{
	// init in controls array:
	aInControlArray.push_back( new VFxInControl (
			"A_In_Control_0",
			this,
			&MyProcessingWithSimpleControls::ACallback< eVoice0 > ) );

	aInControlArray.push_back( new VFxInControl (
			"A_In_Control_1",
			this,
			&MyProcessingWithSimpleControls::ACallback< eVoice1 > ) );

	aInControlArray.push_back( new VFxInControl (
			"A_In_Control_2",
			this,
			&MyProcessingWithSimpleControls::ACallback< eVoice2 > ) );

	aInControlArray.push_back( new VFxInControl (
			"A_In_Control_3",
			this,
			&MyProcessingWithSimpleControls::ACallback< eVoice3 > ) );

	aInControlArray.push_back( new VFxInControl (
			"A_In_Control_4",
			this,
			&MyProcessingWithSimpleControls::ACallback< eVoice4 > ) );

}





MyProcessingWithSimpleControls::MyProcessingWithSimpleControls() :
	// init single in controls
	cSingleControl ("CSingleControl", this, SimpleCallback),
	dSingleControl ("CSingleControl", this,	SimpleCallback)
{
	
	// init sub POs transformation
	InitChildrenPOs();
	// init in controls array:
	InitAInControlArray();
}

void MyProcessingWithSimpleControls::DeleteSubPOs()
{
	Transformations::iterator it;
	for(it=subPOsTransformation.begin(); it!=subPOsTransformation.end(); it++)
		delete (*it); // (it) points to a PO pointer
}

void MyProcessingWithSimpleControls::DeleteControls( VFxInControls& c )
{
	VFxInControls::iterator it;
	for(it=c.begin(); it!=c.end(); it++)
		delete (*it); // (it) points to a control pointer
}

MyProcessingWithSimpleControls::~MyProcessingWithSimpleControls()
{
	DeleteControls(aInControlArray);
	DeleteControls(bInControlArray);
	DeleteSubPOs();

}

}; //namespace