
#include "Region.hxx"
#include "Assert.hxx"

namespace CLAM
{

Region::Region() 
	: mPos(0), mSize(1), mHop(1), mBeginDistance(0)
{
}

Region::~Region()
{
}

const long & Region::Pos() const 
{
	return mPos;
}

const int & Region::Size() const
{
	return mSize;
}

const int & Region::Hop() const
{
	return mHop;
}

const int & Region::BeginDistance()
{
	return mBeginDistance;
}

void Region::BeginDistance( const int &dist )
{
	mBeginDistance = dist;		
}

void Region::Pos( const long & pos )
{
	mPos = pos;
}

void Region::Size( const int & size )
{
	mSize = size;
	SizeChanged(size);
}

void Region::Hop( const int & hop )
{
	CLAM_ASSERT( hop <= mSize, "Region::hop() - hop can't be greater than size" );
	mHop = hop;
}

} // namespace CLAM

