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

#ifndef _NODE_H
#define _NODE_H

#include "ReadStreamRegion.hxx"
#include "WriteStreamRegion.hxx"
#include "Audio.hxx"
#include <list>


namespace CLAM {

	// forward declarations
	class Port;
	class OutPort;
	class InPort;

	//-------------------------------------------
	class NodeBase {
	protected:
		OutPort *mpDriver;
		std::list<InPort*> mInputs;
	public:
		const OutPort * GetWriter() const;
		const std::list<InPort*> GetReaders() const;

		NodeBase() : mpDriver(0){}
		virtual ~NodeBase() {}		
		virtual void Configure(int max_window_size=0) = 0;

		virtual WriteStreamRegion *NewWriter (OutPort *port, 
						      unsigned int hop, 
						      unsigned int length) = 0;
		virtual void RemoveInPortConnection( InPort * port, 
						     ReadStreamRegion * reader) = 0;
		virtual void UnattachAll();
	
		virtual ReadStreamRegion  *NewReader (InPort *port,
						      unsigned int hop,
						      unsigned int length,
						      SourceStreamRegion* source = 0) = 0;

		virtual bool CanActivateRegion(SourceStreamRegion &toActivate)=0;
		virtual bool CanActivateRegion(ReadStreamRegion &toActivate)=0;
	};

	//-------------------------------------------
	

	template<class T>
	class Node : public NodeBase
	{

	public:
		virtual ~Node() {}

		virtual void Configure(int max_window_size=0) = 0;

		virtual void SetPrototype(const T &proto) = 0;

		virtual WriteStreamRegion *NewWriter (OutPort *port,
						      unsigned int hop,
						      unsigned int length) = 0;
		
		virtual ReadStreamRegion  *NewReader (InPort *port,
						      unsigned int hop,
						      unsigned int length,
						      SourceStreamRegion* source = 0) = 0;
		
		virtual void GetAndActivate(WriteStreamRegion* r, Array<T> &a) = 0;
		virtual void GetAndActivate(ReadStreamRegion* r, Array<T> &a) = 0;
		virtual void GetAndActivate(DelayStreamRegion* r, Array<T> &a) = 0;
		virtual void GetAndActivate(InplaceStreamRegion* r, Array<T> &a) = 0;

		virtual void LeaveAndAdvance(WriteStreamRegion *r) = 0;
		virtual void LeaveAndAdvance(ReadStreamRegion *r) = 0;
		virtual void LeaveAndAdvance(DelayStreamRegion *r) = 0;
		virtual void LeaveAndAdvance(InplaceStreamRegion *r) = 0;
	};


	template<>
	class Node<Audio> : public NodeBase
	{
	public:
		virtual ~Node() {}

		virtual void SetPrototype(const Audio &proto) = 0;

		virtual void GetAndActivate(WriteStreamRegion* r, Audio &a) = 0;
		virtual void GetAndActivate(ReadStreamRegion* r, Audio &a) = 0;
		virtual void GetAndActivate(DelayStreamRegion* r, Audio &a) = 0;
		virtual void GetAndActivate(InplaceStreamRegion* r, Audio &a) = 0;

		virtual void LeaveAndAdvance(WriteStreamRegion *r) = 0;
		virtual void LeaveAndAdvance(ReadStreamRegion *r) = 0;
		virtual void LeaveAndAdvance(DelayStreamRegion *r) = 0;
		virtual void LeaveAndAdvance(InplaceStreamRegion *r) = 0;
	};


}
#endif // _NODE_H
