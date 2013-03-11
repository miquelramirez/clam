/*
 * Copyright (c) 2004 MUSIC TECHNOLOGY GROUP (MTG)
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

#ifndef __Region_hxx__
#define __Region_hxx__

#include <list>

namespace CLAM
{

/**
	@ingroup PortsImplementation
	A Region defines a sliding window in a data token stream.

	This class is an internal implementation detail not to be used by users.
*/
class Region
{
public:
	typedef std::list<Region*> ReadingRegionsList;
	typedef ReadingRegionsList::iterator ReadingRegionsIterator;

	Region();
	virtual ~Region();

	/// Current window position
	long Pos() const;
	/// Number of tokens accessible at once
	int Size() const;
	/// Number of tokens the window advances each time
	int Hop() const;
	int BeginDistance();

	void Pos(long pos );
	void Size(int size );
	void Hop(int hop );
	void BeginDistance(int dist );
	
	virtual void RemoveProducer(){}
	virtual Region* ProducerRegion() = 0;
	virtual ReadingRegionsIterator BeginReaders() = 0;
	virtual ReadingRegionsIterator EndReaders() = 0;
	virtual void RemoveRegion( Region & Region ){}

protected:
 	virtual void SizeChanged(int newSize) = 0;

private:
	long mPos;
	int mSize;
	int mHop;
	
	 /// offset from the beginning of the data structure 
	int mBeginDistance;
};

} // namespace CLAM

#endif // __Region_hxx__

