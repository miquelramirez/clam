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

///////////////////////////////////////////////////////////////////////////////////
// class MyProcObj
// Just a test class.
//

#ifndef _MyProcObj_
#define _MyProcObj_


#include "Processing.hxx"
#include "InControl.hxx"
#include "OutControl.hxx"
#include <string>
#include <stdexcept>

#include <iostream> 

using namespace CLAM;

class MyProcConf : public ProcessingConfig {
public:
	DYNAMIC_TYPE_USING_INTERFACE (MyProcConf, 0,ProcessingConfig);
};

class MyProcObj : public Processing
{
// Attributes
	MyProcConf mConfig;
private:
	InControlTmpl<MyProcObj> mInPitch;
	InControlTmpl<MyProcObj> mInAmplitude;
	
	OutControl mOutNoteOn;
	OutControl mOutNoteOff;
	bool ConcreteConfigure(const ProcessingConfig &c) {return true;}
// Constructor/Destructor
public:
	MyProcObj(const MyProcConf &c) :
		mInPitch("Pitch", this, &MyProcObj::DoInPitchControl),
		mInAmplitude("Amplitude", this, &MyProcObj::DoInAmplitudeControl),

		mOutNoteOn("NoteOn", this),
		mOutNoteOff("NoteOff", this)
	{
		Configure(c);
	}
	virtual ~MyProcObj(){}
	const char * GetClassName() const {return "CLAMTest_MyProcObj";}
	
	TControlData mState;
public:
	

	int DoInPitchControl(TControlData val) 
	{ 
		std::cout << GetClassName() << ": DoInPitchControl activated. Value="<< val << std::endl;  
		return 1; 
	}
	int DoInAmplitudeControl(TControlData val) 
	{ 
		std::cout << GetClassName() << ": DoInAmplitudeControl activated. Value="<< val << std::endl;  
		return 2; 
	}
	
	bool Do() 
	{
		std::cout << GetClassName() << ": doing my Do()... " << std::endl;
		return true;
	}
	const ProcessingConfig &GetConfig() const { return mConfig;};
	virtual bool Start(void) {return true;};
	virtual bool Stop(void) {return true;};
};

#endif // _MyProcObj_


int main(void)
{
	try {
	MyProcConf conf1;
	conf1.UpdateData();
	MyProcObj proc1(conf1);
	MyProcObj proc2(conf1);
	MyProcObj proc3(conf1);

	proc1.GetOutControls().GetByNumber(0).AddLink(&proc2.GetInControls().GetByNumber(0));
	proc1.GetOutControls().GetByNumber(1).AddLink(&proc2.GetInControls().GetByNumber(1));
	proc1.GetOutControls().GetByNumber(0).AddLink(&proc3.GetInControls().GetByNumber(0));
	proc1.GetOutControls().GetByNumber(0).AddLink(&proc3.GetInControls().GetByNumber(1));

	proc1.Do();
	proc2.Do();
	proc1.GetOutControls().GetByNumber(0).SendControl(44);
	proc1.GetOutControls().GetByNumber(1).SendControl(555);

	proc1.GetInControls().GetByNumber(0).DoControl(222.2f); 


	}catch(std::out_of_range e)
	{
		std::cerr << e.what() << std::endl;
	}
	catch(Err e)
	{
		e.Print();
	}
	
	return 0;

}
