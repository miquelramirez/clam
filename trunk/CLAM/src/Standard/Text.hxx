#ifndef _TEXT_HXX_
#define _TEXT_HXX_
#include <iosfwd>
#include <string>
#if defined( _MSC_VER ) && ( _MSC_VER < 1310 )
#include <sstream>
#endif
#include "TypeInfo.hxx"

namespace CLAM
{
	/**
	 * This class represents an string that can contain any kind of separators.
	 * Use it instead std::string if you want to deserialize
	 * It inherits all the std::string methods.
	 */
	class Text : public std::basic_string<char>
	{
		public:
			Text()
			{
			}
			Text(const std::string & value)
				: std::basic_string<char>(value)
			{
			}
			Text(const char * value)
				: std::basic_string<char>(value)
			{
			}
	};

	std::istream & operator >> (std::istream & stream, Text & text);

#if defined( _MSC_VER ) && ( _MSC_VER < 1310 )
	/**
	* This is a kludge to make windows work
	*/

	inline std::istream& operator>>( std::istringstream& stream, CLAM::Text& text )
	{
		std::istream & basestream = stream;
		return basestream >> text;
	}
#endif

	CLAM_TYPEINFOGROUP(BasicCTypeInfo, Text);
}


#endif // _TEXT_HXX_
