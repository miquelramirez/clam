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

#ifndef _AUDIO_PORT_HXX
#define _AUDIO_PORT_HXX

#include "Audio.hxx"

namespace CLAM {

	template<>
	class InPortTmpl<Audio> : public InPort
	{
		ReadStreamRegion *mpRegion;
		Node<Audio> *mpNode;
		Audio *mpData;
		Audio mWrapper;
		ProcessingData &GetProcessingData() { return *mpData; }
	public:
		inline InPortTmpl(std::string n, Processing *o, int length, int hop = 0, bool inplace=false);
		inline Audio &GetData();
		inline void LeaveData();
		void Attach(ProcessingData& data);
		inline void Attach(Audio& data);
		inline void Attach(Node<Audio> &n);
		inline void Attach(InPortTmpl<Audio> &n);
		inline void Accept(DataVisitor&);
	};

	template<>
	class OutPortTmpl<Audio> : public OutPort
	{
		WriteStreamRegion *mpRegion;
		Node<Audio> *mpNode;
		Audio *mpData;
		Audio mWrapper;
		ProcessingData &GetProcessingData() { return *mpData; }
	public:
		inline OutPortTmpl(std::string n, Processing *o, int length, int hop = 0);
		inline Audio &GetData();
		inline void LeaveData();
		void Attach(ProcessingData& data);
		inline void Attach(Audio& data);
		inline void Attach(OutPortTmpl<Audio> &n);
		inline void Attach(Node<Audio> &n);
		inline void Accept(DataVisitor&);
	};


	// Template method implementations


	InPortTmpl<Audio>::InPortTmpl(std::string n,
								  Processing *o,
								  int length,
								  int hop,
								  bool inplace)
		: InPort(n,o,length,hop,inplace),
		  mpRegion(0),
		  mpNode(0)
	{
		o->PublishInPort(this);
	}

	Audio &InPortTmpl<Audio>::GetData()	
	{ 
		CLAM_ASSERT(mpData || (mpNode && mpRegion),
					"InPortTmpl::GetData(): No data atached to the port.");
		if (mpNode) {
			mpNode->GetAndActivate(mpRegion,mWrapper);
			mpData = &mWrapper;
		}
		return *mpData;
	}

	void InPortTmpl<Audio>::LeaveData()	
	{
		if (mpNode)
			mpNode->LeaveAndAdvance(mpRegion);
	}

	inline void InPortTmpl<Audio>::Attach(ProcessingData& data)
	{
		try{
			Attach(dynamic_cast<Audio&> (data));
		}
		catch (std::bad_cast){
			CLAM_ASSERT(false,"You are trying to attach a processing data that is not an Audio to an Audio port");
		}
	}
	
	void InPortTmpl<Audio>::Attach(Audio& data)
	{
		mpNode = 0;
		mpData = &data;
	}	

	void InPortTmpl<Audio>::Attach(Node<Audio>& node)
	{
		mpData = 0;
		mpNode = &node;
		mpRegion = node.NewReader(this, Hop(), Length());
	}	

	void InPortTmpl<Audio>::Attach(InPortTmpl<Audio>& port)
	{
		if (port.mpNode)
			Attach(*port.mpNode);
		else
			Attach(*port.mpData);
	}	

	void InPortTmpl<Audio>::Accept(DataVisitor& v)
	{
		v.Visit(*mpData);
	}

	OutPortTmpl<Audio>::OutPortTmpl(std::string n,
									Processing *o,
									int length,
									int hop)
		: OutPort(n,o,length,hop),
		  mpRegion(0),
		  mpNode(0)
	{
		o->PublishOutPort(this);
	}

	
	inline void OutPortTmpl<Audio>::Attach(ProcessingData& data)
	{
		try{
			Attach(dynamic_cast<Audio&> (data));
		}
		catch (std::bad_cast){
			CLAM_ASSERT(false,"You are trying to attach a processing data that is not an Audio to an Audio port");
		}
	}

	void OutPortTmpl<Audio>::Attach(Audio& data)
	{
		mpData = &data;
		mpNode = 0;
	}	

	void OutPortTmpl<Audio>::Attach(Node<Audio>& node)
	{
		mpData = 0;
		mpNode = &node;
		mpRegion = node.NewWriter(this, Hop(), Length());
	}	

	void OutPortTmpl<Audio>::Attach(OutPortTmpl<Audio>& port)
	{
		if (port.mpNode)
			Attach(*port.mpNode);
		else
			Attach(*port.mpData);
	}	

	Audio &OutPortTmpl<Audio>::GetData()	
	{ 
		CLAM_ASSERT(mpData || (mpNode && mpRegion),
					"OutPortTmpl::GetData(): No data atached to the port.");
		if (mpNode) {
			mpNode->GetAndActivate(mpRegion,mWrapper);
			mpData = &mWrapper;
		}
		return *mpData;
	}

	void OutPortTmpl<Audio>::LeaveData()	
	{
		if (mpNode)
			mpNode->LeaveAndAdvance(mpRegion);
	}

	void OutPortTmpl<Audio>::Accept(DataVisitor& v)
	{
		v.Visit(*mpData);
	}


}

#endif
