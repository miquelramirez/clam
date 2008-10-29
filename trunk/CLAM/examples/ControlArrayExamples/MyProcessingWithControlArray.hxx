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

#ifndef _MyProcessingWithControlArray_hxx_
#define _MyProcessingWithControlArray_hxx_

#include <vector>
#include "TransformationDummy.hxx"
#include "InControlTmplArray.hxx"
#include "Processing.hxx"

namespace CLAM
{

class MyProcessingWithControlArray : public Processing
{
private:
	//typedef InControlArray ControlsWithoutCallback;

	typedef std::vector< TransformationDummy* > Transformations;
	typedef MyProcessingWithControlArray ThisProc;

public:
	enum {NVoices = 4};
	Transformations mSubPOsTransformation;
	InControlTmpl<ThisProc> mIn;
	InControlTmpl<ThisProc> mIn2;

	InControlTmplArray<ThisProc> mInArray;

	InControlTmplArray<ThisProc> mVoiceControl;

	bool Do(){return 0;}
	const ProcessingConfig& GetConfig() const {throw 0;}
	const char* GetClassName() const {return "MyProcessingWithControlArray";}
	bool ConcreteConfigure(const ProcessingConfig& c) {return true;}

	MyProcessingWithControlArray();
	~MyProcessingWithControlArray();
	int CallbackId(int id, TControlData val );
	int Callback(TControlData val ) { 
		std::cout << "Callback( "<<val<<std::endl; 
		return 0;
	};

	int VoiceControlCallbackId(int id, TControlData val);

	/**  this is only because this class is impersonating a Processing*/
	void PublishInControl(InControlBase* i) {};
};




} //namespace CLAM
#endif

