#ifndef __VMQT_LOCATOR_RENDERER_H__
#define __VMQT_LOCATOR_RENDERER_H__

#include "vmRenderer2D.hxx"

namespace CLAM
{
	namespace VM
	{
		class Locator : public Renderer2D
		{
			Q_OBJECT
		public:
			Locator();
			~Locator();

			void SetLocatorColor(const Color& c);
			void SetRegionColor(const Color& c);

		signals:
			void selectedRegion(double,double);
			void regionChanged(double, double, bool);
			
		public slots:
		    void updateLocator(double);
			void updateLocator(double, bool);
			void updateRegion(double, double, bool);

		protected:
			void Render();
			void SetXRange(double xmin, double xmax);
			void MousePressEvent(double x, double y);
			void MouseReleaseEvent(double x, double y);
			void MouseMoveEvent(double x, double y);
			void KeyPressEvent(int key);
			void KeyReleaseEvent(int key);
			void LeaveEvent();
			
		private:
			bool   mUpdatingLocator;
			double mLocatorPos;
			Range  mRegion;
			Color  mLocatorColor;
			Color  mRegionColor;

			enum { TOLERANCE=3 };
			enum EditionMode 
			{
				Idle,
				DraggingLocator,
				DraggingBeginRegion,
				DraggingEndRegion
			};

			EditionMode mEditionMode;

			void DrawLocator();
			void DrawRegion(int mode);
			int DrawRegionMode();
			bool PickBeginRegion(double x, double tolerance);
			bool PickEndRegion(double x, double tolerance);
		};
	}
}

#endif

