#include "Text.hxx"

#include <sstream>
#include <iterator>

namespace CLAM
{

	std::istream & operator >> (std::istream & stream, Text & text)
	{
#if 0
		stream.unsetf(std::ios::skipws);
		text assign(
			(std::istream_iterator<char>(stream)),
			std::istream_iterator<char>());
#elif 0
		text.assign(
			(std::istreambuf_iterator<char>(stream)),
			std::istreambuf_iterator<char>());
#elif 1
		text = "";
		char buffer[1024];
		stream.read( buffer, 1023 );
		do 
		{
			text.append( buffer, stream.gcount() );
		}
		while( stream.read( buffer, 1023 ) );
#endif
		return stream;

	}

}


