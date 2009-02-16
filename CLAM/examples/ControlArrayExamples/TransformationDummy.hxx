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

#ifndef _TransformationDummy_hxx_
#define _TransformationDummy_hxx_

#include <iostream>

#include "InControl.hxx"
#include "Processing.hxx"

namespace CLAM 
{

class TransformationDummy : public Processing
{
public:
	FloatInControl voiceInControl;

	TransformationDummy() :
		voiceInControl(
			"voice_in_control", 
			this, 
			&TransformationDummy::StupidCallback	) 
	{}
	
	virtual ~TransformationDummy() {}

	bool Do(){return 0;}
	const ProcessingConfig& GetConfig() const {throw 0;}
	const char* GetClassName() const {return "TransformationDummy";}
	bool ConcreteConfigure(const ProcessingConfig& c) {return true;}

	// this is only because we are pretending that this class is a Processing
	void PublishInControl(InControlBase* i) {};

private:
	void StupidCallback(TControlData data) {
		std::cout << "\tTransformationDummy::StupidCallback(" << 
			data << ") with this==" << this << std::endl;
	}

};

}; //namespace


#endif

