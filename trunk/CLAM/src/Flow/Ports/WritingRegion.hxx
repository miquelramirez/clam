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

#ifndef __WritingRegion_hxx__
#define __WritingRegion_hxx__

#include "ReadingRegion.hxx"
#include "StreamImpl.hxx"

#include <iostream> // TODO: remove

namespace CLAM
{

template< typename Token, template <class> class DataStructure = PhantomBuffer>
class WritingRegion : public Region
{
public:
	typedef StreamImpl< Token, DataStructure > ProperStream;
	typedef Token ProperToken;
	typedef ReadingRegion< WritingRegion<Token,DataStructure> > ProperReadingRegion;

	WritingRegion();
	virtual ~WritingRegion();

	Region::ReadingRegionsIterator BeginReaders();
	Region::ReadingRegionsIterator EndReaders();
	void LinkRegions( ProperReadingRegion &  reader);
	void RemoveRegion( Region & region );

	/**
	 * As a precondition all the regions must be even (in order to center them correctly).
	 * Otherways the method gives an assertion
	 */
	void CenterEvenRegions();

	/**
		returns the underlying stream. Useful for testing.
	*/
	ProperStream& Stream();

	/** when no linked reading region is found it returns the writing pos thus eviting overlap condition */
	int RearmostReadingPos();
	
	/**
		Checks that after moving the writing-region no overlap occur with
		reading regions. This could happen since this is a circular buffer.
		Returns true if no overlapping occur so it can produce.
	*/
	bool CanProduce();
	void Produce();

	Token& operator[](int offset);  // TODO decide if operator[0] is the best option to get the data chunk	
	bool FulfilsInvariant();
	int LogicalStreamSize();
	Region* ProducerRegion();

	/**
	 * Initializes all the data contained inside the region ( to the Token's default constructor)
	 */
	void ClearData();


	/*
	 * Synchronizes Follower Region and returns last written Tokens
	 */
	Token & GetLastWrittenData( int offset = 0);
private:
	/** Don't allow copies. Thus copy contructor made private */
	WritingRegion(const WritingRegion<Token>& original){}
	void SizeChanged(const int & newSize);
	int GetGreatestReaderRegionSize();

	/*
	 * This method checks that all the regions have even size and are located at the correct position (the beginning of the buffer )
	 */
	void CheckRegionsAreEven();
	void CenterReadingRegions( int centralIndex );

	/*
	 *  Synchronizes the follower region with the last zone written, with the same size and hop than the writer.
	 */	
	void SyncFollowerRegion();

	ReadingRegionsList mReadingRegions;
	ProperStream  mStream;
	ProperReadingRegion mFollowerRegion;
};

/////// Implementation ////////

template< typename Token, template <class> class DataStructure>
WritingRegion< Token, DataStructure >::WritingRegion()
{
	SizeChanged( Size() );
}

template< typename Token, template <class> class DataStructure>
WritingRegion< Token, DataStructure >::~WritingRegion()
{	
	ReadingRegionsIterator it;
	for(it=BeginReaders(); it!=EndReaders(); it++)	
		(*it)->RemoveProducer();
	mReadingRegions.clear();
}

template< typename Token, template <class> class DataStructure>
Region::ReadingRegionsIterator WritingRegion< Token, DataStructure >::BeginReaders()
{
	return mReadingRegions.begin();
}

template< typename Token, template <class> class DataStructure>
Region::ReadingRegionsIterator WritingRegion< Token, DataStructure >::EndReaders()
{
	return mReadingRegions.end();
}

template< typename Token, template <class> class DataStructure>
void WritingRegion< Token, DataStructure >::LinkRegions( ProperReadingRegion &  reader)
{
	mReadingRegions.push_back(&reader);
	reader.LinkProducerRegion(*this);
	reader.LinkAndNotifySizeToStream( Stream() );
}

template< typename Token, template <class> class DataStructure>
int WritingRegion< Token, DataStructure >::GetGreatestReaderRegionSize()
{
	int maxSize = 0;
	
	ReadingRegionsIterator it;
	for(it=BeginReaders(); it!=EndReaders(); it++)
	{
		if(((*it)->Size())>maxSize) 
			maxSize = (*it)->Size();
	}

	return maxSize;
}

template< typename Token, template <class> class DataStructure>
void WritingRegion<Token, DataStructure >::CheckRegionsAreEven()
{
	//TODO: writer must be even?	
	CLAM_ASSERT( Size()%2==0, "WritingRegion::CheckRegionsAreEven - Writer's size must be even for centering." );
	
	ReadingRegionsIterator it;
	for(it=BeginReaders(); it!=EndReaders(); it++)
	{
		CLAM_ASSERT( (*it)->Size()%2==0, "WritingRegion::CheckRegionsAreEven - Reader's size must be even for centering.");
	}

}
	
template< typename Token, template <class> class DataStructure>
void WritingRegion< Token, DataStructure >::ClearData()
{
	for( int i=0; i<Size(); i++ )		
		operator[](i) = Token();
}

template< typename Token, template <class> class DataStructure>
void WritingRegion< Token, DataStructure >::CenterReadingRegions( int centralIndex )
{
	ReadingRegionsIterator it;
	for(it=BeginReaders(); it!=EndReaders(); it++)
	{
		int hopToMove = centralIndex - (*it)->Size()/2; 
		(*it)->Pos( hopToMove );
		(*it)->BeginDistance( hopToMove );
	}
}

template< typename Token, template <class> class DataStructure>
void WritingRegion< Token, DataStructure >::CenterEvenRegions()
{
	CheckRegionsAreEven();
	int centralIndex = GetGreatestReaderRegionSize()/2;

	Pos( 0 );
	BeginDistance( 0);

	int currentHop = Hop();
	int currentSize = Size();

	Size( centralIndex);	
	Hop( centralIndex );

	ClearData();
	

	CenterReadingRegions( centralIndex );

	Produce();
	Size( currentSize );
	Hop( currentHop );

	

}

template< typename Token, template <class> class DataStructure>
void WritingRegion< Token, DataStructure >::RemoveRegion( Region & region )
{
	CLAM_ASSERT( region.ProducerRegion()==this, "Region::RemoveRegion() - Trying to remove an unlinked region");

	mReadingRegions.remove( &region );
	region.RemoveProducer();
}

template< typename Token, template <class> class DataStructure>
typename WritingRegion< Token, DataStructure>::ProperStream& WritingRegion< Token, DataStructure >::Stream()
{
	return mStream;
}

template< typename Token, template <class> class DataStructure>
int WritingRegion< Token, DataStructure >::RearmostReadingPos()
{
	int result = Pos();
	ReadingRegionsIterator it;
	for(it=mReadingRegions.begin(); it!=mReadingRegions.end(); it++)
		if( (*it)->Pos() < result) result = (*it)->Pos();

	return result;
}

template< typename Token, template <class> class DataStructure>
bool WritingRegion< Token, DataStructure >::CanProduce()
{
	CLAM_DEBUG_ASSERT( FulfilsInvariant(), "WritingRegion::canProduce() - fulfils invariant checking failed" );
	return !mStream.ExistsCircularOverlap( RearmostReadingPos(), Pos()+Size() );
}

template< typename Token, template <class> class DataStructure>
void WritingRegion< Token, DataStructure >::Produce()
{
	CLAM_DEBUG_ASSERT( CanProduce(), "WritingRegion::produce() - WritingRegion can't produce" );
	Pos( Pos() + Hop());
	// reserve stream tokens for next position
	mStream.WriterHasAdvanced( *this );
}

template< typename Token, template <class> class DataStructure>
Token& WritingRegion< Token, DataStructure >::operator[](int offset) 
{

	CLAM_DEBUG_ASSERT(0 <= offset && offset < Size(), "WritingRegion::operator [] - Index out of bounds" ); 
	int physicalIndex = BeginDistance() + offset;

	if (offset==0)
		return mStream.Read(physicalIndex, Size());
	return mStream[physicalIndex];
}

template< typename Token, template <class> class DataStructure>
bool WritingRegion< Token, DataStructure >::FulfilsInvariant()
{
	ReadingRegionsIterator it;
	for(it=mReadingRegions.begin(); it!=mReadingRegions.end(); it++)
		if( (*it)->Pos() >= Pos()+Size() )
			return false;
	return true;
}

template< typename Token, template <class> class DataStructure>
int WritingRegion< Token, DataStructure >::LogicalStreamSize()
{
	return mStream.LogicalSize();
}

template< typename Token, template <class> class DataStructure>
Region* WritingRegion< Token, DataStructure >::ProducerRegion()
{
	return 0;
}

template< typename Token, template <class> class DataStructure>
void WritingRegion< Token, DataStructure >::SizeChanged(const int & newSize)
{
	mStream.NewWritingRegionSize( *this );
}

template< typename Token, template <class> class DataStructure>
void WritingRegion< Token, DataStructure >::SyncFollowerRegion()
{
	mFollowerRegion.LinkProducerRegion(*this);
	mFollowerRegion.Size( Size() );
	mFollowerRegion.Hop( Hop() );	
	mFollowerRegion.LinkAndNotifySizeToStream( Stream() );

	mFollowerRegion.Pos( Pos() - Hop() );
	mFollowerRegion.BeginDistance( BeginDistance() - Hop() );
	if(mFollowerRegion.BeginDistance() < 0 )
		mFollowerRegion.BeginDistance( mFollowerRegion.BeginDistance() + mStream.LogicalSize() );
}

template< typename Token, template <class> class DataStructure>
Token & WritingRegion< Token, DataStructure >::GetLastWrittenData( int offset)
{
	SyncFollowerRegion();
	int physicalIndex = mFollowerRegion.BeginDistance() + offset;
//	CLAM_ASSERT( physicalIndex < mStream.LogicalSize(), "WritingRegion GetLastWrittenData - Out of bounds access"  );
	return mStream.operator[]( physicalIndex );
}

} // namespace CLAM

#endif // __WritingRegion_hxx__

