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

#ifndef __MIDILINKER__
#define __MIDILINKER__

#include "Processing.hxx"
#include "MIDIInControl.hxx"
#include "ControlMapper.hxx"
#include "MIDIManager.hxx"

#include <algorithm>

using std::for_each;
using std::string;


namespace CLAM
{

	class MIDIRelation : public Component
	{
	public:
		MIDIRelation(){}
		~MIDIRelation(){}
		void StoreOn (Storage & store);
		void LoadFrom (Storage & store);
		bool operator== (MIDIRelation  r);

		const char* GetClassName() const
		{
			return "MIDIRelation";
		}

//	private:

		string mNameMIDI;
		string mNameMapper;
		string mNamePO;
		unsigned mNumControl;	       
	};
		


	class MIDILinkerConfig : public ProcessingConfig
	{
	public:
		DYNAMIC_TYPE_USING_INTERFACE (MIDILinkerConfig, 5, ProcessingConfig);
		
		DYN_ATTRIBUTE (0,public, std::string, Name);
		DYN_CONTAINER_ATTRIBUTE (1,public, std::vector<MIDIInConfig>, MIDICfgs,midicfgs);
		DYN_CONTAINER_ATTRIBUTE (2,public, std::vector<ControlMapperConfig>, MapperCfgs,mappercfgs);
		DYN_CONTAINER_ATTRIBUTE (3,public, std::vector<unsigned>, NumBytes,numbytes);
//proves

		DYN_CONTAINER_ATTRIBUTE (4,public, std::vector<MIDIRelation>, Relations, relations);
		
	protected:
		void DefaultInit(void);
	};
	
	class Eraser
	{
		public:
		template <typename PtrToDelete>
		void operator()(PtrToDelete p)
		{
			delete p;
		}
	};

	class MIDILinker : public Processing
	{ 
	public:
		MIDILinker();
		MIDILinker(const MIDILinkerConfig &cfg);
		virtual ~MIDILinker();
		const char * GetClassName() const {return "MIDILinker";}

		virtual bool Do(void);
		virtual const ProcessingConfig &GetConfig() const { return mConfig; }
		
		virtual ControlMapperConfig CreateMapperConfig
		(const string name,const string mapping, const DataArray Arguments);
		
		MIDIInConfig CreateMIDICfg
		(const unsigned short channel, const MIDI::Message message, 
		 const string name,const TData filter=-1);

		void MIDILinker::LinkMIDIWithControl
		(const MIDIInConfig &MIDICfg, const char* procName,  
		 const unsigned CtrlId,const ControlMapperConfig &mapperCfg, 
		 const int numByte=0);
		
	private:
		virtual bool ConcreteConfigure(const ProcessingConfig& c);
		ControlMapperConfig GetMapper(string mapName);
		MIDIInConfig GetMIDI(string midiName);
		MIDIInControl* CreateMIDICtrl(const MIDIInConfig &cfg);
		ControlMapper* CreateMapper(const ControlMapperConfig &cfg);
		
		std::vector< MIDIInControl* > mMIDICtrl;
		std::vector< ControlMapper* > mMIDIMapper;
		MIDIManager mMIDIManager;
	public:
		MIDILinkerConfig   mConfig;
		
	protected:
		Processing* GetProcessingObject(const char* procName);
	};
	
	class Cmd_MIDILinker : public MIDILinker
	{
	public:
		Cmd_MIDILinker(){}
		Cmd_MIDILinker(const MIDILinkerConfig &cfg)
			:MIDILinker(cfg){}
		virtual ~Cmd_MIDILinker(){}
		void cbShowProcessingControls(const char* procName);
		template <class Control>class PrintControlName;
	};
}

#endif
