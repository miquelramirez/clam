/*
 * Copyright (c) 2001-2002 MUSIC TECHNOLOGY GROUP (MTG)
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

#include "MIDILinker.hxx"
#include <cstdlib>
#include <iostream>
#include <string>
#include <algorithm>
#include "XMLAdapter.hxx"
#include "TopLevelProcessing.hxx"

using namespace CLAM;

using std::cout;
using std::endl;
using std::for_each;
using std::find;

void MIDIRelation::StoreOn(Storage & store)
{

	XMLAdapter<std::string> midi(mNameMIDI, "MIDI_Control");
	store.Store(&midi);

	XMLAdapter<std::string> mapper(mNameMapper, "Mapper");
	store.Store(&mapper);

	XMLAdapter<std::string> po(mNamePO, "Processing");
	store.Store(&po);

	XMLAdapter<unsigned> uintAdapter(mNumControl, "Control_affected");
	store.Store(&uintAdapter);
}



void MIDIRelation::LoadFrom(Storage & store)
{

	XMLAdapter<std::string> midi(mNameMIDI, "MIDI_Control");
	store.Load(&midi);

	XMLAdapter<std::string> mapper(mNameMapper, "Mapper");
	store.Load(&mapper);

	XMLAdapter<std::string> po(mNamePO, "Processing");
	store.Load(&po);

	XMLAdapter<unsigned> uintAdapter(mNumControl, "Control_affected");
	store.Load(&uintAdapter);
}

bool MIDIRelation::operator== (MIDIRelation  r)
{
	if (r.mNumControl != this->mNumControl)
		return false;
	if (r.mNameMIDI != this->mNameMIDI)
		return false;
	if (r.mNameMapper != this->mNameMapper)
		return false;
	if (r.mNamePO != this->mNamePO)
		return false;

	return true;
}

void MIDILinkerConfig::DefaultInit (void)
{
	AddName();
	AddMIDICfgs();
	AddMapperCfgs();
	AddNumBytes();
	AddRelations();
	UpdateData();
}

MIDILinker::MIDILinker()
{
	Configure(MIDILinkerConfig());
}

MIDILinker::MIDILinker(const MIDILinkerConfig& cfg)
{
	Configure(cfg);
}

bool MIDILinker::ConcreteConfigure(const ProcessingConfig& c)
{
	CopyAsConcreteConfig(mConfig, c);
	
	std::vector<unsigned>::iterator beginBytes;
	std::vector<MIDIRelation>::iterator beginRel, endRel;

	char* name;
	ControlMapperConfig mapperCfg;
	MIDIInConfig midiCfg;
	MIDIRelation rel;
	
	beginBytes = mConfig.GetNumBytes().begin();
	beginRel = mConfig.GetRelations().begin();
	endRel = mConfig.GetRelations().end();

	while (beginRel != endRel)
	{
		rel = *beginRel;
		string po  = rel.mNamePO;
		string mapper = rel.mNameMapper;
		string midi = rel.mNameMIDI;
		int num = rel.mNumControl;
		name = (char*)po.c_str();

		mapperCfg = GetMapper(mapper);
		midiCfg = GetMIDI(midi);

		LinkMIDIWithControl (midiCfg, name, num, mapperCfg,*beginBytes);
		beginBytes++;
		beginRel++;
	}
	return true;
}

MIDIInConfig MIDILinker::CreateMIDICfg(const unsigned short channel, const MIDI::Message message, const string  name,TData filter)
{
	MIDIInConfig cfg;
	
	cfg.SetDevice("default:default");
	cfg.SetChannelMask(MIDI::ChannelMask(channel));

	if (filter != -1)
	{
		cfg.SetFilter(filter);
	}
	cfg.SetMessageMask(MIDI::MessageMask(message));
	cfg.SetName(name);
	return cfg;	
}

ControlMapperConfig MIDILinker::CreateMapperConfig(const string name,const string mapping, const DataArray Arguments)
{
	ControlMapperConfig cfg ;
	cfg.SetName(name);
	cfg.SetMapping(mapping);
	cfg.SetArguments(Arguments);
	return cfg;
}

ControlMapperConfig MIDILinker::GetMapper(string mapName)
{
	ControlMapperConfig conf;
	std::vector<ControlMapperConfig>::iterator begin,end;
	begin = mConfig.GetMapperCfgs().begin();
	end = mConfig.GetMapperCfgs().end();

	while (begin != end)
	{
		conf = *begin;
		if (conf.GetName() == mapName)
		{
			return conf;
		}		
		begin++;
	}
	return conf;
}

MIDIInConfig MIDILinker::GetMIDI(string midiName)
{
	MIDIInConfig conf;
	std::vector<MIDIInConfig>::iterator begin,end;
	begin = mConfig.GetMIDICfgs().begin();
	end = mConfig.GetMIDICfgs().end();

	while (begin != end)
	{
		conf = *begin;
		if (conf.GetName() == midiName)
		{
			return conf;
		}	
		begin++;
	}
	return conf;
}


Processing* MIDILinker::GetProcessingObject(const char* procName)
{
	TopLevelProcessing& top = TopLevelProcessing::GetInstance();
	iterator begin, end;
	Processing* proc = NULL;
	begin = top.composite_begin();
	end = top.composite_end();

	while (begin != end)
	{
		proc = *begin;
		const char* name = proc->GetName().c_str();
		if (strcmp(procName,name) == 0)
		{
			return proc;
		}
		else
		{
			begin++;
		}
	}
	return proc;	
}

MIDIInControl* MIDILinker::CreateMIDICtrl(const MIDIInConfig &cfg)
{
	MIDIInControl* midi,* aux = NULL;
	std::vector<MIDIInControl*>::iterator begin,end;
	begin = mMIDICtrl.begin();
	end = mMIDICtrl.end();

	while (begin != end)
	{
		midi = *begin;
		if (midi->GetName() == cfg.GetName())
		{
			break; //sortim del while
		}		
		begin++;
	}
	if (begin == end) //no hem trobat res
	{
		aux = new MIDIInControl(cfg);
		mConfig.GetMIDICfgs().push_back(cfg);
		mMIDICtrl.push_back(aux);
		midi = NULL;
		return aux;
	}
	else
	{
		cout << "ha entrat al else" << endl;
		return midi;
	}
}

ControlMapper* MIDILinker::CreateMapper(const ControlMapperConfig &cfg)
{
	ControlMapper* midi,* aux=NULL;
	std::vector<ControlMapper*>::iterator begin,end;
	begin = mMIDIMapper.begin();
	end = mMIDIMapper.end();

	while (begin != end)
	{
		midi = *begin;
		if (midi->GetName() == cfg.GetName())
		{
			break; //sortim del while
		}	
		begin++;	
	}
	if (begin == end) //no hem trobat res
	{
		aux = new ControlMapper(cfg);
		mConfig.GetMapperCfgs().push_back(cfg);
		mMIDIMapper.push_back(aux);
		return aux;
	}
	else
	{
		return midi;
	}
}

void MIDILinker::LinkMIDIWithControl(
	const MIDIInConfig& MIDICfg, 
	const char*  procName, 
	const unsigned CtrlId, 
	const ControlMapperConfig& mapperCfg, 
	const int numByte)
{
	Processing* po = GetProcessingObject(procName);
	
	MIDIInControl* MIDICtrl;//= new MIDIInControl(MIDICfg);
	ControlMapper* mapper;//= new ControlMapper(mapperCfg);

	MIDICtrl = CreateMIDICtrl(MIDICfg);
	mapper = CreateMapper(mapperCfg);
	
	MIDICtrl->LinkOutWithInControl(numByte,mapper,0);
	mapper->LinkOutWithInControl(0, po, CtrlId);


		
	std::vector<MIDIRelation>::iterator iter;

	MIDIRelation mrel;
	mrel.mNameMIDI = MIDICfg.GetName();
	mrel.mNameMapper = mapperCfg.GetName();
	mrel.mNamePO = procName;
	mrel.mNumControl = CtrlId;


	iter = find(mConfig.GetRelations().begin(), mConfig.GetRelations().end(),mrel);
	
	if (iter == mConfig.GetRelations().end())
	{
		
		std::string nom  (procName);
		mConfig.GetNumBytes().push_back(numByte);
		mConfig.GetRelations().push_back(mrel);

	}
}

bool MIDILinker::Do()
{
	mMIDIManager.Check();
	return true;
}

MIDILinker::~MIDILinker()
{
	std::for_each(mMIDICtrl.begin(), mMIDICtrl.end(),Eraser());
	std::for_each(mMIDIMapper.begin(), mMIDIMapper.end(),Eraser());
}

template <class Control>class PrintControl {
public:
	void operator() (const Control* c) {
		std::cout << c->GetName() << std::endl;
	}
};

void Cmd_MIDILinker::cbShowProcessingControls(const char* procName)
{
	Processing* po = GetProcessingObject(procName);
	cout << "\nPrinting control names of processing :" << po->GetName() << endl;
	cout << "InControls:\n";
	for_each(po->FirstInControl(), po->LastInControl(), PrintControl<InControl>() );
}
