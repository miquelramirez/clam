#ifndef SONG_H
#define SONG_H

#include <qcolor.h>
#include <sstream>
//std includes
#include <string>
#include <vector>
#include <map>
#include <qwidgetplugin.h>
#include "Descriptors.hxx"
#include "SegmentInformation.hxx"

class Song 
{
public:
	typedef std::string Id;
	typedef std::vector< SegmentInformation > Segments;
	Song();
	Id GetId() const;
	void setId(Id);
	const QColor & color() const;
	void AddAttribute(const std::string & name, const std::string & value);
	const Descriptors & GetAllAttributes() const;
	const std::string & GetAttributeValue(const std::string & attributeName) const;
	void setColor(int red, int green, int blue);
	void AddSegmentInformation( double origin, double end, const std::string & value);
	void GetSegmentInformation( Song::Segments &);
private:
	static Id GetIdForNewSong()
	{
	 static unsigned id=1;
	 std::ostringstream oss;
	 oss<<id++;
	 
	 return oss.str();
	}
protected:
	Segments mSegments;
	Id mId;
	Descriptors mAttributes;
	QColor mColor;
};

#endif
