#ifndef _MyProcessingWithSimpleControls_hxx_
#define _MyProcessingWithSimpleControls_hxx_

#include "InControl.hxx"
#include "TransformationDummy.hxx"

#include <vector>
#include <iostream>

namespace CLAM
{

/**
 * This is just a dummy class for playing with with arrays of controls.
 * In this case we'll use a std container of simple InControls.
 */
class MyProcessingWithSimpleControls
{
public:
	enum { eVoice0=0, eVoice1, eVoice2, eVoice3, eVoice4 };

private:
	enum { NControlsArray=5 };

	typedef InControlTmpl<MyProcessingWithSimpleControls> VFxInControl;
	typedef std::vector< VFxInControl* > VFxInControls;
	typedef std::vector< TransformationDummy* > Transformations;

	/** Let's pretend we have sub processing objects doing transformations */
	Transformations subPOsTransformation;
	
public:
	VFxInControls aInControlArray;
	VFxInControls bInControlArray;
	VFxInControl cSingleControl;
	VFxInControl dSingleControl;
	
	/**  this is only because this class is impersonating a Processing*/
	void PublishInControl(InControl* i) {};

	MyProcessingWithSimpleControls();
	virtual ~MyProcessingWithSimpleControls();

private:
	void InitChildrenPOs();
	void InitAInControlArray();
	void DeleteSubPOs();
	void DeleteControls(VFxInControls& c);

	/** in control callback definitions: */
	int SimpleCallback( TControlData data )
	{
		std::cout << "MyProcessingWithSimpleControls::SimpleCallback(" << data <<")\n";
		// in the real case we would send the data to the apropiate sub-processing in-control
		return 0;
	}

	/** template callback methods to associate to different array controls  */
	template<int N> 
	int ACallback( TControlData data)
	{
		std::cout << "MyProcessingWithSimpleControls::ACallback<" << N << "> (" << data <<")\n";
		// we send the data to the N-th in control of subPOsTransformation
		subPOsTransformation.at(N)->voiceInControl.DoControl( data );
		return 0;
	}

};


}; //namespace

#endif