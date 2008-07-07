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


class ProcessingClass2LadspaBase
{
public:	
	std::vector<CLAM::TData *> mLocationsList;
	std::vector<CLAM::TData *> mInLocationsList;

	std::vector<CLAM::AudioOutPort*> mWrappersList;
	std::vector<LADSPA_Data*> mInControlsList;
	std::vector<LADSPA_Data*> mOutControlsList;
	int mId;
	virtual void AddWrapperPort( CLAM::TData * DataLocation );
	virtual void AddLocation( CLAM::TData * DataLocation );
public:

	virtual void Start();
	virtual void Stop();
	virtual void DoProc();
	virtual int GetInControlsSize();
	virtual int GetOutControlsSize();
	virtual int GetInPortsSize();
	virtual int GetOutPortsSize();
	
	virtual int GetInControlsIndex();
	virtual int GetOutControlsIndex();
	virtual int GetInPortsIndex();
	virtual int GetOutPortsIndex();
	virtual void DoControls();
	virtual void DoSizeCheck(int size);
	virtual const char* GetClassName();
	virtual const char * GetInControlName(int id);
	virtual const char * GetOutControlName(int id);
	virtual const char * GetInPortName(int id);
	virtual const char * GetOutPortName(int id);

	ProcessingClass2LadspaBase(int id)
	{
		mId = id;
	}
	int GetId()
	{
		return mId;
	}
protected:
	CLAM::Processing * _proc;
};

template<class Proc>
class ProcessingClass2Ladspa : public ProcessingClass2LadspaBase
{
public:
	static ProcessingClass2LadspaBase* Create(int id) { return new ProcessingClass2Ladspa<Proc>(id); }
	
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


#endif // __ProcessingClass2Ladspa_hxx__

