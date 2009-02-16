/*
 * Copyright (c) 2004 MUSIC TECHNOLOGY GROUP (MTG)
 *                         UNIVERSITAT POMPEU FABRA
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#ifndef _MyProcessingWithSimpleControls_hxx_
#define _MyProcessingWithSimpleControls_hxx_

#include "InControl.hxx"
#include "TransformationDummy.hxx"
#include "Processing.hxx"

#include <vector>
#include <iostream>

namespace CLAM
{

/**
 * This is just a dummy class for playing with with arrays of controls.
 * In this case we'll use a std container of simple InControls.
 */
class MyProcessingWithSimpleControls : public Processing
{
public:
	enum { eVoice0=0, eVoice1, eVoice2, eVoice3, eVoice4 };

private:
	enum { NControlsArray=5 };

	typedef std::vector< FloatInControl* > FloatInControls;
	typedef std::vector< TransformationDummy* > Transformations;

	/** Let's pretend we have sub processing objects doing transformations */
	Transformations subPOsTransformation;
	
public:
	FloatInControls aInControlArray;
	FloatInControls bInControlArray;
	FloatInControl cSingleControl;
	FloatInControl dSingleControl;
	
	/**  this is only because this class is impersonating a Processing*/
	void PublishInControl(InControlBase* i) {};

	MyProcessingWithSimpleControls();
	virtual ~MyProcessingWithSimpleControls();

	bool Do(){return 0;}
	const ProcessingConfig& GetConfig() const {throw 0;}
	const char* GetClassName() const {return "MyProcessingWithControlArray";}
	bool ConcreteConfigure(const ProcessingConfig& c) {return true;}
private:
	void InitChildrenPOs();
	void InitAInControlArray();
	void DeleteSubPOs();
	void DeleteControls(FloatInControls& c);

	/** in control callback definitions: */
	void SimpleCallback( TControlData data )
	{
		std::cout << "MyProcessingWithSimpleControls::SimpleCallback(" << data <<")\n";
		// in the real case we would send the data to the apropiate sub-processing in-control
	}

	/** template callback methods to associate to different array controls  */
	template<int N> 
	void ACallback( TControlData data)
	{
		std::cout << "MyProcessingWithSimpleControls::ACallback<" << N << "> (" << data <<")\n";
		// we send the data to the N-th in control of subPOsTransformation
		subPOsTransformation.at(N)->voiceInControl.DoControl( data );
	}

};


}; //namespace

#endif

