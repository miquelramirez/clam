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

#ifndef StreamImpl_hxx
#define StreamImpl_hxx

#include "Region.hxx"
#include <list>
#include <vector>
#include "PhantomBuffer.hxx"
namespace CLAM
{

/** This declaration is necessary for VC7 compatibility */
template <typename T>
class StdList : public std::list<T>
{};

template <typename T>
class StdVector : public std::vector<T>
{};



template< typename Token, template <class> class DataStructure >
class StreamImpl
{

public:	
	void NewWritingRegionSize( Region& writer );
	/** Do not check anything because the necessary checks are done at consume time*/
	void NewReadingRegionSize( Region& );
	void WriterHasAdvanced( Region& writer );
	void ReaderHasAdvanced( Region& reader );
	/** This method is for generic interface convenience. 
	 * It is used in the PhantomBuffer implementation. 
	 **/
	Token& Read(int physicalIndex, int size);
	Token& operator[](int physicalIndex);
	int LogicalSize() const;
	bool ExistsCircularOverlap(int rear, int writingHead) const;

private:
	void RegionHasAdvanced( Region& region );
	
	DataStructure<Token> mDataImpl;
};

/////// Implementation ////////

template< typename Token, template <class> class DataStructure >
void StreamImpl<Token,DataStructure>::NewWritingRegionSize( Region& writer  )
{
	if ( writer.Size() <= LogicalSize() ) return;
	int newTokens = writer.Size() - LogicalSize();
	for( int i=0; i<newTokens; i++)
		mDataImpl.push_back(Token());
}
	
template< typename Token, template <class> class DataStructure >
void StreamImpl<Token,DataStructure>::RegionHasAdvanced( Region& region ) 
{
	region.BeginDistance( region.BeginDistance() +  region.Hop() );

	if (region.BeginDistance() >= LogicalSize() ) // circular movement
		region.BeginDistance( region.BeginDistance() - LogicalSize() );
}

template< typename Token, template <class> class DataStructure >
void StreamImpl<Token,DataStructure>::NewReadingRegionSize( Region& )
{
}

template< typename Token, template <class> class DataStructure >
void StreamImpl<Token,DataStructure>::WriterHasAdvanced( Region& writer )
{
	for( int i=0; i<writer.Hop(); i++)
		mDataImpl.push_back(Token());

	RegionHasAdvanced( writer );
}

template< typename Token, template <class> class DataStructure >
void StreamImpl<Token,DataStructure>::ReaderHasAdvanced( Region& reader )
{
	// TODO: discard old tokens
	RegionHasAdvanced( reader);
}

template< typename Token, template <class> class DataStructure >
Token& StreamImpl<Token,DataStructure>::Read(int physicalIndex, int size)
{
	return operator[](physicalIndex);
}

template< typename Token, template <class> class DataStructure >
Token& StreamImpl<Token,DataStructure>::operator[](int physicalIndex)
{
	CLAM_DEBUG_ASSERT( physicalIndex < int(mDataImpl.size()), "StreamImpl operator[] - Index out of bounds" );
	typename DataStructure<Token>::iterator it;
	int i;
	for(i=0, it = mDataImpl.begin(); i<physicalIndex; it++, i++);
	return (*it);
}

template< typename Token, template <class> class DataStructure >
int StreamImpl<Token,DataStructure>::LogicalSize() const
{
	return int(mDataImpl.size());
}

template< typename Token, template <class> class DataStructure >
bool StreamImpl<Token,DataStructure>::ExistsCircularOverlap(int rear, int writingHead) const
{
	return false;
}

//---------------------------------------------------------------------------------

template< typename Token >
class StreamImpl<Token, PhantomBuffer>
{

public:
	void NewWritingRegionSize( Region& writer );
	void NewReadingRegionSize( Region& reader );
	/** Notifies the stream impl that the writing region have just produced */
	void WriterHasAdvanced( Region& writer );
	void ReaderHasAdvanced( Region& reader );
	
	Token& Read(int physicalIndex, int size);
	Token& operator[](int physicalIndex);
	int LogicalSize() const;
	/**
		This method appears only in the PhantomBuffer specialization template.
		Most useful for testing purposes.
	*/
	int PhantomSize();
	bool ExistsCircularOverlap(int rear, int writingHead) const;
private:
	int ExponentOfClosestGreaterPowerOfTwo( int newSize);
	void CommonNewRegionSize( Region& anyRegion );	
	bool ReaderAffectedByInsertion( Region & reader, Region & writer ) const;
	void UpdateBeginDistanceOfReadingRegions( Region & writer, int tokensInserted );	
	void RegionHasAdvanced( Region& region );

	PhantomBuffer<Token> mDataImpl;
};

/////// Implementation ////////

template< typename Token >
void StreamImpl<Token, PhantomBuffer>::NewWritingRegionSize( Region& writer )
{
	CLAM_DEBUG_ASSERT( writer.Size()>0, "StreamImpl::newWritingRegionSize() - size must be greater than 0" );
	CLAM_DEBUG_ASSERT( !writer.ProducerRegion(), "StreamImpl::newWritingRegionSize() - region must be a WritingRegion" );
	CommonNewRegionSize( writer );	
}

template< typename Token >
void StreamImpl<Token, PhantomBuffer>::NewReadingRegionSize( Region& reader )
{
	CLAM_DEBUG_ASSERT( reader.ProducerRegion(), "StreamImpl::newReadingRegionSize() - region must be a ReadingRegion" );
	CommonNewRegionSize(reader);
}

template< typename Token >
void StreamImpl<Token, PhantomBuffer>::CommonNewRegionSize( Region& anyRegion )
{
	int newLogicalSize;
	int newPhantomSize;

	if(anyRegion.Size()==1)
	{
		newLogicalSize = anyRegion.Size();
		newPhantomSize = 0;
	}
	else
	{	
		int logicalSizeCandidate = anyRegion.Size()*2;
		newLogicalSize = 1 << ExponentOfClosestGreaterPowerOfTwo(logicalSizeCandidate);
		newPhantomSize = anyRegion.Size()-1;
	}

	if(newLogicalSize <= LogicalSize())
		return;

	CLAM_DEBUG_ASSERT(newLogicalSize > LogicalSize(), "StreamImpl::commonNewRegionSize() - new logical size"
							"must be greater than the older logical size" ); 

	Region & producer = anyRegion.ProducerRegion() ? (*anyRegion.ProducerRegion()) : anyRegion;
	int insertionPos = producer.BeginDistance();

	int tokensToInsert = newLogicalSize - LogicalSize();

	mDataImpl.Resize( 
			newLogicalSize, 
			std::max(newPhantomSize,PhantomSize()), // phantom buffer size
			insertionPos );
	
	UpdateBeginDistanceOfReadingRegions( producer, tokensToInsert );
}

template< typename Token >
bool StreamImpl<Token, PhantomBuffer>::ReaderAffectedByInsertion( Region & reader, Region & writer ) const
{
	// a reader will be affected by the insertion of new tokens due a writer's resize if:
	
	// a) the reader is physically positioned (beginDistance) at the rear of the writer.
	if (reader.BeginDistance() > writer.BeginDistance()) 
		return true;
	
	// b) the reader is physically positioned (beginDistance) at the same position than the writer AND
	// is logically position (pos) after the writer. It means that the writer is exactly at
	// LogicalSize() positions before the reader.
	if( reader.BeginDistance()==writer.BeginDistance() && reader.Pos() < writer.Pos() )
		return true;
	return false;
}

template< typename Token >
void StreamImpl<Token, PhantomBuffer>::UpdateBeginDistanceOfReadingRegions( Region & writer, int tokensInserted )
{
	/// traverses reading regions and only updates the ones
	/// that are at the right hand of the writer
	Region::ReadingRegionsIterator actualReader;

	for ( actualReader=writer.BeginReaders(); actualReader!=writer.EndReaders(); actualReader++)
		if( ReaderAffectedByInsertion(**actualReader, writer) )
			(*actualReader)->BeginDistance( (*actualReader)->BeginDistance() + tokensInserted );
}

template< typename Token >
void StreamImpl<Token, PhantomBuffer>::RegionHasAdvanced( Region& region ) 
{
	region.BeginDistance( region.BeginDistance() + region.Hop());
	if (region.BeginDistance() >= LogicalSize() ) // circular movement
		region.BeginDistance( region.BeginDistance() -  LogicalSize());
}

template< typename Token >
void StreamImpl<Token, PhantomBuffer>::WriterHasAdvanced( Region& writer )	
{
	mDataImpl.Touch( writer.BeginDistance(), writer.Size() );
	RegionHasAdvanced( writer );
}

template< typename Token >
void StreamImpl<Token, PhantomBuffer>::ReaderHasAdvanced( Region& reader )
{
	RegionHasAdvanced( reader );		
}

template< typename Token >		
Token& StreamImpl<Token, PhantomBuffer>::Read(int physicalIndex, int size)
{
	return *mDataImpl.Read( physicalIndex, size );
}

template< typename Token >
Token& StreamImpl<Token, PhantomBuffer>::operator[](int physicalIndex)
{
	CLAM_DEBUG_ASSERT( physicalIndex < LogicalSize()+PhantomSize(), "StreamImpl::operator[] - Index out of bounds" );
	return Read( physicalIndex, 1);
}

template< typename Token >
int StreamImpl<Token, PhantomBuffer>::LogicalSize() const
{
	return mDataImpl.LogicalSize();
}

template< typename Token >
int StreamImpl<Token, PhantomBuffer>::PhantomSize()
{
	return mDataImpl.PhantomSize();
}

template< typename Token >
bool StreamImpl<Token, PhantomBuffer>::ExistsCircularOverlap(int rear, int writingHead) const
{
	return writingHead - rear > LogicalSize(); 
}

template< typename Token >
int StreamImpl<Token, PhantomBuffer>::ExponentOfClosestGreaterPowerOfTwo( int newSize)
{
	int newLogicalSize = 1;
	int power = 0;
	while( newLogicalSize < newSize )
	{
		newLogicalSize <<= 1;
		power++;
	}
	return power;
}

} // namespace CLAM

#endif // StreamImpl_hxx

