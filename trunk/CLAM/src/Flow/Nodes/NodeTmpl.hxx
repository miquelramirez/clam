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
#define _NODE_TMPL_H

#include "Node.hxx"
#include "StreamBuffer.hxx"

namespace CLAM {

	template<class DATA, class BUFFER>
	class NodeTmpl : public Node<DATA> {
		StreamBuffer<DATA,BUFFER> mStream;
	public:
		virtual ~NodeTmpl() {}

		void SetPrototype(const DATA &proto);

		void Configure(int max_window_size=0) { mStream.Configure(max_window_size); }

		WriteStreamRegion *NewWriter (OutPort *port,
					      unsigned int hop,
					      unsigned int length);
	
		ReadStreamRegion  *NewReader (InPort *port,
					      unsigned int hop,
					      unsigned int length,
					      SourceStreamRegion* source = 0);

		void RemoveInPortConnection( InPort * port , ReadStreamRegion *  reader);	
		void GetAndActivate(WriteStreamRegion* r, Array<DATA> &a);
		void GetAndActivate(ReadStreamRegion* r, Array<DATA> &a);
		void GetAndActivate(DelayStreamRegion* r, Array<DATA> &a);
		void GetAndActivate(InplaceStreamRegion* r, Array<DATA> &a);

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

	template<class DATA, class BUFFER>
	void NodeTmpl<DATA,BUFFER>::SetPrototype(const DATA &d)
	{
		mStream.SetPrototype(d);
	}


	template<class DATA, class BUFFER>
	WriteStreamRegion *NodeTmpl<DATA,BUFFER>::NewWriter (OutPort *port, unsigned int hop,
							     unsigned int length)
	{
	
		Node<DATA>::mpDriver = port;
		return mStream.NewWriter(hop,length);
	}
	
	
	template<class DATA, class BUFFER>
	ReadStreamRegion *NodeTmpl<DATA,BUFFER>::NewReader (InPort *port, unsigned int hop,
							    unsigned int length,
							    SourceStreamRegion* source)
	{
		Node<DATA>::mInputs.push_back(port);
		return mStream.NewReader(hop,length,source);
	}

	template<class DATA, class BUFFER>
	void NodeTmpl<DATA,BUFFER>::RemoveInPortConnection(
		InPort * port , ReadStreamRegion *  reader)
	{
		CLAM_ASSERT( port->GetNode() == this, "NodeTmpl::RemoveInPort() "
			     "InPort to remove connection is not attached to the node" );
		Node<DATA>::mInputs.remove(port);
		mStream.RemoveReader( reader );
	}

	template<class DATA, class BUFFER>
	void NodeTmpl<DATA,BUFFER>::GetAndActivate(WriteStreamRegion* r, Array<DATA> &a)
	{
		mStream.GetAndActivate(r,a);
	}

	template<class DATA, class BUFFER>
	void NodeTmpl<DATA,BUFFER>::GetAndActivate(ReadStreamRegion* r, Array<DATA> &a)
	{
		mStream.GetAndActivate(r,a);
	}

	template<class DATA, class BUFFER>
	void NodeTmpl<DATA,BUFFER>::GetAndActivate(DelayStreamRegion* r, Array<DATA> &a)
	{
		mStream.GetAndActivate(r,a);
	}

	template<class DATA, class BUFFER>
	void NodeTmpl<DATA,BUFFER>::GetAndActivate(InplaceStreamRegion* r, Array<DATA> &a)
	{
		mStream.GetAndActivate(r,a);
	}

	template<class DATA, class BUFFER>
	void NodeTmpl<DATA,BUFFER>::LeaveAndAdvance(WriteStreamRegion *r)
	{
		mStream.LeaveAndAdvance(r);
	}

	template<class DATA, class BUFFER>
	void NodeTmpl<DATA,BUFFER>::LeaveAndAdvance(ReadStreamRegion *r)
	{
		mStream.LeaveAndAdvance(r);
	}

	template<class DATA, class BUFFER>
	void NodeTmpl<DATA,BUFFER>::LeaveAndAdvance(DelayStreamRegion *r)
	{
		mStream.LeaveAndAdvance(r);
	}

	template<class DATA, class BUFFER>
	void NodeTmpl<DATA,BUFFER>::LeaveAndAdvance(InplaceStreamRegion *r)
	{
		mStream.LeaveAndAdvance(r);
	}

}

// Class speciallizations:

#include "AudioNodeTmpl.hxx"

#endif
