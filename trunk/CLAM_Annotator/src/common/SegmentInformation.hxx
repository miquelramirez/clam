#ifndef SEGMENT_INFORMATION_HXX
#define SEGMENT_INFORMATION_HXX

#include <string>

class SegmentInformation
{
	public:
		SegmentInformation( double origin, double end, const std::string & information);
		double origin() const;
		double end() const;
		void information( std::string &);
		void setOrigin( double );
		void setEnd( double );
		void setInformation( const std::string &);
	private:
		double mOrigin;
		double mEnd;
		std::string mInformation;
};

#endif

