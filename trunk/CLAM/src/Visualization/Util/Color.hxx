#ifndef __COLOR__
#define __COLOR__

namespace CLAMVM
{

struct Color
{
	char r;
	char g;
	char b;

	Color( char _r = 0, char _g = 0, char _b=0 )
		: r( _r ), g( _g ), b( _b )
	{
	}
};

}
#endif // Color.hxx
