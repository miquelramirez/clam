#ifndef __TIMESEGMENTLABELSGROUP__
#define __TIMESEGMENTLABELSGROUP__

#include <qwidget.h>
#include "MediaTime.hxx"

class QLabel;

namespace CLAM
{
	namespace VM
	{
		class TimeSegmentLabelsGroup : public QWidget
		{
			public:
				TimeSegmentLabelsGroup(QWidget* parent=0);
				virtual ~TimeSegmentLabelsGroup();
				
				void UpdateLabels(MediaTime time);
			
			private:
				QLabel *_beginTimeLab, *_endTimeLab, *_durTimeLab;

				void InitLabelsGroup();
				QString GetText(const TData& value);
				
		};
	}
}

#endif
