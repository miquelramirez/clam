#ifndef __VMQT_MULTIDISPLAY_PLOT_H__
#define __VMQT_MULTIDISPLATPLOT_H__

#include <vector>
#include "vmWPlot.hxx"

namespace CLAM
{
	namespace VM
	{
		class Ruler;
		class Plot2D;
		class ScrollGroup;

		class MultiDisplayPlot : public WPlot
		{
			Q_OBJECT
		public:
			MultiDisplayPlot(QWidget* parent=0);
			virtual ~MultiDisplayPlot();

		protected:
			Ruler*       mXRuler;
			ScrollGroup* mHScroll;

			std::vector<Plot2D*> mDisplay;

			virtual void CreateDisplay()=0;

			void InitMultiDisplayPlot(int master_id);
			void SetMasterId(int id);

		private slots:
			void setMaxHScroll(int);

		private:
			int mMasterId;
		};
	}
}

#endif

