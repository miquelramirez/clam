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

// TokenDelay_example.cxx
// example for the TokenDelay processing object

#include "TokenDelay.hxx"
#include "OutControl.hxx"
#include "Processing.hxx"
#include "ProcessingData.hxx"

#include <string>
#include <stdio.h>
#include <iostream>

using namespace CLAM;
using namespace std;

namespace CLAMTEST {
//////////////////////////////////////////////////////////////////////////////////////////
struct ControlerConfig : public ProcessingConfig
{
	REG_NUM_ATTR_USING_INTERFACE (ControlerConfig, 1, ProcessingConfig);
	REGISTER (0, Name, std::string);
	/** 
	* Dynamic type initialization: all attributes are instantiated.
	*/
	void Init(void)
	{
		AddName();
		UpdateData();
		SetName("Controler");
	}
};
//////////////////////////////////////////////////////////////////////////////////////////
class Controler : public Processing
{
public:
	OutControl out;
	Controler () : out("out control", this)
	{}

	bool ConcreteConfigure(const ProcessingConfig& c)
	{
		mConfig = dynamic_cast<const ControlerConfig&>(c);
		return true;
	}

	bool Do() {return false;}

	const ProcessingConfig&  GetConfig () const { return mConfig; }
private:
	ControlerConfig mConfig;
};
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
class CharDataConfig : public ProcessingDataConfig
{
public:
	REG_NUM_ATTR_USING_INTERFACE (CharDataConfig, 0, ProcessingDataConfig);

};
//////////////////////////////////////////////////////////////////////////////////////////
class CharDataObject : public ProcessingData
{
public:
	REG_NUM_ATTR_USING_INTERFACE(CharDataObject, 2, ProcessingData);
	REGISTER (0, Name, string);
	REGISTER (1, Chr, char);

	void Init()
	{
		AddChr();
		UpdateData();
	}
	const ProcessingDataConfig &GetProcessingDataConfig() const { return mSettings;}
	virtual ~CharDataObject()
	{
		cout << " deleting: " << GetChr();
	}

private:
	CharDataConfig mSettings;
};
//////////////////////////////////////////////////////////////////////////////////////////
} //namespace CLAMTEST


using namespace CLAMTEST;

//forward references
void PrintHelp(void);
void RunStep(string::iterator, TokenDelay<CharDataObject>&);
void SendControl(string::iterator, Controler&);
void ChangeDelay(istream &, TokenDelay<CharDataObject> &);



int main()
{
	TokenDelayConfig cfg;
	cfg.SetMaxDelay(20);

	TokenDelay<CharDataObject> delay(cfg);
	Controler control;	

try {
	
	// link control with delay objects
	control.LinkOutWithInControl(0, &delay, 0);

	string cmd;
	
	PrintHelp();
	do {
		cout << "Input Mode> ";

		getline(cin, cmd, '\n');
		cout << "Running Mode> ";
		cin.ignore();	
		
		string::iterator i = cmd.begin();

		do {
			char step=cin.get();
	
			if (step=='\n') RunStep(i++, delay);
			else if (step=='q') cmd="";
			else if (step>='0' && step <='9') ChangeDelay(cin, delay);
			else {
				cout << "press Enter to do step or 'q' to quit\n";
				cin.ignore();
			}
			cout << "Running Mode> ";
		} while (cmd != "");
		
	} while (cmd != "");
	
} catch (Err e)
{
	e.Print();
	delay.Debug();

} catch (exception ex)
{
	cout << ex.what() << endl;
	delay.Debug();
}



	return 0;
}

void PrintHelp(void)
{
	cout << "TokenDelay Example Usage: \n-------------------------\n"
		<< "In this example the ProcessingDataObjects are just chars.\n"
		<< "Introduce a string as input sequence.\n"
		<< "Then press ENTER and\n"
		<< "that puts it in running (debug) mode: just press ENTER for a single\n"
		<< "Do() execution of the TokenDelay object.\n"
		<< "and enter an integer for changing the actual delay value\n"
		<< "Exit entering a blank line in InputMode\n"
		<< "Exit the running mode entering 'q'\n\n";
}
void RunStep(string::iterator i, TokenDelay<CharDataObject>& delay)
{
	// encapsulate char
	CharDataObject* in = new CharDataObject();
	in->SetChr(*i);
	CharDataObject* out;
	//CharDataObject* out;
	
	delay.Do(*in, out);
	
	//Provisional:
	delay.FulfillsInvariant();
	delay.Debug();

	// desencapsulate char
	CharDataObject* chardata = dynamic_cast<CharDataObject*>(out);
	char chrout = (out)? chardata->GetChr() : '?';
	cout << "       delay output : " << chrout << "  delay input : " << *i << endl;


}

void ChangeDelay(istream & cin, TokenDelay<CharDataObject> & delay)
{
	cin.unget();
	TControlData aux;
	cin >> aux;
	unsigned idelay=(int)aux;
	enum { chgDelayControl = 0 };
	delay.DoControl(chgDelayControl, idelay);
	//Provisional:
	delay.FulfillsInvariant();

	cin.ignore();
}

