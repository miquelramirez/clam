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

#ifndef _SOURCE_STREAM_REGION_H
#define _SOURCE_STREAM_REGION_H

#include "StreamRegion.hxx"
#include <list>

namespace CLAM {

	class ReadStreamRegion;

	class SourceStreamRegion : public StreamRegion {

		std::list<ReadStreamRegion*> mReaders;

	protected:

		SourceStreamRegion(unsigned int hop,
		                   unsigned int length);
	public:

		typedef std::list<ReadStreamRegion*>::iterator reader_iterator;
		typedef std::list<ReadStreamRegion*>::const_iterator reader_const_iterator;

		reader_iterator readers_begin();
		reader_iterator readers_end();
		reader_const_iterator readers_begin() const;
		reader_const_iterator readers_end()   const;

		void AddReader(ReadStreamRegion*);
		void RemoveReader(ReadStreamRegion*);
		bool IsSourceOf(const ReadStreamRegion*) const;

		virtual bool FulfilsInvariant() const;
		bool CanActivate() const;
		virtual bool Accept(RegionVisitor& v) const { return v.Visit(*this); }

		/** This method initializes the source region and all its associated
		 *	readers. All readers are centered around 0 (or offset passed as a
		 *	parameter if you need a different value). The source region is advanced
		 *	so half of the biggest window size is written with zeros. */
		void Init(unsigned int offset=0);
		
		const ReadStreamRegion& GetLastReading() const;
		bool HasReaders() const { return !mReaders.empty(); }

	protected:
		/** Initializes associated reading regions*/
		void InitReaders(unsigned int offset);
		/** Helper function to compute the length of the largest reading region. */
		unsigned int FindLargestReadRegionLength();

		virtual ~SourceStreamRegion() {}
	private:
		bool AllReadersPreceedsThisWithNoOverlap() const;

	};

}

#endif
