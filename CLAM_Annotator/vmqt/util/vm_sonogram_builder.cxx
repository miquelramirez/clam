#include <numeric>
#include "CLAM_Math.hxx"
#include "SpecTypeFlags.hxx"
#include "vm_sonogram_builder.hxx"

namespace CLAM
{
	namespace VM
	{
		SonogramBuilder::SonogramBuilder()
			: sb_palette(0.0f)
		{
		}

		SonogramBuilder::~SonogramBuilder()
		{
		}

		void SonogramBuilder::make_sonogram(const Array<Spectrum>& data_in,
											std::vector<std::vector<float> >& data_out,
											std::vector<std::vector<Color> >& color_sonogram_out,
											std::vector<std::vector<Color> >& blackwhite_sonogram_out)
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
					data_out[n][m] = matrix_block_mean(data,i,i+row_step,j,j+col_step);
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
			
			std::vector<Color> color_vector(specSize);
			std::vector<Color> blackwhite_vector(specSize);

			for(i=0; i < TIndex(dataSize); i++)
			{
				for(j = 0; j < TIndex(specSize); j++)
				{
					Color c;
					float value = clamp_to_range(data_out[i][j]);
					// cplor
					int colorIndex = sb_palette.get(value);
					sb_palette.get_rgb_from_index( colorIndex, c.r, c.g, c.b);
					color_vector[j]=c;
					// B&W
					value = fabs(value-1.0f);
					c.r = c.g = c.b = int(255.0*value);
					blackwhite_vector[j]=c;
				}
				color_sonogram_out[i]=color_vector;
				blackwhite_sonogram_out[i]=blackwhite_vector;
			}
		}

		std::vector<Color> SonogramBuilder::get_color_scale(int w)
		{
			std::vector<Color> scale(w);
			float step = -150.f/float(w);
			for(unsigned i=0; i < scale.size(); i++)
			{
				Color c;
				TIndex colorIndex = sb_palette.get( clamp_to_range(float(i)*step));
				sb_palette.get_rgb_from_index( colorIndex, c.r, c.g, c.b);
				scale[i]=c;
			}
			return scale;
		}

		std::vector<Color> SonogramBuilder::get_gray_scale(int w)
		{
			std::vector<Color> scale(w);
			float step = -150.0f/float(w);
			for(unsigned i=0; i < scale.size(); i++)
			{
				Color c;
				float value = clamp_to_range(float(i)*step);
				value = fabs(value-1.0f);
				c.r = c.g = c.b = int(255.0*value);
				scale[i]=c;
			}
			return scale;
		}

		float SonogramBuilder::matrix_block_mean(const Array<Spectrum>& specMtx, 
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

		float SonogramBuilder::clamp_to_range(float value) 
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
