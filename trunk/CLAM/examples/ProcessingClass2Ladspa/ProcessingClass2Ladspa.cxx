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

// LADSPA call backs
extern "C"
{

LADSPA_Handle Instantiate(const struct _LADSPA_Descriptor * descriptor,  unsigned long sampleRate)
{
	std::string className = std::string(descriptor->Name).substr(5);
	CLAM::ProcessingClass2LadspaBase * adapter = new CLAM::ProcessingClass2LadspaBase(className);
	adapter->Instantiate();
	return adapter;
}

void ConnectPort(LADSPA_Handle instance, unsigned long port, LADSPA_Data * data)
{
	CLAM::ProcessingClass2LadspaBase * adapter = (CLAM::ProcessingClass2LadspaBase*)instance;
	adapter->ConnectPort(port, data);
}

void Activate(LADSPA_Handle instance)
{
	CLAM::ProcessingClass2LadspaBase* adapter = (CLAM::ProcessingClass2LadspaBase*)instance;
	adapter->Activate();
}
  
void Run(LADSPA_Handle instance, unsigned long sampleCount)
{
	CLAM::ProcessingClass2LadspaBase * adapter = (CLAM::ProcessingClass2LadspaBase*)instance;
	adapter->Run(sampleCount);
}

void Deactivate(LADSPA_Handle instance)
{
	CLAM::ProcessingClass2LadspaBase * adapter = (CLAM::ProcessingClass2LadspaBase*)instance;
	adapter->Deactivate();
}

void CleanUp(LADSPA_Handle instance)
{
	CLAM::ProcessingClass2LadspaBase * adapter = (CLAM::ProcessingClass2LadspaBase*)instance;
	delete adapter;
}
}

namespace CLAM
{

LADSPA_Descriptor * ProcessingClass2LadspaBase::CreateDescriptor(unsigned long id)
{
	LADSPA_Descriptor * descriptor = (LADSPA_Descriptor *)malloc(sizeof(LADSPA_Descriptor));
	std::string className = _proc->GetClassName();
	descriptor->UniqueID  = id;
	descriptor->Label = strdup(("CLAM_"+className).c_str());
	descriptor->Name = strdup(("CLAM "+className).c_str());
	descriptor->Maker = "CLAM-dev";
	descriptor->Copyright = "None";
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

	return descriptor;
}

void ProcessingClass2LadspaBase::SetPortsAndControls(LADSPA_Descriptor *& descriptor)
{
	LADSPA_PortDescriptor *& portDescriptors = const_cast<LADSPA_PortDescriptor*&>(descriptor->PortDescriptors);
	char **& portNames = const_cast<char **&>(descriptor->PortNames);
	LADSPA_PortRangeHint *& portRangeHints = const_cast<LADSPA_PortRangeHint *&>(descriptor->PortRangeHints);

	portDescriptors = (LADSPA_PortDescriptor *)calloc(NPorts(), sizeof(LADSPA_PortDescriptor));
	portNames = (char **)calloc(NPorts(), sizeof(char *));
	portRangeHints = ((LADSPA_PortRangeHint *)calloc(NPorts(), sizeof(LADSPA_PortRangeHint)));

	unsigned i=0;
	for(unsigned j=0; j<GetInControlsSize() ; i++, j++)
	{
		portDescriptors[i] = LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL; 
		portNames[i] = strdup(GetInControlName(j));
		portRangeHints[i].HintDescriptor = 0;
	}

	for(unsigned j=0; j<GetOutControlsSize() ; i++, j++)
	{
		portDescriptors[i] = LADSPA_PORT_OUTPUT | LADSPA_PORT_CONTROL; 
		portNames[i] =  strdup(GetOutControlName(j));
		portRangeHints[i].HintDescriptor = 0;
	}
	for(unsigned j=0; j<GetInPortsSize() ; i++, j++)
	{
		portDescriptors[i] = LADSPA_PORT_INPUT | LADSPA_PORT_AUDIO; 
		portNames[i] =  strdup(GetInPortName(j));
		portRangeHints[i].HintDescriptor = 0;
	}
	for(unsigned j=0; j<GetOutPortsSize() ; i++, j++)
	{
		portDescriptors[i] = LADSPA_PORT_OUTPUT | LADSPA_PORT_AUDIO; 
		portNames[i] =  strdup(GetOutPortName(j));
		portRangeHints[i].HintDescriptor = 0;
	}
/*
	// pitch
	portRangeHints[0].HintDescriptor = (
		LADSPA_HINT_BOUNDED_BELOW |
		LADSPA_HINT_BOUNDED_ABOVE |
		LADSPA_HINT_SAMPLE_RATE |
		LADSPA_HINT_LOGARITHMIC |
		LADSPA_HINT_DEFAULT_440);
	portRangeHints[0].LowerBound = 0;
	portRangeHints[0].UpperBound = 0.5;
	   
	// amplitude
	portRangeHints[1].HintDescriptor = (
		LADSPA_HINT_BOUNDED_BELOW | 
		LADSPA_HINT_BOUNDED_ABOVE |
		LADSPA_HINT_DEFAULT_1);
	portRangeHints[1].LowerBound = 0;
	portRangeHints[1].UpperBound = 1;
			
	// audio output
	portRangeHints[2] = 0;
*/
}
void ProcessingClass2LadspaBase::CleanUpDescriptor(LADSPA_Descriptor *& descriptor)
{
	if (not descriptor) return;
	free((char *)descriptor->Label);
	free((char *)descriptor->Name);
	free((LADSPA_PortDescriptor *)descriptor->PortDescriptors);
	for (unsigned long lIndex = 0; lIndex < descriptor->PortCount; lIndex++)
		 free((char *)(descriptor->PortNames[lIndex]));
	free((char **)descriptor->PortNames);
	free((LADSPA_PortRangeHint *)descriptor->PortRangeHints);
	free(descriptor);
	descriptor=0;
}

void ProcessingClass2LadspaBase::DoControls()
{
	for(unsigned i=0;i<GetInControlsSize();i++)
		_proc->GetInControls().GetByNumber(i).DoControl((CLAM::TData)*_incontrolBuffers[i]);
	// TODO: No output controls!
}	

void ProcessingClass2LadspaBase::SetPortSizes(int size)
{		
	for(int i=0;i<_proc->GetInPorts().Size();i++)
	{
		if(_proc->GetInPorts().GetByNumber(i).GetSize() == size ) continue;
		_proc->GetInPorts().GetByNumber(i).SetSize( size );
		mWrappersList[i]->SetSize( size );
	}

	for(int i=0;i<_proc->GetOutPorts().Size();i++)
	{
		if(_proc->GetOutPorts().GetByNumber(i).GetSize() == size ) continue;
		_proc->GetOutPorts().GetByNumber(i).SetSize( size );
	}
}

void ProcessingClass2LadspaBase::DoProc(unsigned long nSamples)
{
	for(int i=0;i<_proc->GetInPorts().Size();i++)
	{
		memcpy( &(mWrappersList[i]->GetData()), _inportBuffers[i], nSamples*sizeof(CLAM::TData) );
		mWrappersList[i]->Produce();
	}

	std::vector<CLAM::TData*> dataList(GetOutPortsSize());
	for(int i=0;i<_proc->GetOutPorts().Size();i++)
	{
		CLAM::OutPortBase & port = _proc->GetOutPorts().GetByNumber(i);
		CLAM::AudioOutPort & audioPort = dynamic_cast<CLAM::AudioOutPort&>(port);
		dataList[i] = &(audioPort.GetData());
	}

	_proc->Do();

	for(int i=0; i<_proc->GetOutPorts().Size(); i++)
		memcpy(_outcontrolBuffers[i], dataList[i], nSamples*sizeof(CLAM::TData) );		
}



const char * ProcessingClass2LadspaBase::GetInControlName(int id) const
{
	return _proc->GetInControls().GetByNumber(id).GetName().c_str();
}
const char * ProcessingClass2LadspaBase::GetOutControlName(int id) const
{
	return _proc->GetOutControls().GetByNumber(id).GetName().c_str();
}
const char * ProcessingClass2LadspaBase::GetInPortName(int id) const
{
	return _proc->GetInPorts().GetByNumber(id).GetName().c_str();
}
const char * ProcessingClass2LadspaBase::GetOutPortName(int id) const
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

}

