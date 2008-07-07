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

#ifndef __ProcessingClass2Ladspa_hxx__
#define __ProcessingClass2Ladspa_hxx__

#include <ladspa.h>
#include <vector>

#include "Audio.hxx"
#include "AudioOutPort.hxx"
#include "AudioInPort.hxx"
#include "InControl.hxx"
#include "OutControl.hxx"
#include "Processing.hxx"

namespace CLAM
{

class ProcessingClass2LadspaBase
{
public:	
	std::vector<CLAM::TData *> mLocationsList;
	std::vector<CLAM::TData *> mInLocationsList;

	std::vector<CLAM::AudioOutPort*> mWrappersList;
	std::vector<LADSPA_Data*> mInControlsList;
	std::vector<LADSPA_Data*> mOutControlsList;
	int mId;
	void AddWrapperPort( CLAM::TData * data );
	void AddLocation( CLAM::TData * data );
public:

	void DoProc();
	unsigned GetInControlsSize() const;
	unsigned GetOutControlsSize() const;
	unsigned GetInPortsSize() const;
	unsigned GetOutPortsSize() const;
	
	unsigned GetInControlsOffset() const;
	unsigned GetOutControlsOffset() const;
	unsigned GetInPortsOffset() const;
	unsigned GetOutPortsOffset() const;
	void DoControls();
	void SetPortSizes(int size);
	const char* GetClassName();
	const char * GetInControlName(int id);
	const char * GetOutControlName(int id);
	const char * GetInPortName(int id);
	const char * GetOutPortName(int id);

	ProcessingClass2LadspaBase(int id)
	{
		mId = id;
	}
	int GetId()
	{
		return mId;
	}
// Ladspa entry points
public:
	void Activate()
	{
		for(int i=0;i<_proc->GetInPorts().Size();i++)	
			mWrappersList[i]->ConnectToIn( _proc->GetInPorts().GetByNumber(i) );
		_proc->Start();
	}
	void ConnectPort(unsigned long port, LADSPA_Data * data)
	{
		if(port < GetInControlsOffset() + GetInControlsSize()) // is an "in control"
		{
			mInControlsList[port] = data;
			return;
		}
		if(port < GetOutControlsOffset() + GetOutControlsSize()) // is an "out control"
		{
			mOutControlsList[port-GetOutControlsOffset()] = data;
			return;
		}
		if(port < GetInPortsOffset() + GetInPortsSize())
		{
			std::cout << "connect in port" << std::endl;
			AddWrapperPort( (CLAM::TData*)data );
			return;
		}

		if(port < GetOutPortsOffset() + GetOutPortsSize()) 
		{
			std::cout << "connect out port" << std::endl;
			AddLocation( (CLAM::TData*)data );
			return;
		}
		CLAM_ASSERT(false, "port out of range");
	}
	void Run(unsigned long sampleCount)
	{
		DoControls();
		SetPortSizes(sampleCount);
		DoProc();
	}
	void Deactivate()
	{
		_proc->Stop();
	}

	unsigned NPorts() const
	{
		return
			GetInControlsSize() + GetOutControlsSize() +
			GetInPortsSize() + GetOutPortsSize();
	}

	LADSPA_Descriptor * CreateDescriptor();

	void CleanUpDescriptor(LADSPA_Descriptor *& descriptor)
	{
		if (not descriptor) return;
		free((char *)descriptor->Label);
		free((char *)descriptor->Name);
		free((char *)descriptor->Maker);
		free((char *)descriptor->Copyright);
		
		free((LADSPA_PortDescriptor *)descriptor->PortDescriptors);
		for (unsigned long lIndex = 0; lIndex < descriptor->PortCount; lIndex++)
			 free((char *)(descriptor->PortNames[lIndex]));
		free((char **)descriptor->PortNames);
		free((LADSPA_PortRangeHint *)descriptor->PortRangeHints);
		free(descriptor);
		descriptor=0;
	}
private:
	void SetPortsAndControls(LADSPA_Descriptor *& descriptor)
	{
		LADSPA_PortDescriptor *& portDescriptors = const_cast<LADSPA_PortDescriptor*&>(descriptor->PortDescriptors);
		char **& portNames = const_cast<char **&>(descriptor->PortNames);
		descriptor->PortDescriptors = (LADSPA_PortDescriptor *)calloc(NPorts(), sizeof(LADSPA_PortDescriptor));
		portDescriptors = (LADSPA_PortDescriptor *)calloc(NPorts(), sizeof(LADSPA_PortDescriptor));
		portNames = (char **)calloc(NPorts(), sizeof(char *));

		unsigned i=0;
		for(unsigned j=0; i<GetInControlsOffset()+GetInControlsSize() ; i++, j++)
		{
			portDescriptors[i] = LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL; 
			portNames[i] = strdup(GetInControlName(j));
		}

		for(unsigned j=0; i<GetOutControlsOffset()+GetOutControlsSize() ; i++, j++)
		{
			portDescriptors[i] = LADSPA_PORT_OUTPUT | LADSPA_PORT_CONTROL; 
			portNames[i] =  strdup(GetOutControlName(j));
		}
		for(unsigned j=0; i<GetInPortsOffset()+GetInPortsSize() ; i++, j++)
		{
			portDescriptors[i] = LADSPA_PORT_INPUT | LADSPA_PORT_AUDIO; 
			portNames[i] =  strdup(GetInPortName(j));
		}
		for(unsigned j=0; i<GetOutPortsOffset()+GetOutPortsSize() ; i++, j++)
		{
			portDescriptors[i] = LADSPA_PORT_OUTPUT | LADSPA_PORT_AUDIO; 
			portNames[i] =  strdup(GetOutPortName(j));
		}
	}

	void SetPortRangeHints(LADSPA_Descriptor *& descriptor)
	{
		LADSPA_PortRangeHint *& portRangeHints = const_cast<LADSPA_PortRangeHint *&>(descriptor->PortRangeHints);
		portRangeHints = ((LADSPA_PortRangeHint *)calloc(NPorts(), sizeof(LADSPA_PortRangeHint)));
		for(unsigned i=0; i<NPorts(); i++)
			portRangeHints[i].HintDescriptor = 0;
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
protected:
	CLAM::Processing * _proc;
};

template<class Proc>
class ProcessingClass2Ladspa : public ProcessingClass2LadspaBase
{
public:
	ProcessingClass2Ladspa(int id)
		: ProcessingClass2LadspaBase(id)
	{
		_proc = new Proc;
	}
	~ProcessingClass2Ladspa()
	{
		delete _proc;
	}

};

}


#endif // __ProcessingClass2Ladspa_hxx__

