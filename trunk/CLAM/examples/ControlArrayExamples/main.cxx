#include "MyProcessingWithSimpleControls.hxx"
#include "MyProcessingWithControlArray.hxx"

using namespace CLAM;

void main()
{
	TControlData c1(1), c2(2), c3(3);

	MyProcessingWithSimpleControls model;
	
	model.aInControlArray[ MyProcessingWithSimpleControls::eVoice0 ]->DoControl (c1);
	model.aInControlArray[ MyProcessingWithSimpleControls::eVoice3 ]->DoControl (c2);
	model.aInControlArray[ MyProcessingWithSimpleControls::eVoice4 ]->DoControl (c3);

	model.cSingleControl.DoControl( c2 );
	model.dSingleControl.DoControl( c3 );

// THe new version using InControlTmplArray:

	MyProcessingWithControlArray withControlArray;

	withControlArray.mIn.DoControl( TControlData(3) );
	std::cout << " expected: id 12, val 3\n";

	withControlArray.mIn2.DoControl( TControlData(3) );
	std::cout << " expected: val 3\n";

	for (int i=0; i<10; i++)
		withControlArray.mInArray[i].DoControl( TControlData(3.14) );
	std::cout << " expected: from id 0 to 9, val 3.14\n";
	
	int NVoices = MyProcessingWithControlArray::NVoices;
    for (int i=0; i<NVoices; i++)
		withControlArray.mVoiceControl[i].DoControl( TControlData(2) );
	std::cout << " expected: from id 0 to " << NVoices << " val 2\n";




	
}
