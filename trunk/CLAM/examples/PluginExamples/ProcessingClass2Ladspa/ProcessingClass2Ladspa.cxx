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

static LADSPA_Handle Instantiate(const struct _LADSPA_Descriptor * descriptor,  unsigned long sampleRate)
{
	std::string className = std::string(descriptor->Name).substr(5);
	CLAM::ProcessingClass2LadspaBase * adapter = new CLAM::ProcessingClass2LadspaBase(className);
	adapter->Instantiate();
	return adapter;
}

static void ConnectPort(LADSPA_Handle instance, unsigned long port, LADSPA_Data * data)
{
	CLAM::ProcessingClass2LadspaBase * adapter = (CLAM::ProcessingClass2LadspaBase*)instance;
	adapter->ConnectPort(port, data);
}

static void Activate(LADSPA_Handle instance)
{
	CLAM::ProcessingClass2LadspaBase* adapter = (CLAM::ProcessingClass2LadspaBase*)instance;
	adapter->Activate();
}
  
static void Run(LADSPA_Handle instance, unsigned long sampleCount)
{
	CLAM::ProcessingClass2LadspaBase * adapter = (CLAM::ProcessingClass2LadspaBase*)instance;
	adapter->Run(sampleCount);
}

static void Deactivate(LADSPA_Handle instance)
{
	CLAM::ProcessingClass2LadspaBase * adapter = (CLAM::ProcessingClass2LadspaBase*)instance;
	adapter->Deactivate();
}

static void CleanUp(LADSPA_Handle instance)
{
	CLAM::ProcessingClass2LadspaBase * adapter = (CLAM::ProcessingClass2LadspaBase*)instance;
	delete adapter;
}
}

namespace CLAM
{

LADSPA_Descriptor * ProcessingClass2LadspaBase::CreateDescriptor(unsigned long id)
{
	LADSPA_Descriptor * descriptor = new LADSPA_Descriptor;
	std::string className = _proc->GetClassName();
	descriptor->UniqueID  = id;
	descriptor->Label = LadspaLibrary::dupstr(("CLAM_"+className).c_str());
	descriptor->Name = LadspaLibrary::dupstr(("CLAM "+className).c_str());
	descriptor->Maker = LadspaLibrary::dupstr("CLAM-dev");
	descriptor->Copyright = LadspaLibrary::dupstr("None");
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
	const char **& portNames = const_cast<const char **&>(descriptor->PortNames);
	LADSPA_PortRangeHint *& portRangeHints = const_cast<LADSPA_PortRangeHint *&>(descriptor->PortRangeHints);

	typedef const char * ConstCharPtr;
	portNames = new ConstCharPtr[NPorts()];
	portDescriptors = new LADSPA_PortDescriptor[NPorts()];
	portRangeHints = new LADSPA_PortRangeHint[NPorts()];

	unsigned i=0;
	for(unsigned j=0; j<_nInControls; i++, j++)
	{
		portDescriptors[i] = LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL; 
		portNames[i] = LadspaLibrary::dupstr(GetInControlName(j));
		portRangeHints[i].HintDescriptor = 0;
	}

	for(unsigned j=0; j<_nOutControls; i++, j++)
	{
		portDescriptors[i] = LADSPA_PORT_OUTPUT | LADSPA_PORT_CONTROL; 
		portNames[i] =  LadspaLibrary::dupstr(GetOutControlName(j));
		portRangeHints[i].HintDescriptor = 0;
	}
	for(unsigned j=0; j<_nInPorts; i++, j++)
	{
		portDescriptors[i] = LADSPA_PORT_INPUT | LADSPA_PORT_AUDIO; 
		portNames[i] =  LadspaLibrary::dupstr(GetInPortName(j));
		portRangeHints[i].HintDescriptor = 0;
	}
	for(unsigned j=0; j<_nOutPorts; i++, j++)
	{
		portDescriptors[i] = LADSPA_PORT_OUTPUT | LADSPA_PORT_AUDIO; 
		portNames[i] =  LadspaLibrary::dupstr(GetOutPortName(j));
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
	for(unsigned i=0;i<_nInControls;i++)
		_proc->GetInControls().GetByNumber(i).DoControl((CLAM::TData)*_incontrolBuffers[i]);
	// TODO: No output controls!
}	

void ProcessingClass2LadspaBase::SetPortSizes(int size)
{		
	for(int i=0;i<_nInControls;i++)
	{
		if(_proc->GetInPorts().GetByNumber(i).GetSize() == size ) continue;
		_proc->GetInPorts().GetByNumber(i).SetSize( size );
		mWrappersList[i]->SetSize( size );
	}

	for(int i=0;i<_nOutPorts;i++)
	{
		if(_proc->GetOutPorts().GetByNumber(i).GetSize() == size ) continue;
		_proc->GetOutPorts().GetByNumber(i).SetSize( size );
	}
}

void ProcessingClass2LadspaBase::DoProc(unsigned long nSamples)
{
	for(int i=0;i<_nInPorts;i++)
	{
		memcpy( &(mWrappersList[i]->GetData()), _inportBuffers[i], nSamples*sizeof(CLAM::TData) );
		mWrappersList[i]->Produce();
	}

	std::vector<CLAM::TData*> dataList(_nOutPorts);
	for(int i=0;i<_nOutPorts;i++)
	{
		CLAM::OutPortBase & port = _proc->GetOutPorts().GetByNumber(i);
		CLAM::AudioOutPort & audioPort = dynamic_cast<CLAM::AudioOutPort&>(port);
		dataList[i] = &(audioPort.GetData());
	}

	_proc->Do();

	for(int i=0; i<_nOutControls; i++)
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

}

