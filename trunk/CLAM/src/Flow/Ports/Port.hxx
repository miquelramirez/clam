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

#ifndef _Port_hxx_
#define _Port_hxx_

#include "Assert.hxx"
#include <string>

namespace CLAM {

	class NodeBase;
	class Processing;
	class ProcessingData;
	class Spectrum;
	class Audio;
	class Envelope;
	class Peak;



	class DataVisitor
	{
		void ErrMsg(std::string&,const std::string&);
	public:
		virtual const std::string& ClassName() =0;
		virtual void Visit(ProcessingData& data);
		virtual void Visit(Spectrum& data);
		virtual void Visit(Audio& data);
		virtual void Visit(Envelope& data);
		virtual void Visit(Peak& data);
	};

	/** Processing port base class */

	class Processing;

	class Port
	{
		/** Port name */
		std::string mName;
		/** Processing to which this port belongs */
		Processing *mParent;
		/** Number of data objects used in each Do */
		unsigned int mLength;
		/** Number of data objects discarded in each Do */
		unsigned int mHop;
	public:
		Port(const std::string &n, Processing *o, int length, int hop = 0) : 
			mName(n),
			mParent(o),
			mLength(length),
			mHop(hop?hop:length)
		{};

		virtual ~Port() {}

		virtual void Attach( NodeBase& )= 0;
		virtual void Attach(ProcessingData& data)= 0;
		virtual bool IsAttached() = 0;
		virtual void Unattach() = 0;		
		virtual ProcessingData* GetProcessingData() = 0;
		virtual NodeBase* GetNode() = 0;

		virtual void Accept(DataVisitor&) = 0;
		unsigned int Length() const { return mLength; }
		unsigned int Hop() const { return mHop; }
		void SetParams(unsigned int length,
		               unsigned int hop = 0)
		{
			//CLAM_ASSERT( !IsAttached(), "Port::SetParams(int), SetParams must be called before Attach port" );

			// PA: todo: allow attach before a SetParams. For this we need to finish the Nodes refactoring
			// by now I comment this assert because it doesn't allows SpectralDelay to work.
			// a cludge (which is not rapid) would be to add a new virtual IsAttachedToNode() which would
			// be a better assert.
			mLength = length;
			mHop = hop?hop:length;
		}
		const std::string& GetName() const { return mName; }
		Processing * GetProcessing() const { return mParent;}
	};

}




#endif // _PORT_H
