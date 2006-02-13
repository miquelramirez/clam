#ifndef __VMQT_PALETTE_H__
#define __VMQT_PALETTE_H__

#include <map>
#include "Assert.hxx"
#include "DataTypes.hxx"
#include "vmDataTypes.hxx"

namespace CLAM
{
	namespace VM
	{
		/** 
		 * Class to convert values in the range 0-1 to a color scale.
		 * The color scale can be adjusted, so weaker values can be
		 * emphasized, are be darkened, so the stronger values stick
		 * out more clearly.
		 *
		 */
		class Palette
		{
			typedef std::map<int,Color> Colormap;
			
			static const int NCOLORMAPPING;
			static const int FIRST_INDEX;
			static const int NCOLORS;

			static Colormap mColormap;

		public:
			Palette(float v);
			~Palette();

			void Value(float v);	
			float Value() { return mValue; }
			void GetRGBFromIndex( int idx, unsigned short& r, unsigned short& g, unsigned short& b )
				{
					Color c = mColormap[idx];
					r = c.r;
					g = c.g;
					b = c.b;
				}

			int Get(float v);
				
		protected:
			static int mInited;

			static void InitColors();
			// This value allows to introduce a 'boost' for input values,
			// thus 'saturating' the resulting colors
			float mValue;
			int*  mColormapping;

			void CalcColormap();

		};

		inline void Palette::Value(float v)
		{
			//D/ Adjust the color scale.
			mValue=v; CalcColormap();
		}

		inline int Palette::Get(float v) 
		{
			// Return the corresponding scaled color for value v (0-1).
			return (FIRST_INDEX+mColormapping[int(v*float(NCOLORMAPPING-1))]);
		}
	}
}

#endif


