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

#ifndef _STREAM_REGION_CONTAINER_H
#define _STREAM_REGION_CONTAINER_H

#include <list>

#include "ReadStreamRegion.hxx"
#include "WriteStreamRegion.hxx"
#include "DelayStreamRegion.hxx"
#include "AddStreamRegion.hxx"
#include "InplaceStreamRegion.hxx"

namespace CLAM {


	class StreamRegionContainer;

	/** Region visitor which implements region searches in the
	 * container.
	 *
	 * This is a helper class. It can be seen as the part of
	 * RegionContainer which implements the search to see if a certain
	 * region is contained in it.
	 *
	 * This search is different depending on the type of region being
	 * searched, but we don't want to put this functionality in the
	 * regions themselves, so a visitor pattern is used.
	 */
	class RegionFinder : public RegionVisitor
	{
		const StreamRegionContainer& mContainer;
	public:
		RegionFinder(const StreamRegionContainer& container)
			: mContainer(container)
			{}
		virtual bool Visit(const ReadStreamRegion&);
		virtual bool Visit(const SourceStreamRegion&);
		virtual bool Visit(const WriteStreamRegion&);
		virtual bool Visit(const DelayStreamRegion&);
		virtual bool Visit(const InplaceStreamRegion&);
	};


	/** Stream Region container class.
	 *  This is a companion class for StreamBuffer. Its roles are:
	 *     - Storing the regions registered in the stream
	 *     - Checking the consistency of the region graph
	 *     - Providing convenience iterator interfaces and accessors
	 *       to different region types.
	 */
	class StreamRegionContainer {

		std::list<SourceStreamRegion*> mSources;

		friend class RegionFinder;
		RegionFinder mFinder;

	public:

		StreamRegionContainer();
		StreamRegionContainer(const StreamRegionContainer&);

		typedef std::list<SourceStreamRegion*>::iterator source_iterator;
		typedef std::list<SourceStreamRegion*>::const_iterator source_const_iterator;

		source_iterator sources_begin();
		source_iterator sources_end();
		source_const_iterator sources_begin() const;
		source_const_iterator sources_end()   const;

		void SetWriter  (WriteStreamRegion   *writer);
		void SetAdder  (AddStreamRegion   *adder);

		void AddDelay   (DelayStreamRegion   *new_delay,
		                 SourceStreamRegion  *source=0);
		void AddInplace (InplaceStreamRegion *new_inplace,
		                 SourceStreamRegion  *source=0);
		void AddReader  (ReadStreamRegion    *new_reader,
		                 SourceStreamRegion  *source=0);
		void RemoveReader(ReadStreamRegion * reader);

		WriteStreamRegion *Writer();
		const WriteStreamRegion *Writer() const;
		AddStreamRegion *Adder();
		const AddStreamRegion *Adder() const;
		const SourceStreamRegion *LastSource() const;

 		bool Contains(const StreamRegion*);

		// Necessary kludge because of multiple inheritance.
 		bool Contains(const DelayStreamRegion*);
 		bool Contains(const InplaceStreamRegion*);


		bool FulfilsInvariant() const;

		//XA:
		void Init();
		unsigned int FindLargestReadRegionLength() const;
	};
};

#endif
