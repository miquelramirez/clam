/*
 */

#ifndef _TEXTBLOCK_HXX_
#define _TEXTBLOCK_HXX_

#include "Text.hxx"

namespace CLAM
{
	/**
	 * This class represents an string that can contain any kind of separators.
	 * Use it instead std::string if you want to deserialize
	 * It inherits all the std::string methods.
	 */
	class TextBlock : public Text
	{
		public:
			TextBlock()
			{
			}
			
			TextBlock(const std::string & value)
				: Text(value)
			{
			}
			
			TextBlock(const char * value)
				: Text(value)
			{
			}
	};

	std::istream & operator >> (std::istream & stream, TextBlock & textBlock);

#if defined( _MSC_VER ) && ( _MSC_VER < 1310 )
	/**
	* This is a kludge to make windows work
	*/

	inline std::istream& operator>>( std::istringstream& stream, CLAM::TextBlock& textBlock )
	{
		std::istream & basestream = stream;
		return basestream >> textBlock;
	}
#endif

	CLAM_TYPEINFOGROUP(BasicCTypeInfo, TextBlock);
}

#endif // _TEXTBLOCK_HXX_

