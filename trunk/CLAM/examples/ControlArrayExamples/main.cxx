#include "MyProcessingWithSimpleControls.hxx"
#include "MyProcessingWithControlArray.hxx"

using namespace CLAM;

void main()
{
	TControlData c1(1), c2(2), c3(3);

	MyProcessingWithSimpleControls model;
	
	model.aInControlArray.at( MyProcessingWithSimpleControls::eVoice0 )->DoControl (c1);
	model.aInControlArray.at( MyProcessingWithSimpleControls::eVoice3 )->DoControl (c2);
	model.aInControlArray.at( MyProcessingWithSimpleControls::eVoice4 )->DoControl (c3);

	model.cSingleControl.DoControl( c2 );
	model.dSingleControl.DoControl( c3 );

// THe new version using InControlTmplArray:

	MyProcessingWithControlArray withControlArray;

	withControlArray._in.DoControl( TControlData(3) );
	std::cout << " expected: id 12, val 3\n";

	withControlArray._in2.DoControl( TControlData(3) );
	std::cout << " expected: val 3\n";

	for (int i=0; i<10; i++)
		withControlArray._inArray[i].DoControl( TControlData(3.14) );
	std::cout << " expected: from id 0 to 9, val 3.14\n";
	
	int NVoices = MyProcessingWithControlArray::NVoices;
    for (int i=0; i<NVoices; i++)
		withControlArray._voiceControl[i].DoControl( TControlData(2) );
	std::cout << " expected: from id 0 to " << NVoices << " val 2\n";




	
}