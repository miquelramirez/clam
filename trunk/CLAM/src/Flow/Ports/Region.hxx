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
	
class Region
{
public:
	typedef std::list<Region*> ReadingRegionsList;
	typedef ReadingRegionsList::iterator ReadingRegionsIterator;

	Region();
	virtual ~Region();

	const long& Pos() const;
	const int& Size() const;
	const int& Hop() const;
	const int& BeginDistance();

	void Pos( const long & pos );
	void Size( const int & size );
	void Hop( const int & hop );
	void BeginDistance( const int & dist );
	
	virtual void RemoveProducer(){}
	virtual Region* ProducerRegion() = 0;
	virtual ReadingRegionsIterator BeginReaders() = 0;
	virtual ReadingRegionsIterator EndReaders() = 0;
	virtual void RemoveRegion( Region & Region ){}

protected:
 	virtual void SizeChanged(const int & newSize) = 0;

private:
	long mPos;
	int mSize;
	int mHop;
	
	 /// offset from the beginning of the data structure 
	int mBeginDistance;
};

} // namespace CLAM

#endif // __Region_hxx__

