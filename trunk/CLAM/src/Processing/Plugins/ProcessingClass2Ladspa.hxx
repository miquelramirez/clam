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


class ProcessingClass2Ladspa
{
public:	
	std::vector<CLAM::TData *> mLocationsList;
	std::vector<CLAM::TData *> mInLocationsList;

	std::vector<CLAM::AudioOutPort*> mWrappersList;
	std::vector<LADSPA_Data*> mInControlsList;
	std::vector<LADSPA_Data*> mOutControlsList;
	int mId;
	virtual void AddWrapperPort( CLAM::TData * DataLocation ) = 0;
	virtual void AddLocation( CLAM::TData * DataLocation ) = 0;
public:

	virtual void Start() = 0;
	virtual void Stop() = 0;
	virtual void DoProc() = 0;
	virtual bool CanDo() = 0;
	virtual int GetInControlsSize()=0;
	virtual int GetOutControlsSize()=0;
	virtual int GetInPortsSize()=0;
	virtual int GetOutPortsSize()=0;
	
	virtual int GetInControlsIndex() = 0;
	virtual int GetOutControlsIndex() = 0;
	virtual int GetInPortsIndex() = 0;
	virtual int GetOutPortsIndex() = 0;
	virtual void DoControls() = 0;
	virtual bool DoSizeCheck(int size) = 0;
	virtual const char* GetClassName() = 0;
	virtual const char * GetInControlName(int id) = 0;
	virtual const char * GetOutControlName(int id) = 0;
	virtual const char * GetInPortName(int id) = 0;
	virtual const char * GetOutPortName(int id) = 0;

	ProcessingClass2Ladspa(int id)
	{
		mId = id;
	}
	int GetId()
	{
		return mId;
	}
};

template<class Proc>
class ProcessingClass2LadspaTmpl : public ProcessingClass2Ladspa, public Proc 
{
public:
	static ProcessingClass2Ladspa* Create(int id) { return new ProcessingClass2LadspaTmpl<Proc>(id); }
	int GetInControlsIndex();
	int GetOutControlsIndex();
	int GetInPortsIndex();
	int GetOutPortsIndex();
	void DoControls();
	bool DoSizeCheck(int size);
	bool CanDo();
	
	ProcessingClass2LadspaTmpl(int id)
		: ProcessingClass2Ladspa(id)
	{
	}

	void AddWrapperPort( CLAM::TData * DataLocation )
	{
		mInLocationsList.push_back(DataLocation);

		CLAM::AudioOutPort * out = new CLAM::AudioOutPort("out", 0 );
		mWrappersList.push_back(out);
	}
	
	void AddLocation( CLAM::TData * DataLocation )
	{
		mLocationsList.push_back(DataLocation);
	}

	void Start()
	{
		for(int i=0;i<GetInPorts().Size();i++)	
			mWrappersList[i]->ConnectToIn( GetInPorts().GetByNumber(i) );
		Proc::Start();
	}

	void Stop()
	{
		Proc::Stop();
	}
	void DoProc()
	{
		for(int i=0;i<GetInPorts().Size();i++)
		{
			memcpy( &(mWrappersList[i]->GetData()), mInLocationsList[i], GetInPorts().GetByNumber(i).GetSize()*sizeof(CLAM::TData) );
			mWrappersList[i]->Produce();
		}

		std::vector<CLAM::TData*> mDataList;
		for(int i=0;i<GetOutPorts().Size();i++)
		{
			CLAM::AudioOutPort & out = (CLAM::AudioOutPort&)(GetOutPorts().GetByNumber(i));
			mDataList.push_back( &(out.GetData()));
		}

		Do();	
		for(int i=0;i<GetOutPorts().Size();i++)
			memcpy(mLocationsList[i], mDataList[i], GetOutPorts().GetByNumber(i).GetSize()*sizeof(CLAM::TData) );		
	}
	const char* GetClassName()
	{
		return Proc::GetClassName();
	}

	const char * GetInControlName(int id)
	{
		return GetInControls().GetByNumber(id).GetName().c_str();
	}
	
	const char * GetOutControlName(int id)
	{
		return GetOutControls().GetByNumber(id).GetName().c_str();
	}
	
	const char * GetInPortName(int id)
	{
		return GetInPorts().GetByNumber(id).GetName().c_str();
	}
	
	const char * GetOutPortName(int id)
	{
		return GetOutPorts().GetByNumber(id).GetName().c_str();
	}


	int GetInControlsSize()
	{
		return GetInControls().Size();
	}
	
	int GetOutControlsSize()
	{
		return GetOutControls().Size();
	}
	
	int GetInPortsSize()
	{
		return GetInPorts().Size();
	}
	
	int GetOutPortsSize()
	{
		return GetOutPorts().Size();
	}
};

template<class Proc>
int ProcessingClass2LadspaTmpl<Proc>::GetInControlsIndex()
{
	return 0;
}

template<class Proc>
int ProcessingClass2LadspaTmpl<Proc>::GetOutControlsIndex()
{
	return GetInControlsSize();
}

template<class Proc>
int ProcessingClass2LadspaTmpl<Proc>::GetInPortsIndex()
{
	return GetOutControlsIndex() + GetOutControlsSize();
}

template<class Proc>
int ProcessingClass2LadspaTmpl<Proc>::GetOutPortsIndex()
{
	return GetInPortsIndex() + GetInPortsSize();
}

template<class Proc>
void ProcessingClass2LadspaTmpl<Proc>::DoControls()
{
	for(int i=0;i<GetInControlsSize();i++)
		GetInControls().GetByNumber(i).DoControl((CLAM::TData)*mInControlsList[i]);
}	

template<class Proc>
bool ProcessingClass2LadspaTmpl<Proc>::DoSizeCheck(int size)
{		
	for(int i=0;i<GetInPorts().Size();i++)
	{
		if(GetInPorts().GetByNumber(i).GetSize() != size )
		{
			GetInPorts().GetByNumber(i).SetSize( size );
			mWrappersList[i]->SetSize( size );
		}
	}
		
	for(int i=0;i<GetOutPorts().Size();i++)
	{
		if(GetOutPorts().GetByNumber(i).GetSize() != size )
			GetOutPorts().GetByNumber(i).SetSize( size );
	}
}

#endif // __ProcessingClass2Ladspa_hxx__

