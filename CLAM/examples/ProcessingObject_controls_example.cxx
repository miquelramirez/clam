/*
 * Copyright (c) 2001-2004 MUSIC TECHNOLOGY GROUP (MTG)
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


/**
 * @example  ProcessingObject_controls_example.cxx
 * This example shows several things about using controls
 *  - How to connect controls each other
 *  - How to explicitly propagate a value from an output control using SendControl
 *  - How to explicitly set a value to a input control using DoControl
 *  - How to associate processing callbacks to an input control
 */

///////////////////////////////////////////////////////////////////////////////////
// class MyProcObj
// Just a test class.
//

#include "Processing.hxx"
#include "InControl.hxx"
#include "OutControl.hxx"
#include <string>
#include <stdexcept>

#include <iostream> 

using namespace CLAM;

// Empty configuration for our processing
class MyProcConf : public ProcessingConfig {
public:
	DYNAMIC_TYPE_USING_INTERFACE (MyProcConf, 0,ProcessingConfig);
};


// A processing with some controls
class MyProcObj : public Processing
{
private:
	MyProcConf mConfig;
	InControlTmpl<MyProcObj> mInPitch;
	InControlTmpl<MyProcObj> mInAmplitude;
	
	OutControl mOutNoteOn;
	OutControl mOutNoteOff;
private:
	bool ConcreteConfigure(const ProcessingConfig &c) {return true;}
// Constructor/Destructor
public:
	MyProcObj(const MyProcConf &c) :
		// Asocciating callbacks to the control receiving
		mInPitch("Pitch", this, &MyProcObj::DoInPitchControl),
		mInAmplitude("Amplitude", this, &MyProcObj::DoInAmplitudeControl),

		mOutNoteOn("NoteOn", this),
		mOutNoteOff("NoteOff", this)
	{
		Configure(c);
	}
	virtual ~MyProcObj(){}
	const char * GetClassName() const {return "CLAMTest_MyProcObj";}
	
public:
	// Callbacks to receive controls
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

	// The do
	bool Do() 
	{
		std::cout << GetClassName() << ": doing my Do()... " << std::endl;
		return true;
	}
	const ProcessingConfig &GetConfig() const { return mConfig;};
	virtual bool Start(void) {return true;};
	virtual bool Stop(void) {return true;};
};


int main(void)
{
	try {
		MyProcConf conf1;
		conf1.UpdateData();
		MyProcObj proc1(conf1);
		MyProcObj proc2(conf1);
		MyProcObj proc3(conf1);

		// Connection by number.
		// Its also possible to connect them using control
		// names: "Pitch", "Amplitude", "NoteOn" and "NoteOff"

		CLAM::ConnectControls( proc1, "NoteOn",  proc2, "Amplitude");
		CLAM::ConnectControls( proc1, "NoteOff", proc2, "Pitch");
		CLAM::ConnectControls( proc1, "NoteOn",  proc3, "Amplitude");
		CLAM::ConnectControls( proc1, "NoteOn",  proc3, "Pitch");

		// SendControl propagates the value from an output control
		// to its connected in controls.

		// From proc1:Out[0] to proc2:In[0], proc3:In[0] and proc3:In[1]
		proc1.GetOutControls().Get("NoteOn").SendControl(44);
		// From proc1:Out[1] to proc2:In[1]
		proc1.GetOutControls().Get("NoteOff").SendControl(555);

		// 
		proc1.GetInControls().GetByNumber(0).DoControl(222.2f); 

		// Useless Do's, they are not necessary for the example
		proc1.Do();
		proc2.Do();


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

