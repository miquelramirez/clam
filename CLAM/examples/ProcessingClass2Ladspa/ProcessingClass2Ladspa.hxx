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
#include <map>

#include "Audio.hxx"
#include "AudioOutPort.hxx"
#include "AudioInPort.hxx"
#include "InControl.hxx"
#include "OutControl.hxx"
#include "Processing.hxx"
#include "ProcessingFactory.hxx"

namespace CLAM
{

class ProcessingClass2LadspaBase
{
	std::vector<LADSPA_Data *> _portBuffers;
	LADSPA_Data ** _outportBuffers;
	LADSPA_Data ** _inportBuffers;
	LADSPA_Data ** _incontrolBuffers;
	LADSPA_Data ** _outcontrolBuffers;
	std::vector<AudioOutPort*> mWrappersList;
private:
	Processing * _proc;
public:
// Ladspa entry points
	// Instantiate
	ProcessingClass2LadspaBase(CLAM::Processing * processing)
		: _proc(0)
	{
		SetProcessing(processing);
	}
	ProcessingClass2LadspaBase(const std::string & className)
		: _proc(0)
	{
		SetProcessing(ProcessingFactory::GetInstance().Create(className));
	}
	void Instantiate()
	{
		_portBuffers.resize(NPorts());
		_incontrolBuffers = &(_portBuffers[0]) + GetInControlsOffset();
		_outcontrolBuffers = &(_portBuffers[0]) + GetOutControlsOffset();
		_inportBuffers = &(_portBuffers[0]) + GetInPortsOffset();
		_outportBuffers = &(_portBuffers[0]) + GetOutPortsOffset();

		mWrappersList.resize(GetInPortsSize());
		for (unsigned i=0; i<mWrappersList.size(); i++)
		{
			mWrappersList[i] = new AudioOutPort("out", 0 );
			mWrappersList[i]->ConnectToIn( _proc->GetInPorts().GetByNumber(i) );
		}
	}
	
	void Activate()
	{
		_proc->Start();
	}
	void ConnectPort(unsigned long port, LADSPA_Data * data)
	{
		_portBuffers[port] = data;
	}
	void Run(unsigned long sampleCount)
	{
		DoControls();
		SetPortSizes(sampleCount);
		DoProc(sampleCount);
	}
	void Deactivate()
	{
		_proc->Stop();
	}

	// CleanUp
	~ProcessingClass2LadspaBase()
	{
		for (unsigned i=0; i<mWrappersList.size(); i++)
			delete mWrappersList[i];
		SetProcessing(0);
	}

private:
	void DoProc(unsigned long nSamples);
	void DoControls();
	void SetPortSizes(int size);


// Pre instantiation interface
public:
	LADSPA_Descriptor * CreateDescriptor(unsigned long id);
	static void CleanUpDescriptor(LADSPA_Descriptor *& descriptor);
private:
	void SetPortsAndControls(LADSPA_Descriptor *& descriptor);

protected:
	void SetProcessing(Processing * processing)
	{
		if (_proc) delete _proc;
		_proc = processing;
	}

// Helper shortcuts
private:
	const char * GetInControlName(int id) const;
	const char * GetOutControlName(int id) const;
	const char * GetInPortName(int id) const;
	const char * GetOutPortName(int id) const;

	unsigned GetInControlsSize() const;
	unsigned GetOutControlsSize() const;
	unsigned GetInPortsSize() const;
	unsigned GetOutPortsSize() const;
	
	unsigned GetInControlsOffset() const;
	unsigned GetOutControlsOffset() const;
	unsigned GetInPortsOffset() const;
	unsigned GetOutPortsOffset() const;

	unsigned NPorts() const
	{
		return
			GetInControlsSize() + GetOutControlsSize() +
			GetInPortsSize() + GetOutPortsSize();
	}

};

class LadspaLibrary
{
	std::vector<LADSPA_Descriptor * > _descriptors;
public:
	LadspaLibrary()
	{
	}
	~LadspaLibrary()
	{
		for (unsigned i=0; i<_descriptors.size(); i++)
			ProcessingClass2LadspaBase::CleanUpDescriptor(_descriptors[i]);
	}
	void AddPluginType(LADSPA_Descriptor * descriptor)
	{
		_descriptors.push_back(descriptor);
	}
	LADSPA_Descriptor * pluginAt(unsigned long i)
	{
		if (i>=_descriptors.size()) return 0;
		return _descriptors[i];
	}
	template <typename ProcessingType>
	class ProcessingExporter
	{
	public:
		ProcessingExporter(LadspaLibrary & library, unsigned long id)
		{
			ProcessingClass2LadspaBase adapter(new ProcessingType);
			LADSPA_Descriptor * descriptor = adapter.CreateDescriptor(id);
			library.AddPluginType(descriptor);
		}
	};
};


}


#endif // __ProcessingClass2Ladspa_hxx__

