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

extern CLAM::ProcessingClass2LadspaBase * Instance();

// LADSPA call backs
extern "C"
{

LADSPA_Handle Instantiate(const struct _LADSPA_Descriptor * Descriptor,  unsigned long SampleRate)
{
	CLAM::ProcessingClass2LadspaBase * test = Instance();

	(test->mInControlsList).resize(test->GetInControlsSize());
	(test->mOutControlsList).resize(test->GetOutControlsSize());

	return test;
}

void ConnectPort(LADSPA_Handle Instance, unsigned long port, LADSPA_Data * data)
{
	CLAM::ProcessingClass2LadspaBase * adapter = (CLAM::ProcessingClass2LadspaBase*)Instance;
	adapter->ConnectPort(port, data);
}

void Activate(LADSPA_Handle Instance)
{
	CLAM::ProcessingClass2LadspaBase* adapter = (CLAM::ProcessingClass2LadspaBase*)Instance;
	adapter->Activate();
}
  
void Run(LADSPA_Handle Instance, unsigned long sampleCount)
{
	CLAM::ProcessingClass2LadspaBase * adapter = (CLAM::ProcessingClass2LadspaBase*)Instance;
	adapter->Run(sampleCount);
}

void Deactivate(LADSPA_Handle Instance)
{
	CLAM::ProcessingClass2LadspaBase * adapter = (CLAM::ProcessingClass2LadspaBase*)Instance;
	adapter->Deactivate();
}

void CleanUp(LADSPA_Handle Instance)
{
	CLAM::ProcessingClass2LadspaBase * adapter = (CLAM::ProcessingClass2LadspaBase*)Instance;
	delete adapter;
}
}

namespace CLAM
{

class LadspaLibrary
{
	LadspaLibrary(...)
	{
	}

};


LADSPA_Descriptor * ProcessingClass2LadspaBase::CreateDescriptor()
{
	LADSPA_Descriptor * descriptor = (LADSPA_Descriptor *)malloc(sizeof(LADSPA_Descriptor));
	descriptor->UniqueID  = GetId(); //?
	descriptor->Label = strdup(GetClassName());
	descriptor->Name = strdup(GetClassName());
	descriptor->Maker = strdup("CLAM-dev");
	descriptor->Copyright = strdup("None");
	descriptor->Properties = LADSPA_PROPERTY_HARD_RT_CAPABLE; //?
	descriptor->PortCount = NPorts();

	descriptor->instantiate  = ::Instantiate;
	descriptor->connect_port = ::ConnectPort;
	descriptor->activate = ::Activate;
	descriptor->run = ::Run;
	descriptor->run_adding = 0;
	descriptor->set_run_adding_gain = 0;
	descriptor->deactivate = ::Deactivate;
	descriptor->cleanup = ::CleanUp;

	SetPortsAndControls(descriptor);
	SetPortRangeHints(descriptor);

	return descriptor;
}

void ProcessingClass2LadspaBase::AddWrapperPort( CLAM::TData * data )
{
	mInLocationsList.push_back(data);

	CLAM::AudioOutPort * out = new CLAM::AudioOutPort("out", 0 );
	mWrappersList.push_back(out);
}

void ProcessingClass2LadspaBase::AddLocation( CLAM::TData * data )
{
	mLocationsList.push_back(data);
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

unsigned ProcessingClass2LadspaBase::GetInControlsSize() const
{
	return _proc->GetInControls().Size();
}

unsigned ProcessingClass2LadspaBase::GetOutControlsSize() const
{
	return _proc->GetOutControls().Size();
}

unsigned ProcessingClass2LadspaBase::GetInPortsSize() const
{
	return _proc->GetInPorts().Size();
}

unsigned ProcessingClass2LadspaBase::GetOutPortsSize() const
{
	return _proc->GetOutPorts().Size();
}

// TODO: Rename Offset -> Offset
unsigned ProcessingClass2LadspaBase::GetInControlsOffset() const
{
	return 0;
}

unsigned ProcessingClass2LadspaBase::GetOutControlsOffset() const
{
	return GetInControlsSize();
}

unsigned ProcessingClass2LadspaBase::GetInPortsOffset() const
{
	return GetOutControlsOffset() + GetOutControlsSize();
}

unsigned ProcessingClass2LadspaBase::GetOutPortsOffset() const
{
	return GetInPortsOffset() + GetInPortsSize();
}

void ProcessingClass2LadspaBase::DoControls()
{
	for(unsigned i=0;i<GetInControlsSize();i++)
		_proc->GetInControls().GetByNumber(i).DoControl((CLAM::TData)*mInControlsList[i]);
}	

void ProcessingClass2LadspaBase::SetPortSizes(int size)
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

namespace Hidden
{

static LADSPA_Descriptor * g_psDescriptor;

class StartupShutdownHandler 
{
	CLAM::ProcessingClass2LadspaBase* _adapter;
public:
	StartupShutdownHandler():
		_adapter(0)
	{
		_adapter = Instance();
		g_psDescriptor = _adapter->CreateDescriptor();
	}
	~StartupShutdownHandler() 
	{
		_adapter->CleanUpDescriptor(g_psDescriptor);
	}


};

static StartupShutdownHandler g_oShutdownStartupHandler;

}
}

extern "C"
{

const LADSPA_Descriptor * ladspa_descriptor(unsigned long index)
{
	switch (index)
	{
		case 0: return CLAM::Hidden::g_psDescriptor;
		default: return NULL;
	}
}

}


