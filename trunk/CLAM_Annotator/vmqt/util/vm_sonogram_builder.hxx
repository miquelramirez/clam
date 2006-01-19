#ifndef __VMQT_SONOGRAM_BUILDER_H__
#define __VMQT_SONOGRAM_BUILDER_H__

#include <vector>
#include "Spectrum.hxx"
#include "vm_palette.hxx"
#include "vm_data_types.hxx"

namespace CLAM
{
	namespace VM
	{
		class SonogramBuilder
		{
		public:
			SonogramBuilder();
			~SonogramBuilder();

			void make_sonogram(const CLAM::Array<CLAM::Spectrum>& data_in,
							   std::vector<std::vector<float> >& data_out,
							   std::vector<std::vector<CLAM::VM::Color> >& color_sonogram_out,
							   std::vector<std::vector<CLAM::VM::Color> >& blackwhite_sonogram_out);

			std::vector<CLAM::VM::Color> get_color_scale(int w);
			std::vector<CLAM::VM::Color> get_gray_scale(int w);

		private:
			CLAM::VM::Palette sb_palette;

			enum { MaxSpectrums=257, MaxSpectrumSize=257};
			
			float matrix_block_mean(const CLAM::Array<CLAM::Spectrum>& specMtx, 
									unsigned firstRow, unsigned lastRow, 
									unsigned  firstCol, unsigned  lastCol);

			float clamp_to_range(float value);
		};
	}
}

#endif

