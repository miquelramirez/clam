#include <cstdlib>
#include "CLAM_Math.hxx"
#include "vm_palette.hxx"

namespace CLAM
{
	namespace VM
	{

		const int Palette::NCOLORMAPPING = 8192;
		const int Palette::FIRST_INDEX = 64;
		const int Palette::NCOLORS = 64;

		Palette::Colormap Palette::wp_color_map;

		int Palette::wp_inited=0;

		Palette::Palette(float v)
			: wp_color_mapping(0)
		{
			wp_color_mapping = new int[NCOLORMAPPING];

			init_colors();
			wp_value=v;
			calc_color_map();
		}

		Palette::~Palette()
		{
			if ( wp_color_mapping )
				delete [] wp_color_mapping;
		}

		void Palette::calc_color_map()
		{
			//D/ Calculate the conversion color map, depending and the scaling value.
			int i;
			float v = 0.34-wp_value/3.;
			for (i=0;i<NCOLORMAPPING;i++) 
			{
				wp_color_mapping[i]=int(pow(float(i)/float(NCOLORMAPPING),v)*float(NCOLORS));
			}
		}

		void Palette::init_colors()
		{
			//D/ Create the colors in the FLTK colormap
			if (wp_inited) return;

			wp_inited=1;
			
			int colors_index[6];

			colors_index[0] = 0;
			colors_index[1] = 12;
			colors_index[2] = 25;
			colors_index[3] = 37;
			colors_index[4] = 50;
			colors_index[5] = 64;
			
			wp_color_map[FIRST_INDEX+colors_index[0]] = CLAM::VM::Color(0,0,0);
			wp_color_map[FIRST_INDEX+colors_index[1]] = CLAM::VM::Color(15,17,16);
			wp_color_map[FIRST_INDEX+colors_index[2]] = CLAM::VM::Color(80,100,153);
			wp_color_map[FIRST_INDEX+colors_index[3]] = CLAM::VM::Color(90,180,100);
			wp_color_map[FIRST_INDEX+colors_index[4]] = CLAM::VM::Color(224,224,44);
			wp_color_map[FIRST_INDEX+colors_index[5]] = CLAM::VM::Color(255,155,80);

			int n = 0;
			int nDif = 0;
			for(int k=0; k < NCOLORS; k+=nDif,n++)
			{
				nDif = colors_index[n+1] - colors_index[n];
				for(int i=1; i < nDif; i++)
				{
					unsigned short R,G,B;
					unsigned short R1,G1,B1;
					unsigned short R2,G2,B2;

					CLAM::VM::Color c;

					c = wp_color_map[FIRST_INDEX + k];
					R1 = c.r;
					G1 = c.g;
					B1 = c.b;
					
					c = wp_color_map[FIRST_INDEX + k+nDif];
					R2 = c.r;
					G2 = c.g;
					B2 = c.b;
					
					float factor = (float)i / nDif;
					R = R1 + int((R2 - R1)*factor);
					G = G1 + int((G2 - G1)*factor);
					B = B1 + int((B2 - B1)*factor);
							
					wp_color_map[FIRST_INDEX+k+i] = CLAM::VM::Color(R,G,B);
				}
			}
		}
	}

}

// END


