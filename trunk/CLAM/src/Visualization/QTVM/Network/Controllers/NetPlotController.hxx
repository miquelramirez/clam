#ifndef __NETPLOTCONTROLLER__
#define __NETPLOTCONTROLLER__

#include <qobject.h>
#include "DataTypes.hxx"
#include "Slotv0.hxx"

namespace CLAM
{
	namespace VM
	{
		typedef struct
		{
			float left;
			float right;
			float bottom;
			float top;
		} SView;

		class NetPlotController : public QObject
		{
			Q_OBJECT

			public:
				NetPlotController();
				virtual ~NetPlotController();

				virtual void Draw() = 0;

			signals:
				void sendView(SView);

			protected:
				SView _view;

                                SigSlot::Slotv0 mStartSlot;
	                        SigSlot::Slotv0 mStopSlot;

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
				TSize _nSamples;
				TData _vmin;
                 		TData _vmax;
		                bool _first;
		                bool _monitorIsRunning;
		                bool _canSendData;
		                bool _canGetData;
		                
				void InitView();
				
		};
	}
}

#endif

