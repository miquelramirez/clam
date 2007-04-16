/*
 * Copyright (c) 2001-2004 MUSIC TECHNOLOGY GROUP (MTG)
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

#include "SpectrumRasterizer.hxx"
#include "Pixel.hxx"
using namespace CLAMGUI;

SpectrumRasterizer* SpectrumRasterizer::mInstance = NULL;


void SpectrumRasterizer::BuildLUT()
{
	int	 startColorIdx;
	int	 endColorIdx;
	int	 curridx = 0;
	int	 segmentNum = mPath.Size() -1;
	int*	 segmentSizes;

	segmentSizes = new int[segmentNum];

	DetermineColorsPerSegment(segmentSizes, segmentNum);

	startColorIdx = 0;

	for (int k = 0; k < segmentNum ; k++ )
	{
		endColorIdx = startColorIdx+1;

		BuildSegment(curridx, segmentSizes[k], mPath[startColorIdx], mPath[endColorIdx]);

		startColorIdx = endColorIdx;
		curridx = curridx + segmentSizes[k];
	}

	delete segmentSizes;
}

void SpectrumRasterizer::DetermineColorsPerSegment(int* segmentSizes, int segmentNum)
{
	int idx;
	int delta = mLUTSize / segmentNum;

	for (idx = 0; idx < segmentNum; idx++ )
	{
		segmentSizes[idx] = delta;
	}		


	if ( mLUTSize % segmentNum!=0 )
	{
		int residue = mLUTSize - ( delta*segmentNum );

		idx = 0;
		while (residue>0)
		{
			segmentSizes[idx] = segmentSizes[idx] + 1;
			residue--;

			if (idx == segmentNum)
			{
				idx = 0;
			}
			else
				idx++;
		}
	}

}

void SpectrumRasterizer::BuildSegment(int LUTstartIdx, int segmentSize, const Pixel& StartColor, const Pixel& EndColor)
{
	CLAM_ASSERT( (unsigned(segmentSize) > 0) && (unsigned(segmentSize) <= mLUTSize), "Whoops! That Segment Size is not very correct at all" );
	float r1,g1,b1;
	float r2,g2,b2;
	float r,g,b;

	unsigned char* colorbuff1 = StartColor.GetData();
	unsigned char* colorbuff2 = EndColor.GetData();

	r1 = static_cast<float> ( colorbuff1[0] ) / 255.0f;
	g1 = static_cast<float> ( colorbuff1[1] ) / 255.0f;
	b1 = static_cast<float> ( colorbuff1[2] ) / 255.0f;
	r2 = static_cast<float> ( colorbuff2[0] ) / 255.0f;
	g2 = static_cast<float> ( colorbuff2[1] ) / 255.0f;
	b2 = static_cast<float> ( colorbuff2[2] ) / 255.0f;



	mColorLUT[LUTstartIdx].r = colorbuff1[0];
	mColorLUT[LUTstartIdx].g = colorbuff1[1];
	mColorLUT[LUTstartIdx].b = colorbuff1[2];
	mColorLUT[LUTstartIdx + segmentSize - 1].r = colorbuff2[0];
	mColorLUT[LUTstartIdx + segmentSize - 1].g = colorbuff2[1];
	mColorLUT[LUTstartIdx + segmentSize - 1].b = colorbuff2[2];

	float step = 0.0f;

	unsigned int offset = LUTstartIdx + 1;

	for (int k = 0; k < segmentSize -1 ;  k++)
	{
		step = static_cast<float>(k)/static_cast<float>(segmentSize);
		r = r1 + (r2 - r1)*step;
		g = g1 + (g2 - g1)*step;
		b = b1 + (b2 - b1)*step;

		mColorLUT[offset + k].r = static_cast<unsigned char>(r*255.0);
		mColorLUT[offset + k].g = static_cast<unsigned char>(g*255.0);
		mColorLUT[offset + k].b = static_cast<unsigned char>(b*255.0);
	}

}

