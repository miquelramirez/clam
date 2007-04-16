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

#ifndef __LadspaLoader_hxx__
#define __LadspaLoader_hxx__

#include "ProcessingConfig.hxx"
#include "Processing.hxx"
#include <ladspa.h>
#include <dlfcn.h>
#include <string>
#include "Filename.hxx"

namespace CLAM
{
	class AudioInPort;
	class AudioOutPort;
	class InControl;
	class OutControl;

/**
 * This class is a helper provided to know which plugins are located inside a shared object.
 */
class LadspaPluginExaminer
{
public:
	typedef std::list< std::string > NamesList;

	LadspaPluginExaminer( const std::string & library );
	NamesList::iterator BeginDescriptors(){return mDescriptorsList.begin();}
	NamesList::iterator EndDescriptors(){return mDescriptorsList.end();}
	int GetIndex( const std::string & descriptor );
private:
	NamesList mDescriptorsList;
};

class LadspaLoaderConfig : public ProcessingConfig
{
public:
	DYNAMIC_TYPE_USING_INTERFACE (LadspaLoaderConfig, 4, ProcessingConfig);
	DYN_ATTRIBUTE (0, public, Filename, SharedObjectName );
	DYN_ATTRIBUTE (1, public, int, Index );
	DYN_ATTRIBUTE (2, public, unsigned long, SampleRate );
	DYN_ATTRIBUTE (3, public, unsigned long, Size );
protected:
	void DefaultInit()
	{
		AddAll();
		UpdateData();
		SetSharedObjectName("");
		SetIndex(0);
		SetSampleRate(44100);
		SetSize(512);
	}
};

class LadspaLoader : public Processing
{
	LadspaLoaderConfig mConfig;
	typedef void * SOPointer;
	LADSPA_Handle mInstance;
	const LADSPA_Descriptor * mDescriptor;
	SOPointer mSharedObject;

	std::vector< AudioInPort* > mInputPorts;
	std::vector< AudioOutPort* > mOutputPorts;

	std::vector< InControl* > mInputControls;
	std::vector< OutControl* > mOutputControls;
	std::vector< LADSPA_Data > mInputControlValues;
	std::vector< LADSPA_Data > mOutputControlValues;

	void ConfigurePortsAndControls();
	void RemovePortsAndControls();
	void UpdatePointers();
	
public:
	LadspaLoader();
	LadspaLoader( const LadspaLoaderConfig & cfg);
	virtual ~LadspaLoader();
	bool Do();
	bool ConcreteStop();
	bool ConcreteStart();		
	bool ConcreteConfigure( const ProcessingConfig & cfg);

	bool ModifiesPortsAndControlsAtConfiguration()
	{
		return true;
	}
	
	const char* GetClassName() const
	{ 
		return "LadspaLoader";
	}

	const ProcessingConfig &GetConfig() const 
	{ 
		return mConfig;
	}
};
	
} // namespace CLAM

#endif // __LadspaLoader_hxx__

