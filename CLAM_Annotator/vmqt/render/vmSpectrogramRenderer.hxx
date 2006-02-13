#ifndef __VMQT_SPECTROGRAM_RENDERER_H__
#define __VMQT_SPECTROGRAM_RENDERER_H__

#include <vector>
#include "Spectrum.hxx"
#include "vmRenderer2D.hxx"

namespace CLAM
{
	namespace VM
	{
		class SpectrogramRenderer : public Renderer2D
		{
			Q_OBJECT
		public:
			SpectrogramRenderer();
			~SpectrogramRenderer();

			void SetData(const Array<Spectrum>& specMtx);

		signals:
			void fixedLabels(QString, QString);
			void labels(QString, QString, QString, QString);
			
		public slots:
			void colorSonogram();
			void blackWhiteSonogram();

		protected:
			void Render();
			void SetHBounds(double left, double right);
			void SetVBounds(double bottom, double top);
			void MouseMoveEvent(double x, double y);
			void LeaveEvent();

		private:
			int mglColorList;	
			int mglBlackWhiteList;
			int mColormap;

			std::vector<std::vector<float> > mComputedData;
			std::vector<std::vector<Color> > mColorData;
			std::vector<std::vector<Color> > mBlackWhiteData;

			GLView mLocalView;

			enum { COLOR_SONOGRAM=0, BW_SONOGRAM=1 };

			void DrawColorData();
			void DrawBlackWhiteData();
		};
	}
}

#endif
