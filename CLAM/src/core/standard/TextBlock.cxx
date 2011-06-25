/*
 */

#include "TextBlock.hxx"

#include <sstream>
#include <iterator>

namespace CLAM
{

	std::istream & operator >> (std::istream & stream, TextBlock & textBlock)
	{
		textBlock.assign(
			(std::istreambuf_iterator<char>(stream)),
			std::istreambuf_iterator<char>());
		return stream;

	}

}


