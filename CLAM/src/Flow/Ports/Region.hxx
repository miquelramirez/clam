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

