#include "VMColor.hxx"

namespace CLAM
{
	namespace VM
	{
		Color VMColor::Red()
		{
			return Custom(255,0,0);
		}

		Color VMColor::Green()
		{
			return Custom(0,255,0);
		}

		Color VMColor::Blue()
		{
			return Custom(0,0,255);
		}

	    Color VMColor::Magenta()
		{
			return Custom(255,0,255);
		}

		Color VMColor::Cyan()
		{
			return Custom(0,255,255);
		}

		Color VMColor::White()
		{
			return Custom(255,255,255);
		}

		Color VMColor::Black()
		{
			return Custom(0,0,0);
		}

		Color VMColor::Yellow()
		{
			return Custom(255,255,0);
		}

		Color VMColor::Gray()
		{
			return Custom(127,127,127);
		}
		
		Color VMColor::LightGray()
		{
			return Custom(185,185,185);
		}

		Color VMColor::Brown()
		{
			return Custom(153,102,31);
		}

	    Color VMColor::Orange()
		{
			return Custom(250,159,31);
		}

		Color VMColor::Custom(TUInt16 r, TUInt16 g, TUInt16 b)
		{
			Color c;
			c.r = r;
			c.g = g;
			c.b = b;
			return c;
		}

	}
}

// END
