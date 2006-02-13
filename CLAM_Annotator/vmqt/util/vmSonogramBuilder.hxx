#ifndef __VMQT_SONOGRAM_BUILDER_H__
#define __VMQT_SONOGRAM_BUILDER_H__

#include <vector>
#include "Spectrum.hxx"
#include "vmPalette.hxx"
#include "vmDataTypes.hxx"

namespace CLAM
{
	namespace VM
	{
		class SonogramBuilder
		{
		public:
			SonogramBuilder();
			~SonogramBuilder();

			void MakeSonogram(const Array<Spectrum>& data_in,
							  std::vector<std::vector<float> >& data_out,
							  std::vector<std::vector<Color> >& color_sonogram_out,
							  std::vector<std::vector<Color> >& blackwhite_sonogram_out);

			std::vector<Color> GetColorScale(int w);
			std::vector<Color> GetGrayScale(int w);

		private:
			Palette mPalette;

			enum { MaxSpectrums=256, MaxSpectrumSize=256};
			
			float MatrixBlockMean(const Array<Spectrum>& specMtx, 
								  unsigned firstRow, unsigned lastRow, 
								  unsigned  firstCol, unsigned  lastCol);

			float ClampToRange(float value);
		};
	}
}

#endif

