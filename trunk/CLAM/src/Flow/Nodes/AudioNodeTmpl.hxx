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

#ifndef _NODE_TMPL_H
#error DO NOT include this file directly. It is included from StreamBuffer.hxx
#endif

#ifndef _AUDIO_NODE_TMPL_H
#define _AUDIO_NODE_TMPL_H

#include "Audio.hxx"

namespace CLAM {

	template<class BUFFER>
	class NodeTmpl<Audio,BUFFER> : public Node<Audio> {
		StreamBuffer<Audio,BUFFER> mStream;
	public:

		void SetPrototype(const Audio &proto);

		void SetSampleRate(TData sample_rate);

		void Configure(int max_window_size=0) { mStream.Configure(max_window_size); }

		WriteStreamRegion *NewWriter (OutPort *port,
									  unsigned int hop,
									  unsigned int length);
	
		ReadStreamRegion  *NewReader (InPort *port,
									  unsigned int hop,
									  unsigned int length,
									  SourceStreamRegion* source = 0);

		void GetAndActivate(WriteStreamRegion* r, Audio &a);
		void GetAndActivate(ReadStreamRegion* r, Audio &a);
		void GetAndActivate(DelayStreamRegion* r, Audio &a);
		void GetAndActivate(InplaceStreamRegion* r, Audio &a);

		void LeaveAndAdvance(WriteStreamRegion *r);
		void LeaveAndAdvance(ReadStreamRegion *r);
		void LeaveAndAdvance(DelayStreamRegion *r);
		void LeaveAndAdvance(InplaceStreamRegion *r);
	};

	template<class BUFFER>
	WriteStreamRegion *NodeTmpl<Audio,BUFFER>::NewWriter (OutPort *port,
														 unsigned int hop,
														 unsigned int length)
	{
		mpDriver = port;
		return mStream.NewWriter(hop,length);
	}
	
	
	template<class BUFFER>
	ReadStreamRegion *NodeTmpl<Audio,BUFFER>::NewReader (InPort *port,
														unsigned int hop,
														unsigned int length,
														SourceStreamRegion* source = 0)
	{
		mInputs.AddElem(port);
		return mStream.NewReader(hop,length,source);
	}


	template<class BUFFER>
	void NodeTmpl<Audio,BUFFER>::GetAndActivate(WriteStreamRegion* r, Audio &a)
	{
		mStream.GetAndActivate(r,a);
	}

	template<class BUFFER>
	void NodeTmpl<Audio,BUFFER>::GetAndActivate(ReadStreamRegion* r, Audio &a)
	{
		mStream.GetAndActivate(r,a);
	}

	template<class BUFFER>
	void NodeTmpl<Audio,BUFFER>::GetAndActivate(DelayStreamRegion* r, Audio &a)
	{
		mStream.GetAndActivate(r,a);
	}

	template<class BUFFER>
	void NodeTmpl<Audio,BUFFER>::GetAndActivate(InplaceStreamRegion* r, Audio &a)
	{
		mStream.GetAndActivate(r,a);
	}

	template<class BUFFER>
	void NodeTmpl<Audio,BUFFER>::SetPrototype(const Audio &a)
	{
		mStream.SetPrototype(a);
	}

	template<class BUFFER>
	void NodeTmpl<Audio,BUFFER>::SetSampleRate(TData sr)
	{
		mStream.SetSampleRate(sr);
	}

	template<class BUFFER>
	void NodeTmpl<Audio,BUFFER>::LeaveAndAdvance(WriteStreamRegion *r)
	{
		mStream.LeaveAndAdvance(r);
	}

	template<class BUFFER>
	void NodeTmpl<Audio,BUFFER>::LeaveAndAdvance(ReadStreamRegion *r)
	{
		mStream.LeaveAndAdvance(r);
	}

	template<class BUFFER>
	void NodeTmpl<Audio,BUFFER>::LeaveAndAdvance(DelayStreamRegion *r)
	{
		mStream.LeaveAndAdvance(r);
	}

	template<class BUFFER>
	void NodeTmpl<Audio,BUFFER>::LeaveAndAdvance(InplaceStreamRegion *r)
	{
		mStream.LeaveAndAdvance(r);
	}

}

#endif
