#include "Song.hxx"
#include "Assert.hxx"

void Song::setId(Id id)
{
	mId = id;
}

Song::Song()
{
	mColor=QColor(rand()%150+50, rand()%150+50, rand()%150+50);
	mId=GetIdForNewSong();
	
}
Song::Id Song::GetId() const
{
	return mId;
}
const QColor & Song::color() const
{
	return mColor;
}

void Song::AddSegmentInformation( double origin, double end, const std::string & value)
{
	mSegments.push_back( SegmentInformation( origin, end, value ) );
}

void Song::GetSegmentInformation( Song::Segments & segments)
{
	Segments::iterator it = mSegments.begin();
	Segments::iterator itEnd = mSegments.end();
	
	while ( it != itEnd )
	{
		segments.push_back( *it );
		it++;
	}
	
}

void Song::AddAttribute(const std::string & name, const std::string & value)
{
	mAttributes[name]=value;
}
const Descriptors & Song::GetAllAttributes() const
{
	return mAttributes;
}
const std::string & Song::GetAttributeValue(const std::string & attributeName) const
{
	static std::string nullString = "";
	Descriptors::const_iterator it = mAttributes.find(attributeName);
	if (it == mAttributes.end())
		return nullString;
	return it->second;
	
}
void Song::setColor(int red, int green, int blue)
{
	mColor=QColor(red, green, blue);
	
}
