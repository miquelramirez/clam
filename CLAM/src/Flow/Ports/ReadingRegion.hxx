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

#ifndef ReadingRegion_hxx
#define ReadingRegion_hxx

#include "Region.hxx"
#include "StreamImpl.hxx"

namespace CLAM
{

template<class WritingRegion>
class ReadingRegion : public Region
{
	typedef typename WritingRegion::ProperStream ProperStream;
	typedef typename WritingRegion::ProperToken ProperToken;

public:
	ReadingRegion();
	~ReadingRegion();

	void LinkAndNotifySizeToStream( ProperStream& stream );
	ProperStream& Stream();
	/** Condition of overlap between reading and writing regions.
		Returns true if are not overlapping so it can consume
	*/
	bool CanConsume();
	/**
		This method must be called when the data of the reading-region has been
		already readen (consumed) and we want to advance the region position
		for further	readings (consumes)
	*/
	void Consume();
	void LinkProducerRegion( Region& writing);
	
	// const?? TODO
	ProperToken& operator[](int offset);

	Region* ProducerRegion();
	void RemoveProducer();
	
 	ReadingRegionsIterator BeginReaders();
	ReadingRegionsIterator EndReaders();

private:
	void SizeChanged(const int & newSize);

	ProperStream* mAttachedStream;
	Region* mProducingRegion;
};

/////// Implementation ////////

template<class WritingRegion>
ReadingRegion<WritingRegion>::ReadingRegion() 
		: mAttachedStream(0), mProducingRegion(0)
{
}

template<class WritingRegion>
ReadingRegion<WritingRegion>::~ReadingRegion()
{
	if(mProducingRegion)
		mProducingRegion->RemoveRegion( *this );
}

template<class WritingRegion>
void ReadingRegion<WritingRegion>::LinkAndNotifySizeToStream( ProperStream& stream )
{
	mAttachedStream = &stream;
	mAttachedStream->NewReadingRegionSize( *this );
}

template<class WritingRegion>
typename ReadingRegion<WritingRegion>::ProperStream & ReadingRegion<WritingRegion>::Stream()
{
	return *(mAttachedStream);
}

template<class WritingRegion>
bool ReadingRegion<WritingRegion>::CanConsume()
{
	if(!mProducingRegion)
		return false;
	return ProducerRegion()->Pos() >= Pos()+Size();
}

template<class WritingRegion>
void ReadingRegion<WritingRegion>::Consume()
{
	CLAM_DEBUG_ASSERT( CanConsume(), "ReadingRegion::Consume() - region can't consume" );
	Pos( Pos() + Hop() );
	mAttachedStream->ReaderHasAdvanced( *this );
}

template<class WritingRegion>
void ReadingRegion<WritingRegion>::LinkProducerRegion( Region& writing)
{
	mProducingRegion = &writing;
	// it starts at the same position than the writer is in this exact moment
	Pos( writing.Pos() );
	BeginDistance( writing.BeginDistance() );
}

template<class WritingRegion>
typename ReadingRegion<WritingRegion>::ProperToken & ReadingRegion<WritingRegion>::operator[](int offset)
{
	CLAM_DEBUG_ASSERT( mAttachedStream, "ReadingRegion operator[] - No attached stream" );
	CLAM_DEBUG_ASSERT( 0 <= offset && offset < Size(), "ReadingRegion operator[] - Index out of bounds" );
	CLAM_DEBUG_ASSERT( CanConsume(), "ReadingRegion operator[] - region can't consume" );

	int physicalIndex = BeginDistance() + offset;

	return mAttachedStream->operator[](physicalIndex);
}

template<class WritingRegion>
Region* ReadingRegion<WritingRegion>::ProducerRegion()
{
	return mProducingRegion;
}

	template<class WritingRegion>
void ReadingRegion<WritingRegion>::RemoveProducer()
{
	mProducingRegion = 0;
	mAttachedStream = 0;
}

template<class WritingRegion>
typename ReadingRegion<WritingRegion>::ReadingRegionsIterator ReadingRegion<WritingRegion>::BeginReaders()
{
	throw 0; 
}

template<class WritingRegion>
typename ReadingRegion<WritingRegion>::ReadingRegionsIterator ReadingRegion<WritingRegion>::EndReaders()
{
	throw 0; 
}

template<class WritingRegion>
void ReadingRegion<WritingRegion>::SizeChanged(const int & newSize)
{
	if (mAttachedStream)
		mAttachedStream->NewReadingRegionSize(*this);
}

} // namespace CLAM

#endif // __ReadingRegion_hxx__

