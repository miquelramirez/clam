#include "SegmentInformation.hxx"

SegmentInformation::SegmentInformation( double origin, double end, const std::string & information ) : mOrigin( origin ), mEnd( end ), mInformation( information )
{
}

void SegmentInformation::setOrigin( double origin )
{
	mOrigin = origin;
}

void SegmentInformation::setEnd( double end )
{
	mEnd = end;
}

void SegmentInformation::setInformation( const std::string & information)
{
	mInformation = information;
}

double SegmentInformation::origin() const
{
	return mOrigin;
}

double SegmentInformation::end() const
{
	return mEnd;
}

void SegmentInformation::information( std::string & info ) 
{
	info = mInformation;
}

