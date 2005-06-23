#ifndef __NETPLOTCONTROLLER__
#define __NETPLOTCONTROLLER__

#include <list>
#include <qobject.h>
#include "DataTypes.hxx"
#include "Slotv0.hxx"
#include "Point.hxx"

namespace CLAM
{
    namespace VM
    {
		struct SView
		{
			float left;
			float right;
			float bottom;
			float top;

			SView();
			SView(float, float, float, float);
			SView(const SView& v);
			~SView();
	    
			void operator=(SView v);
		};

		class NetPlotController : public QObject
		{
			typedef std::list<SView> ViewStack;

			Q_OBJECT
	    
		public:
			NetPlotController();
			virtual ~NetPlotController();

			virtual void Draw();

			void SetPoint(const TData& x, const TData& y);
			virtual void UpdatePoint(const TData& x, const TData& y);

			void SetLeftButtonPressed(bool pressed);
			void SetRightButtonPressed(bool pressed);

		signals:
			void sendView(SView);
			void toolTip(QString);

		protected:
			SView mView;

			SigSlot::Slotv0 mStartSlot;
			SigSlot::Slotv0 mStopSlot;

			virtual void FullView() = 0;

			void SetnSamples(const TSize& nSamples);
			TSize GetnSamples() const;

			void SetvRange(const TData& vmin, const TData& vmax);
			TData GetvMin() const;
			TData GetvMax() const;

			void ConcreteStartMonitor();
			void ConcreteStopMonitor();

			bool MonitorIsRunning();

			void SetFirst(bool first);
			bool First();

			void SetCanGetData(bool canget);
			void SetCanSendData(bool cansend);
			bool CanGetData();
			bool CanSendData();
				
		private:
			TSize     mSamples;
			TData     mVMin;
            TData     mVMax;
			bool      mFirst;
			bool      mMonitorIsRunning;
			bool      mCanGetData;
			bool      mCanSendData;
			bool      mLeftButtonPressed;
			bool      mRightButtonPressed;
			Point     mCorners[2];
			ViewStack mViewStack;
	                
			void PushView();
			void PopView();

			float XMax();
			float XMin();
			float YMax();
			float YMin();

			float Max(float a, float b);
			float Min(float a, float b);

			TData AdjustX(const TData& value);
			TData AdjustY(const TData& value);
				
		};
    }
}

#endif

