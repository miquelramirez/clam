#ifndef __VMCOLOR__
#define __VMCOLOR__

#include "DataTypes.hxx"

namespace CLAM
{
	namespace VM
	{
		typedef struct{
			TUInt16 r;
			TUInt16 g;
			TUInt16 b;
		} Color;
		
		class VMColor
		{
			public:
				static Color Red();
				static Color Green();
				static Color Blue();
				static Color Magenta();
				static Color Cyan();
				static Color Yellow();
				static Color White();
				static Color Black();
				static Color Gray();
				static Color LightGray();
				static Color Brown();
				static Color Orange();
				static Color Custom(TUInt16 r, TUInt16 g, TUInt16 b);
		};

	}
}

#endif
