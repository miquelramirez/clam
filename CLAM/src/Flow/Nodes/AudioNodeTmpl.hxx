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


#if !defined _MSC_VER || _MSC_VER >= 1310 // MSVC++ 6 this class does not compile under Visual 6!
namespace CLAM {

	template<class BUFFER>
	class NodeTmpl<Audio,BUFFER> : public Node<Audio> {
		StreamBuffer<Audio,BUFFER> mStream;
	public:

		void SetPrototype(const Audio &proto);

		void SetSampleRate(TData sample_rate);

		void Configure(int max_window_size=0) { mStream.Configure(max_window_size); }

		WriteStreamRegion *NewWriter (
			OutPort *port, 
			unsigned int hop,
			unsigned int length
		);
	
		ReadStreamRegion  *NewReader (InPort *port, unsigned int hop,
					      unsigned int length, SourceStreamRegion* source = 0);
		
		void RemoveInPortConnection( InPort * port , ReadStreamRegion *  reader);

		void GetAndActivate(WriteStreamRegion* r, Audio &a);
		void GetAndActivate(ReadStreamRegion* r, Audio &a);
		void GetAndActivate(DelayStreamRegion* r, Audio &a);
		void GetAndActivate(InplaceStreamRegion* r, Audio &a);

		void LeaveAndAdvance(WriteStreamRegion *r);
		void LeaveAndAdvance(ReadStreamRegion *r);
		void LeaveAndAdvance(DelayStreamRegion *r);
		void LeaveAndAdvance(InplaceStreamRegion *r);

		bool CanActivateRegion(SourceStreamRegion &toActivate) {
			return mStream.CanActivateRegion( toActivate );
		}
		bool CanActivateRegion(ReadStreamRegion &toActivate) {
			return mStream.CanActivateRegion( toActivate );
		}
	};

	template<class BUFFER>
	WriteStreamRegion *NodeTmpl<Audio,BUFFER>::NewWriter (
		OutPort *port,
		unsigned int hop,
		unsigned int length
	)
	{
		Node<Audio>::mpDriver = port;
		return mStream.NewWriter(hop,length);
	}
	
	
	template<class BUFFER>
	ReadStreamRegion *NodeTmpl<Audio,BUFFER>::NewReader (
		InPort *port, 
		unsigned int hop,
		unsigned int length,
		SourceStreamRegion* source 
	)
	{
		Node<Audio>::mInputs.push_back(port);
		return mStream.NewReader(hop,length,source);
	}

	template<class BUFFER>
	void NodeTmpl<Audio,BUFFER>::RemoveInPortConnection(
		InPort * port , ReadStreamRegion *  reader)
	{
		CLAM_ASSERT( port->GetNode() == this, "NodeTmpl::RemoveInPort() "
			     "InPort to remove connection is not attached to the node" );
		Node<Audio>::mInputs.remove(port);
		mStream.RemoveReader( reader );
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

} // end of namespace CLAM
#else // MSVC 6.0 Hack

	#include "CircularStreamImpl.hxx"

namespace CLAM {

	class AudioNodeTmpl : public NodeTmpl< Audio, CircularStreamImpl<TData> >
	{
		StreamBuffer<Audio,CircularStreamImpl<TData> > mStream;
	public:

		void SetPrototype(const Audio &proto);

		void SetSampleRate(TData sample_rate);

		void Configure(int max_window_size=0) { mStream.Configure(max_window_size); }

		WriteStreamRegion *NewWriter (
			OutPort *port, 
			unsigned int hop,
			unsigned int length
		);
	
		ReadStreamRegion  *NewReader (InPort *port, unsigned int hop,
					      unsigned int length, SourceStreamRegion* source = 0);

		void GetAndActivate(WriteStreamRegion* r, Audio &a);
		void GetAndActivate(ReadStreamRegion* r, Audio &a);
		void GetAndActivate(DelayStreamRegion* r, Audio &a);
		void GetAndActivate(InplaceStreamRegion* r, Audio &a);

		void LeaveAndAdvance(WriteStreamRegion *r);
		void LeaveAndAdvance(ReadStreamRegion *r);
		void LeaveAndAdvance(DelayStreamRegion *r);
		void LeaveAndAdvance(InplaceStreamRegion *r);
	};

	WriteStreamRegion *AudioNodeTmpl::NewWriter (
		OutPort *port,
		unsigned int hop,
		unsigned int length
	)
	{
		Node<Audio>::mpDriver = port;
		return mStream.NewWriter(hop,length);
	}
	
	
	ReadStreamRegion *AudioNodeTmpl::NewReader (
		InPort *port, 
		unsigned int hop,
		unsigned int length,
		SourceStreamRegion* source 
	)
	{
		Node<Audio>::mInputs.push_back(port);
		return mStream.NewReader(hop,length,source);
	}


	void AudioNodeTmpl::GetAndActivate(WriteStreamRegion* r, Audio &a)
	{
		mStream.GetAndActivate(r,a);
	}

	void AudioNodeTmpl::GetAndActivate(ReadStreamRegion* r, Audio &a)
	{
		mStream.GetAndActivate(r,a);
	}

	void AudioNodeTmpl::GetAndActivate(DelayStreamRegion* r, Audio &a)
	{
		mStream.GetAndActivate(r,a);
	}

	void AudioNodeTmpl::GetAndActivate(InplaceStreamRegion* r, Audio &a)
	{
		mStream.GetAndActivate(r,a);
	}

	void AudioNodeTmpl::SetPrototype(const Audio &a)
	{
		mStream.SetPrototype(a);
	}

	void AudioNodeTmpl::SetSampleRate(TData sr)
	{
		mStream.SetSampleRate(sr);
	}

	void AudioNodeTmpl::LeaveAndAdvance(WriteStreamRegion *r)
	{
		mStream.LeaveAndAdvance(r);
	}

	void AudioNodeTmpl::LeaveAndAdvance(ReadStreamRegion *r)
	{
		mStream.LeaveAndAdvance(r);
	}

	void AudioNodeTmpl::LeaveAndAdvance(DelayStreamRegion *r)
	{
		mStream.LeaveAndAdvance(r);
	}

	void AudioNodeTmpl::LeaveAndAdvance(InplaceStreamRegion *r)
	{
		mStream.LeaveAndAdvance(r);
	}


} // end of namespace CLAM

#endif // end of MSVC 6.0 check


#endif
