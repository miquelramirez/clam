/*
 * Copyright (c) 2001-2002 MUSIC TECHNOLOGY GROUP (MTG)
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

#ifndef _TestControlsDummyClasses_hxx
#define _TestControlsDummyClasses_hxx

#include "OutControl.hxx"
#include "Processing.hxx"
#include "DynamicType.hxx"
#include <vector>
#include <iostream>
#include <strstream>


using namespace CLAM;

namespace CLAMTest
{

class ProcessingControlConfig : public ProcessingConfig
{
public:
	DYNAMIC_TYPE_USING_INTERFACE(ProcessingControlConfig, 3,  ProcessingConfig);
	DYN_ATTRIBUTE(0, public, std::string, Name);
	DYN_ATTRIBUTE(1, public, int, NInCtrls);
	DYN_ATTRIBUTE(2, public, int, NOutCtrls);

	void DefaultInit()
	{
		AddAll();
		UpdateData();
		SetNInCtrls(5);
		SetNOutCtrls(10);
	}
};

class StaticControls : public Processing
{	
public:
	InControlTmpl<StaticControls> in1, in3;
	InControl in2;
	
	OutControl out1, out2;
	
	// for checking publication order:
	InControl o0, o1, o2, o3, o4, o5;

	StaticControls() : 
		in1("in1", this, &StaticControls::CallBack1),
		in2("in2", this),
		in3("in3", this), // testing a inControlTmpl without CallBack.
		out1("out1", this),
		out2("out2", this), 
		o0("0", this), o1("1", this), o2("2", this), o3("3", this), o4("4", this), o5("5", this)
	{}

private:
	int CallBack1(TControlData d) {
		std::cout << "CallBack1 called. data: "<< d << std::endl;
		return 0;
	}
	
	ProcessingControlConfig mConfig;
	
	bool ConcreteConfigure(const ProcessingConfig& cfg) { return true;}
	bool Do(void) { 
		CLAM_ASSERT(true, "this Do is not aimed to be used."); 
		return 0;
	}
	const ProcessingConfig& GetConfig(void) const {
		return mConfig;
	}
};

class DynamicControls : public Processing
{
public:
	std::vector< InControlTmpl<DynamicControls>* > inCtrlsTmpl;
	std::vector< InControl* > inCtrls;
	std::vector< OutControl* > outCtrls;

	DynamicControls() {
	}
	~DynamicControls() {
		// remove vectors:
		int i;
		for (i=0; i<inCtrlsTmpl.size(); i++) {
			delete inCtrlsTmpl.at(i);
		}
		for (i=0; i<inCtrls.size(); i++) {
			delete inCtrls.at(i);
		}
		for (i=0; i<outCtrls.size(); i++) {
			delete outCtrls.at(i);
		}
		
	}

private:

	int CallBack2(TControlData d) {
		std::cout << "CallBack2 called. (dynamic controls example). data: "<< d << std::endl;

		return 0;
	}

	ProcessingControlConfig mConfig;
	bool ConcreteConfigure(const ProcessingConfig& cfg) { 
		mConfig = dynamic_cast<const ProcessingControlConfig&>(cfg);
		
		int nin = mConfig.GetNInCtrls();
		int nout =mConfig.GetNOutCtrls();
		int i;

		
		
		inCtrlsTmpl.reserve(nin);
		for (i=0; i<nin; i++) {
			std::strstream name;
			name << "dynamic_in_control_tmpl_" << i << std::ends;
			inCtrlsTmpl.push_back(new InControlTmpl<DynamicControls>(name.str(), this, &DynamicControls::CallBack2) );
		}

		inCtrls.reserve(nin);
		for ( i=0; i<nin; i++) {
			std::strstream name;
			name << "dynamic_in_control_" << i << std::ends;
			inCtrls.push_back(new InControl(name.str(), this) );
		}
		outCtrls.reserve(nout);
		for ( i=0; i<nout; i++) {
			std::strstream name;
			name << "dynamic_out_control_" << i << std::ends;
			outCtrls.push_back(new OutControl(name.str(), this) );
		}

		// now let's do a lot of connections:
		int j;
		for (i=0; i<inCtrlsTmpl.size(); i++) {
			for (j=0; j<outCtrls.size(); j++) outCtrls.at(j)->AddLink(inCtrlsTmpl.at(i));
		}
		for (i=0; i<inCtrls.size(); i++) {
			for (j=0; j<outCtrls.size(); j++) outCtrls.at(j)->AddLink(inCtrls.at(i));
		}

		return true;
	}

	bool Do(void) { 
		CLAM_ASSERT(true, "this Do is not aimed to be used."); 
		return 0;
	}
	const ProcessingConfig& GetConfig(void) const {
		return mConfig;
	}

};


}; 

#endif
