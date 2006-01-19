#ifndef __VMQT_SPECTROGRAM_RENDERER_H__
#define __VMQT_SPECTROGRAM_RENDERER_H__

#include <vector>
#include "Spectrum.hxx"
#include "vm_renderer2d.hxx"

namespace CLAM
{
	namespace VM
	{
		class SpectrogramRenderer : public CLAM::VM::Renderer2D
		{
			Q_OBJECT
		public:
			SpectrogramRenderer();
			~SpectrogramRenderer();

			void set_data(const CLAM::Array<CLAM::Spectrum>& specMtx);

		signals:
			void fixedLabels(QString, QString);
			void labels(QString, QString, QString, QString);
			
		public slots:
			void colorSonogram();
			void blackWhiteSonogram();

		protected:
			void render();
			void set_hbounds(double left, double right);
			void set_vbounds(double bottom, double top);
			void mouse_move_event(double x, double y);
			void leave_event();

		private:
			int rd_glColorList;	
			int rd_glBlackWhiteList;
			int rd_colormap;

			std::vector<std::vector<float> >           rd_computed_data;
			std::vector<std::vector<CLAM::VM::Color> > rd_color_data;
			std::vector<std::vector<CLAM::VM::Color> > rd_blackwhite_data;

			CLAM::VM::GLView      rd_local_view;

			enum { COLOR_SONOGRAM=0, BW_SONOGRAM=1 };

			void draw_color_data();
			void draw_blackwhite_data();

			int get_glList();
		};
	}
}

#endif
