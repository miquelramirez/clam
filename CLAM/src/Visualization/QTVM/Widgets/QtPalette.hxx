#ifndef __QTPALETTE__
#define __QTPALETTE__

#include <qglcolormap.h>
#include "Assert.hxx"
#include "DataTypes.hxx"

namespace CLAM
{
	namespace VM
	{
		class QtPalette
		/*D* 
		Class to convert values in the range 0-1 to a color scale.
		The color scale can be adjusted, so weaker values can be
			emphasized, are be darkened, so the stronger values stick
			out more clearly.

		*/
		{
			static const int NCOLORMAPPING;
			static const int FIRST_INDEX;
			static const int NCOLORS;

			static QGLColormap _colorMap;

			protected:
				static int inited_;
				static void InitColors(void);
				// This value allows to introduce a 'boost' for input values,
				// thus 'saturating' the resulting colors
				float value_;
				int* colormapping;
				void CalcColorMap(void);
			public:
				void value(float v);
				
				float value(void) { return value_; }
				
				void GetRGBFromIndex( int idx, TUInt16& r, TUInt16& g, TUInt16& b )
				{
					QColor c = _colorMap.entryColor(idx);
					r = (TUInt16)c.red();
					g = (TUInt16)c.green();
					b = (TUInt16)c.blue();
				}

				int Get(float v);
				
				QtPalette(float v);
				~QtPalette();
		};

		inline void QtPalette::value(float v)
		{
			//D/ Adjust the color scale.
			value_=v; CalcColorMap();
		}

		inline int QtPalette::Get(float v) 
		{
			// Return the corresponding scaled color for value v (0-1).
			return (FIRST_INDEX+colormapping[int(v*float(NCOLORMAPPING-1))]);
		}
	}

}
#endif


