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

#include "Assert.hxx"
#include "CLAM_Math.hxx"

#ifndef _STREAM_REGION_H
#define _STREAM_REGION_H


namespace CLAM {

	class RegionVisitor;

	class StreamRegion {
	protected:
		/** Number of samples to advance the region in each "advance" operation */
		unsigned int mHop;
		/** Start position in number of elements of the beginning of the region */
		unsigned int mPos;
		/** Lenght of the region in number of elements */
		unsigned int mLen;
		/** End of the region. If region is not active it is equal to mPos */
		unsigned int mEnd;
		/** Offset in number of elements used to compute "virtual" center of the region.
		 *	A region centered in zero is not actually centered around sample zero but around
		 *	this value. */
		unsigned int mOffset;
	public:
		StreamRegion(unsigned int hop,
		             unsigned int length);

		/** Accessor to region's hop size in number of elements. @see mHop */
		unsigned int Hop() const {return mHop;}
		/** Accessor to region's starting position in number of elements. @see mPos */
		unsigned int Pos() const {return mPos;}
		/** Accessor to region's ending position in number of elements. @see mEnd */
		unsigned int End() const {return mEnd;}
		
		unsigned int Len() const {return mLen;}
		unsigned int ActiveLen() const {return mEnd-mPos;}
				
		/** Accessor to "virtual" center of region. @see mOffset*/
		unsigned int Center() const {return Chop((mPos+mEnd)*0.5f)-mOffset;} 
		

		/** \todo
		 */
		virtual bool CanActivate() const = 0;

		/** Sets the condition of the region being active, which is actually setting mEnd
		 *	to mPos+mLen. */
		void Activate();
		/** Moves the starting position the number of elements specified by mHop. @see mHop. */
		void LeaveAndAdvance();

		/** Removes the activation condition without actually moving it*/
		void Leave();

		/** Set center of the region to specified value */
		void SetCenter(unsigned int center){
			mPos=center - Chop(mLen*0.5f) + mOffset;
		}
		/** Sets center of the region to the "virtual" zero. @see Center, SetCenter */
		void CenterToZero() {SetCenter(0);}
		/** Initializes offset member variable and centers region to zero. This method is
		 *	overriden by some derived classes. @see CenterToZero*/
		virtual void Init(unsigned int offset) {
			mOffset=offset;
			CenterToZero();}

		/** Returns true if this region preceeds the region passed as argument */
		bool PreceedsWithNoOverlap(const StreamRegion*) const;
		
		virtual bool FulfilsInvariant() const;
		virtual bool Accept(RegionVisitor& v) const = 0;
	};

	class ReadStreamRegion;
	class SourceStreamRegion;
	class WriteStreamRegion;
	class DelayStreamRegion;
	class InplaceStreamRegion;

	class RegionVisitor {
	public:
		virtual bool Visit(const ReadStreamRegion&)    = 0;
		virtual bool Visit(const SourceStreamRegion&)  = 0;
		virtual bool Visit(const WriteStreamRegion&)   = 0;
		virtual bool Visit(const DelayStreamRegion&)   = 0;
		virtual bool Visit(const InplaceStreamRegion&) = 0;
	};

	class RegionCheck : public RegionVisitor
	{
	public:
		virtual bool Visit(const ReadStreamRegion&);
		virtual bool Visit(const SourceStreamRegion&);
		virtual bool Visit(const WriteStreamRegion&);
		virtual bool Visit(const DelayStreamRegion&);
		virtual bool Visit(const InplaceStreamRegion&);
	};



};


#endif
