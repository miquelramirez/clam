#ifndef __VMQT_PALETTE_H__
#define __VMQT_PALETTE_H__

#include <map>
#include "Assert.hxx"
#include "DataTypes.hxx"
#include "vm_data_types.hxx"

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

			static Colormap wp_color_map;

		public:
			Palette(float v);
			~Palette();

			void value(float v);	
			float value(void) { return wp_value; }
			void get_rgb_from_index( int idx, unsigned short& r, unsigned short& g, unsigned short& b )
				{
					Color c = wp_color_map[idx];
					r = c.r;
					g = c.g;
					b = c.b;
				}

			int get(float v);
				
		protected:
			static int wp_inited;

			static void init_colors();
			// This value allows to introduce a 'boost' for input values,
			// thus 'saturating' the resulting colors
			float wp_value;
			int*  wp_color_mapping;

			void calc_color_map();

		};

		inline void Palette::value(float v)
		{
			//D/ Adjust the color scale.
			wp_value=v; calc_color_map();
		}

		inline int Palette::get(float v) 
		{
			// Return the corresponding scaled color for value v (0-1).
			return (FIRST_INDEX+wp_color_mapping[int(v*float(NCOLORMAPPING-1))]);
		}
	}
}

#endif


