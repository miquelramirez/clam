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

#include <cstdlib>
#include <cstring>

#include "ProcessingClass2Ladspa.hxx"

void ProcessingClass2LadspaBase::AddWrapperPort( CLAM::TData * DataLocation )
{
	mInLocationsList.push_back(DataLocation);

	CLAM::AudioOutPort * out = new CLAM::AudioOutPort("out", 0 );
	mWrappersList.push_back(out);
}

void ProcessingClass2LadspaBase::AddLocation( CLAM::TData * DataLocation )
{
	mLocationsList.push_back(DataLocation);
}

void ProcessingClass2LadspaBase::Start()
{
	for(int i=0;i<_proc->GetInPorts().Size();i++)	
		mWrappersList[i]->ConnectToIn( _proc->GetInPorts().GetByNumber(i) );
	_proc->Start();
}

void ProcessingClass2LadspaBase::Stop()
{
	_proc->Stop();
}
void ProcessingClass2LadspaBase::DoProc()
{
	for(int i=0;i<_proc->GetInPorts().Size();i++)
	{
		memcpy( &(mWrappersList[i]->GetData()), mInLocationsList[i], _proc->GetInPorts().GetByNumber(i).GetSize()*sizeof(CLAM::TData) );
		mWrappersList[i]->Produce();
	}

	std::vector<CLAM::TData*> mDataList;
	for(int i=0;i<_proc->GetOutPorts().Size();i++)
	{
		CLAM::AudioOutPort & out = (CLAM::AudioOutPort&)(_proc->GetOutPorts().GetByNumber(i));
		mDataList.push_back( &(out.GetData()));
	}

	_proc->Do();
	for(int i=0;i<_proc->GetOutPorts().Size();i++)
		memcpy(mLocationsList[i], mDataList[i], _proc->GetOutPorts().GetByNumber(i).GetSize()*sizeof(CLAM::TData) );		
}
const char* ProcessingClass2LadspaBase::GetClassName()
{
	return _proc->GetClassName();
}

const char * ProcessingClass2LadspaBase::GetInControlName(int id)
{
	return _proc->GetInControls().GetByNumber(id).GetName().c_str();
}

const char * ProcessingClass2LadspaBase::GetOutControlName(int id)
{
	return _proc->GetOutControls().GetByNumber(id).GetName().c_str();
}

const char * ProcessingClass2LadspaBase::GetInPortName(int id)
{
	return _proc->GetInPorts().GetByNumber(id).GetName().c_str();
}

const char * ProcessingClass2LadspaBase::GetOutPortName(int id)
{
	return _proc->GetOutPorts().GetByNumber(id).GetName().c_str();
}


int ProcessingClass2LadspaBase::GetInControlsSize()
{
	return _proc->GetInControls().Size();
}

int ProcessingClass2LadspaBase::GetOutControlsSize()
{
	return _proc->GetOutControls().Size();
}

int ProcessingClass2LadspaBase::GetInPortsSize()
{
	return _proc->GetInPorts().Size();
}

int ProcessingClass2LadspaBase::GetOutPortsSize()
{
	return _proc->GetOutPorts().Size();
}

// TODO: Rename Index -> Offset
int ProcessingClass2LadspaBase::GetInControlsIndex()
{
	return 0;
}

int ProcessingClass2LadspaBase::GetOutControlsIndex()
{
	return GetInControlsSize();
}

int ProcessingClass2LadspaBase::GetInPortsIndex()
{
	return GetOutControlsIndex() + GetOutControlsSize();
}

int ProcessingClass2LadspaBase::GetOutPortsIndex()
{
	return GetInPortsIndex() + GetInPortsSize();
}

void ProcessingClass2LadspaBase::DoControls()
{
	for(int i=0;i<GetInControlsSize();i++)
		_proc->GetInControls().GetByNumber(i).DoControl((CLAM::TData)*mInControlsList[i]);
}	

// TODO: Rename DoSizeCheck -> SetPortSizes
void ProcessingClass2LadspaBase::DoSizeCheck(int size)
{		
	for(int i=0;i<_proc->GetInPorts().Size();i++)
	{
		if(_proc->GetInPorts().GetByNumber(i).GetSize() != size )
		{
			_proc->GetInPorts().GetByNumber(i).SetSize( size );
			mWrappersList[i]->SetSize( size );
		}
	}
		
	for(int i=0;i<_proc->GetOutPorts().Size();i++)
	{
		if(_proc->GetOutPorts().GetByNumber(i).GetSize() != size )
			_proc->GetOutPorts().GetByNumber(i).SetSize( size );
	}
}

extern ProcessingClass2LadspaBase * Instance(void);

extern "C"
{

LADSPA_Handle Instantiate(const struct _LADSPA_Descriptor * Descriptor,  unsigned long SampleRate)
{
	ProcessingClass2LadspaBase * test = Instance();

	(test->mInControlsList).resize(test->GetInControlsSize());
	(test->mOutControlsList).resize(test->GetOutControlsSize());

	return test;
}

void ConnectPort(LADSPA_Handle Instance, unsigned long Port, LADSPA_Data * DataLocation)
{
	ProcessingClass2LadspaBase * test = (ProcessingClass2LadspaBase*)Instance;

	if(Port < test->GetInControlsIndex() + test->GetInControlsSize()) // is an "in control"
	{
		test->mInControlsList[Port] = DataLocation;
		return;
	}
	if(Port < test->GetOutControlsIndex() + test->GetOutControlsSize()) // is an "out control"
	{
		test->mOutControlsList[Port-test->GetOutControlsIndex()] = DataLocation;
		return;
	}
	if(Port < test->GetInPortsIndex() + test->GetInPortsSize())
	{
		std::cout << "connect in port" << std::endl;

		test->AddWrapperPort( (CLAM::TData*)DataLocation );
		return;
	}

	if(Port < test->GetOutPortsIndex() + test->GetOutPortsSize()) 
	{
		std::cout << "connect out port" << std::endl;
		test->AddLocation( (CLAM::TData*)DataLocation );
		return;
	}
	CLAM_ASSERT(false, "port out of range");
}

void Activate(LADSPA_Handle Instance)
{
	ProcessingClass2LadspaBase* test = (ProcessingClass2LadspaBase*)Instance;
	test->Start();
}
  
void Run(LADSPA_Handle Instance, unsigned long SampleCount)
{
	ProcessingClass2LadspaBase * test = (ProcessingClass2LadspaBase*)Instance;
	test->DoControls();
	test->DoSizeCheck(SampleCount);
	test->DoProc();
}

void DeActivate(LADSPA_Handle Instance)
{
	ProcessingClass2LadspaBase * test = (ProcessingClass2LadspaBase*)Instance;
	test->Stop();
}

void CleanUp(LADSPA_Handle Instance)
{
	ProcessingClass2LadspaBase * test = (ProcessingClass2LadspaBase*)Instance;
	delete test;
}

LADSPA_Descriptor * g_psDescriptor;

}

namespace CLAM
{
namespace Hidden
{

class StartupShutdownHandler 
{
	ProcessingClass2LadspaBase* _test;
	char ** pcPortNames;
	LADSPA_PortDescriptor * piPortDescriptors;
	LADSPA_PortRangeHint * psPortRangeHints;
	int numPorts;

	void CreateDescriptor()
	{
		if (not g_psDescriptor) return;

		g_psDescriptor->UniqueID  = _test->GetId(); //?
		g_psDescriptor->Label = strdup(_test->GetClassName());
		g_psDescriptor->Name = strdup(_test->GetClassName());
		g_psDescriptor->Maker = strdup("CLAM-dev");
		g_psDescriptor->Copyright = strdup("Universitat Pompeu Fabra");
		g_psDescriptor->Properties = LADSPA_PROPERTY_HARD_RT_CAPABLE; //?
		g_psDescriptor->PortCount = numPorts;

		g_psDescriptor->instantiate  = Instantiate;
		g_psDescriptor->connect_port = ConnectPort;
		g_psDescriptor->activate = Activate;
		g_psDescriptor->run = Run;
		g_psDescriptor->run_adding = 0;
		g_psDescriptor->set_run_adding_gain = 0;
		g_psDescriptor->deactivate = DeActivate;
		g_psDescriptor->cleanup = CleanUp;
	}


	void SetPortsAndControls()
	{
		int i=0;
		for(int j=0; i<_test->GetInControlsIndex()+_test->GetInControlsSize() ; i++, j++)
		{
			piPortDescriptors[i] = LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL; 
			pcPortNames[i] = strdup(_test->GetInControlName(j));
		}

		for(int j=0; i<_test->GetOutControlsIndex()+_test->GetOutControlsSize() ; i++, j++)
		{
			piPortDescriptors[i] = LADSPA_PORT_OUTPUT | LADSPA_PORT_CONTROL; 
			pcPortNames[i] =  strdup(_test->GetOutControlName(j));
		}
		for(int j=0; i<_test->GetInPortsIndex()+_test->GetInPortsSize() ; i++, j++)
		{
			piPortDescriptors[i] = LADSPA_PORT_INPUT | LADSPA_PORT_AUDIO; 
			pcPortNames[i] =  strdup(_test->GetInPortName(j));
		}
		for(int j=0; i<_test->GetOutPortsIndex()+_test->GetOutPortsSize() ; i++, j++)
		{
			piPortDescriptors[i] = LADSPA_PORT_OUTPUT | LADSPA_PORT_AUDIO; 
			pcPortNames[i] =  strdup(_test->GetOutPortName(j));
		}
	}

	void SetPortRangeHints()
	{
		for(int i=0; i<numPorts; i++)
			psPortRangeHints[i].HintDescriptor = 0;
	}

public:
	~StartupShutdownHandler() 
	{
		if (not g_psDescriptor) return;
		free((char *)g_psDescriptor->Label);
		free((char *)g_psDescriptor->Name);
		free((char *)g_psDescriptor->Maker);
		free((char *)g_psDescriptor->Copyright);
		
		free((LADSPA_PortDescriptor *)g_psDescriptor->PortDescriptors);
		for (long lIndex = 0; lIndex < g_psDescriptor->PortCount; lIndex++)
			 free((char *)(g_psDescriptor->PortNames[lIndex]));
		free((char **)g_psDescriptor->PortNames);
		free((LADSPA_PortRangeHint *)g_psDescriptor->PortRangeHints);
		free(g_psDescriptor);   
	}


	StartupShutdownHandler():
		_test(0), numPorts(0)
			
	{
		_test = Instance();

		numPorts = 
			_test->GetInControlsSize()+_test->GetOutControlsSize() +
			_test->GetInPortsSize() + _test->GetOutPortsSize();

		g_psDescriptor = (LADSPA_Descriptor *)malloc(sizeof(LADSPA_Descriptor));
		CreateDescriptor();
		piPortDescriptors = (LADSPA_PortDescriptor *)calloc(numPorts, sizeof(LADSPA_PortDescriptor));
		g_psDescriptor->PortDescriptors = (const LADSPA_PortDescriptor *)piPortDescriptors;
		pcPortNames = (char **)calloc(numPorts, sizeof(char *));
		g_psDescriptor->PortNames = (const char **)pcPortNames;

		SetPortsAndControls();
		psPortRangeHints = ((LADSPA_PortRangeHint *)calloc(numPorts, sizeof(LADSPA_PortRangeHint)));
		g_psDescriptor->PortRangeHints = (const LADSPA_PortRangeHint *)psPortRangeHints;

		SetPortRangeHints();
	
/*
		// pitch
		psPortRangeHints[0].HintDescriptor = (
			LADSPA_HINT_BOUNDED_BELOW |
			LADSPA_HINT_BOUNDED_ABOVE |
			LADSPA_HINT_SAMPLE_RATE |
			LADSPA_HINT_LOGARITHMIC |
			LADSPA_HINT_DEFAULT_440);
		psPortRangeHints[0].LowerBound = 0;
		psPortRangeHints[0].UpperBound = 0.5;
		   
		// amplitude
		psPortRangeHints[1].HintDescriptor = (
			LADSPA_HINT_BOUNDED_BELOW | 
			LADSPA_HINT_BOUNDED_ABOVE |
			LADSPA_HINT_DEFAULT_1);
		psPortRangeHints[1].LowerBound = 0;
		psPortRangeHints[1].UpperBound = 1;
				
		// audio output
		psPortRangeHints[2] = 0;
*/
	}
} g_oShutdownStartupHandler;

}
}

extern "C"
{
  
const LADSPA_Descriptor * ladspa_descriptor(unsigned long index)
{
	switch (index)
	{
		case 0: return g_psDescriptor;
		default: return NULL;
	}
}

}


