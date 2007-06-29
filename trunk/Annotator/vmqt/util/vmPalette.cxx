/*
 * Copyright (c) 2001-2006 MUSIC TECHNOLOGY GROUP (MTG)
 *                         UNIVERSITAT POMPEU FABRA
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include <cstdlib>
#include "CLAM_Math.hxx"
#include "vmPalette.hxx"

namespace CLAM
{
	namespace VM
	{

		const int Palette::NCOLORMAPPING = 8192;
		const int Palette::FIRST_INDEX = 64;
		const int Palette::NCOLORS = 64;

		Palette::Colormap Palette::mColormap;

		int Palette::mInited=0;

		Palette::Palette(float v)
			: mColormapping(0)
		{
			mColormapping = new int[NCOLORMAPPING];

			InitColors();
			mValue=v;
			CalcColormap();
		}

		Palette::~Palette()
		{
			if ( mColormapping )
				delete [] mColormapping;
		}

		void Palette::CalcColormap()
		{
			//D/ Calculate the conversion color map, depending and the scaling value.
			int i;
			float v = 0.34-mValue/3.;
			for (i=0;i<NCOLORMAPPING;i++) 
			{
				mColormapping[i]=int(pow(float(i)/float(NCOLORMAPPING),v)*float(NCOLORS));
			}
		}

		void Palette::InitColors()
		{
			//D/ Create the colors in the FLTK colormap
			if (mInited) return;

			mInited=1;
			
			int colors_index[6];

			colors_index[0] = 0;
			colors_index[1] = 12;
			colors_index[2] = 25;
			colors_index[3] = 37;
			colors_index[4] = 50;
			colors_index[5] = 64;
			
			mColormap[FIRST_INDEX+colors_index[0]] = QColor(0,0,0);
			mColormap[FIRST_INDEX+colors_index[1]] = QColor(15,17,16);
			mColormap[FIRST_INDEX+colors_index[2]] = QColor(80,100,153);
			mColormap[FIRST_INDEX+colors_index[3]] = QColor(90,180,100);
			mColormap[FIRST_INDEX+colors_index[4]] = QColor(224,224,44);
			mColormap[FIRST_INDEX+colors_index[5]] = QColor(255,155,80);

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

					QColor c;

					c = mColormap[FIRST_INDEX + k];
					R1 = c.red();
					G1 = c.green();
					B1 = c.blue();
					
					c = mColormap[FIRST_INDEX + k+nDif];
					R2 = c.red();
					G2 = c.green();
					B2 = c.blue();
					
					float factor = (float)i / nDif;
					R = R1 + int((R2 - R1)*factor);
					G = G1 + int((G2 - G1)*factor);
					B = B1 + int((B2 - B1)*factor);
							
					mColormap[FIRST_INDEX+k+i] = QColor(R,G,B);
				}
			}
		}
	}

}

// END


