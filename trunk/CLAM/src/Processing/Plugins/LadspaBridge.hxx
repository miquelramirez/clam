
#ifndef __LadspaBrige_hxx__
#define __LadspaBridge_hxx__

#include <ladspa.h>
#include <vector>

#include "Audio.hxx"

class LadspaBridge
{
public:
	std::vector<LADSPA_Data*> mInControlsList;
	std::vector<LADSPA_Data*> mOutControlsList;
	std::vector<CLAM::Audio> mInAudioList;
	std::vector<CLAM::Audio> mOutAudioList;
	int mId;

public:

	virtual void Start() = 0;
	virtual void Stop() = 0;
	virtual void DoProc() = 0;
	
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

	LadspaBridge(int id)
	{
		mId = id;
	}
	int GetId()
	{
		return mId;
	}
};

template<class Proc>
class LadspaBridgeTmpl : public LadspaBridge, public Proc 
{
public:
	static LadspaBridge* Create(int id) { return new LadspaBridgeTmpl<Proc>(id); }
	int GetInControlsIndex();
	int GetOutControlsIndex();
	int GetInPortsIndex();
	int GetOutPortsIndex();
	void DoControls();
	bool DoSizeCheck(int size);
	
	LadspaBridgeTmpl(int id)
		: LadspaBridge(id)
	{
	}
	
	void Start()
	{
		for(int i=0;i<GetInPorts().Size();i++)
			GetInPorts().GetByNumber(i).Attach(mInAudioList[i]);

		for(int i=0;i<GetOutPorts().Size();i++)
			GetOutPorts().GetByNumber(i).Attach(mOutAudioList[i]);
	
		Proc::Start();
	}
	void Stop()
	{
		Proc::Stop();
	}
	void DoProc()
	{
		if(CanDo())
			Do();
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

	bool CanDo()
	{
		for(int i=0;i<GetInPorts().Size();i++)
			if(!GetInPorts().GetByNumber(i).IsAttached())
				return false;	

		for(int i=0;i<GetOutPorts().Size();i++)
			if(!GetOutPorts().GetByNumber(i).IsAttached())
				return false;	
		return true;
	}
};

template<class Proc>
int LadspaBridgeTmpl<Proc>::GetInControlsIndex()
{
	return 0;
}

template<class Proc>
int LadspaBridgeTmpl<Proc>::GetOutControlsIndex()
{
	return GetInControlsSize();
}

template<class Proc>
int LadspaBridgeTmpl<Proc>::GetInPortsIndex()
{
	return GetOutControlsIndex() + GetOutControlsSize();
}

template<class Proc>
int LadspaBridgeTmpl<Proc>::GetOutPortsIndex()
{
	return GetInPortsIndex() + GetInPortsSize();
}

template<class Proc>
void LadspaBridgeTmpl<Proc>::DoControls()
{
	for(int i=0;i<GetInControlsSize();i++)
		GetInControls().GetByNumber(i).DoControl((CLAM::TData)*mInControlsList[i]);
}	

template<class Proc>
bool LadspaBridgeTmpl<Proc>::DoSizeCheck(int size)
{		
	for(int i=0;i<GetInPorts().Size();i++)
		if(mInAudioList[i].GetBuffer().Size() != size )
			mInAudioList[i].GetBuffer().SetSize(size);

	for(int i=0;i<GetOutPorts().Size();i++)
		if(mOutAudioList[i].GetBuffer().Size() != size )
			mOutAudioList[i].GetBuffer().SetSize(size);
}

#endif // __LadspaBridge_hxx__

