#ifndef __NETPLOTCONTROLLER__
#define __NETPLOTCONTROLLER__

#include <qobject.h>
#include "DataTypes.hxx"

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
				void requestRefresh();
				void sendView(SView);

			protected:
				SView _view;

				void SetnSamples(const TSize& n);
				TSize GetnSamples() const;

				void SetvRange(const TData& vmin, const TData& vmax);
				TData GetvMin() const;
				TData GetvMax() const;
				
			private:
				TSize _nSamples;
				TData _vmin;
				TData _vmax;

				void InitView();
				
		};
	}
}

#endif

