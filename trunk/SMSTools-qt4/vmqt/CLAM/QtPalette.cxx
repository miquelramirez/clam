/*
 * Copyright (c) 2004 MUSIC TECHNOLOGY GROUP (MTG)
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
#include <CLAM/CLAM_Math.hxx>
#include <CLAM/QtPalette.hxx>

namespace CLAM
{
	namespace VM
	{

		const int QtPalette::NCOLORMAPPING = 8192;
		const int QtPalette::FIRST_INDEX = 64;
		const int QtPalette::NCOLORS = 64;

		QGLColormap QtPalette::mColorMap;

		int QtPalette::mInited=0;

		QtPalette::QtPalette(float v)
			: mColorMapping(0)
		{
			mColorMapping = new int[NCOLORMAPPING];

			InitColors();
			mValue=v;
			CalcColorMap();
		}

		QtPalette::~QtPalette()
		{
			if ( mColorMapping )
				delete [] mColorMapping;
		}

		void QtPalette::CalcColorMap(void)
		{
			//D/ Calculate the conversion color map, depending and the scaling value.
			int i;
			float v = 0.34-mValue/3.;
			for (i=0;i<NCOLORMAPPING;i++) 
			{
				mColorMapping[i]=int(pow(float(i)/float(NCOLORMAPPING),v)*float(NCOLORS));
			}
		}

		void QtPalette::InitColors(void)
		{
			//D/ Create the colors in the FLTK colormap
			if (mInited) return;

			mInited=1;
			
			int ColorsIndex[6];

			ColorsIndex[0] = 0;
			ColorsIndex[1] = 12;
			ColorsIndex[2] = 25;
			ColorsIndex[3] = 37;
			ColorsIndex[4] = 50;
			ColorsIndex[5] = 64;
			
			mColorMap.setEntry(FIRST_INDEX+ColorsIndex[0],QColor(0,0,0));
			mColorMap.setEntry(FIRST_INDEX+ColorsIndex[1],QColor(15,17,16));
			mColorMap.setEntry(FIRST_INDEX+ColorsIndex[2],QColor(80,100,153));
			mColorMap.setEntry(FIRST_INDEX+ColorsIndex[3],QColor(90,180,100));
			mColorMap.setEntry(FIRST_INDEX+ColorsIndex[4],QColor(224,224,44));
			mColorMap.setEntry(FIRST_INDEX+ColorsIndex[5],QColor(255,155,80));
			
			int n = 0;
			int nDif = 0;
			for(int k=0; k<NCOLORS; k+=nDif,n++)
			{
				nDif = ColorsIndex[n+1] - ColorsIndex[n];
				for(int i=1; i<nDif; i++)
				{
					TUInt16 R,G,B;
					TUInt16 R1,G1,B1;
					TUInt16 R2,G2,B2;

					QColor c;

					c = mColorMap.entryColor(FIRST_INDEX + k);
					R1 = (TUInt16)c.red();
					G1 = (TUInt16)c.green();
					B1 = (TUInt16)c.blue();

					c = mColorMap.entryColor(FIRST_INDEX + k+nDif);
					R2 = (TUInt16)c.red();
					G2 = (TUInt16)c.green();
					B2 = (TUInt16)c.blue();
					
							
					float factor = (float)i / nDif;
					R = R1 + TUInt16((R2 - R1)*factor);
					G = G1 + TUInt16((G2 - G1)*factor);
					B = B1 + TUInt16((B2 - B1)*factor);
							
					mColorMap.setEntry(FIRST_INDEX+k+i,QColor(int(R),int(G),int(B)));
				}
			}
		}
	}

}

// END


