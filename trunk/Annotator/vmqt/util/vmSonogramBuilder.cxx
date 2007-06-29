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

#include <numeric>
#include "CLAM_Math.hxx"
#include "SpecTypeFlags.hxx"
#include "vmSonogramBuilder.hxx"

namespace CLAM
{
	namespace VM
	{
		SonogramBuilder::SonogramBuilder()
			: mPalette(0.0f)
		{
		}

		SonogramBuilder::~SonogramBuilder()
		{
		}

		void SonogramBuilder::MakeSonogram(const Array<Spectrum>& data_in,
										   std::vector<std::vector<float> >& data_out,
										   std::vector<std::vector<QColor> >& color_sonogram_out,
										   std::vector<std::vector<QColor> >& blackwhite_sonogram_out)
		{
			Array<Spectrum> data = data_in;
			
			// adap spectral data
			SpecTypeFlags spFlags;
			data[0].GetType(spFlags);

			if(!spFlags.bMagPhase)
			{
				SpecTypeFlags newFlags;
				for(TIndex i=0; i < data.Size(); i++) data[i].SetTypeSynchronize(newFlags);
			}
			for(TIndex i=0; i < data.Size(); i++) data[i].ToDB();

			// compute data
			double dx = 1.0;
			double dy = 1.0;

			int nSpectrums = data.Size();
			int spectrSize = data[0].GetMagBuffer().Size();

			data_out.clear();

			if(nSpectrums > MaxSpectrums)
			{
				dx = double(nSpectrums)/double(MaxSpectrums);
				nSpectrums = MaxSpectrums;
			}

			if(spectrSize > MaxSpectrumSize)
			{
				dy = double(spectrSize)/double(MaxSpectrumSize);
				spectrSize = MaxSpectrumSize;
			}

			data_out.resize(nSpectrums);
			for(unsigned i=0; i < data_out.size(); i++) data_out[i].resize(spectrSize);
	    
			double row_diff = dx - double(int(dx));
			double col_diff = dy - double(int(dy));

			double row_accum = 0.0;
			double col_accum = 0.0;

			int total_spectrums = data.Size();
			int original_spectrum_size = data[0].GetMagBuffer().Size();

			TIndex row_step = int(dx);
			TIndex col_step = int(dy);
			TIndex i_step = row_step;
			TIndex j_step = col_step;
			int n, m;
			TIndex i,j;
			for(i=0, n=0; i < total_spectrums; i+=i_step, n++)
			{
				for(j=0, m=0; j < original_spectrum_size; j+=j_step, m++)
				{
					data_out[n][m] = MatrixBlockMean(data,i,i+row_step,j,j+col_step);
					if(m == spectrSize-1)
					{
						j_step = original_spectrum_size-j;
					}
					else
					{
						col_accum += col_diff;
						if(col_accum >= 1.0)
						{
							j_step = col_step+1;
							col_accum -= 1.0;
						}
						else
						{
							j_step = col_step;
						}
					}
				}
				if(n == nSpectrums-1)
				{
					i_step = total_spectrums-i;
				}
				else
				{
					row_accum += row_diff;
					if(row_accum >= 1.0)
					{
						i_step = row_step+1;
						row_accum -= 1.0;
					}
					else
					{
						i_step = row_step;
					}
				}
			}

			// colorize
			color_sonogram_out.clear();
			blackwhite_sonogram_out.clear();

			unsigned dataSize = data_out.size();
			unsigned specSize = data_out[0].size();
	 
			color_sonogram_out.resize(dataSize);
			blackwhite_sonogram_out.resize(dataSize);
			
			std::vector<QColor> color_vector(specSize);
			std::vector<QColor> blackwhite_vector(specSize);

			for(i=0; i < TIndex(dataSize); i++)
			{
				for(j = 0; j < TIndex(specSize); j++)
				{
					float value = ClampToRange(data_out[i][j]);
					// cplor
					int colorIndex = mPalette.Get(value);
					mPalette.GetRGBFromIndex( colorIndex, color_vector[j]);
					// B&W
					value = fabs(value-1.0f);
					blackwhite_vector[j].setRgbF(value,value,value);
				}
				color_sonogram_out[i]=color_vector;
				blackwhite_sonogram_out[i]=blackwhite_vector;
			}
		}

		std::vector<QColor> SonogramBuilder::GetColorScale(int w)
		{
			std::vector<QColor> scale(w);
			float step = -150.f/float(w);
			for(unsigned i=0; i < scale.size(); i++)
			{
				TIndex colorIndex = mPalette.Get( ClampToRange(float(i)*step));
				mPalette.GetRGBFromIndex( colorIndex, scale[i]);
			}
			return scale;
		}

		std::vector<QColor> SonogramBuilder::GetGrayScale(int w)
		{
			std::vector<QColor> scale(w);
			float step = -150.0f/float(w);
			for(unsigned i=0; i < scale.size(); i++)
			{
				float value = ClampToRange(float(i)*step);
				value = fabs(value-1.0f);
				scale[i].setRgbF(value,value,value);
			}
			return scale;
		}

		float SonogramBuilder::MatrixBlockMean(const Array<Spectrum>& specMtx, 
											   unsigned firstRow, unsigned lastRow, 
											   unsigned  firstCol, unsigned  lastCol)
		{
			float sum = 0.0f;
			for(unsigned i=firstRow; i < lastRow; i++)
			{
				sum = std::accumulate(specMtx[i].GetMagBuffer().GetPtr()+firstCol,
									  specMtx[i].GetMagBuffer().GetPtr()+lastCol,
									  sum);
			}
			return sum/float((lastRow-firstRow)*(lastCol-firstCol));
		}

		float SonogramBuilder::ClampToRange(float value) 
		{
			if(value >= 0) return 1.0f; // 0 = max
			if(value <= -150.0) return 0.0f; // -150 = min
			value += 150.0f;
			value /= 150.0f;
			return value;
		}
	    
	}
}

// END

