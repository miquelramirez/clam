#ifndef SegmentationView_hxx
#define SegmentationView_hxx


#include "SegmentationDataSource.hxx"

#include <QtGui/QLabel>
#include <QtGui/QPainter>
#include <CLAM/Processing.hxx>
#include <CLAM/PortMonitor.hxx>
#include <QtDesigner/QDesignerExportWidget>


class QDESIGNER_WIDGET_EXPORT SegmentationView : public QWidget
{
	Q_OBJECT
	Q_PROPERTY(QColor lineColor READ lineColor WRITE setLineColor)
	Q_PROPERTY(QColor pointColor READ pointColor WRITE setPointColor)
	enum Dimensions {
	};
public:
	SegmentationView(QWidget * parent=0, CLAM::VM::SegmentationDataSource * dataSource=0)
		: QWidget(parent)
		, _dataSource(dataSource)
		, _lineColor(Qt::green)
		, _pointColor(Qt::white)
	{
		startTimer(50);
	}
	void paintEvent(QPaintEvent * event)
	{
		const unsigned margin = 4;
		if ( !_dataSource) return;
		QVector<QRectF> segmentBoxes;
		const CLAM::Segmentation & segmentation = _dataSource->frameData();
		for (unsigned i=0; i<segmentation.onsets().size(); i++)
		{
//			double maxTime = segmentation.maxPosition();
			double maxTime = segmentation.offsets().back();
			double minTime = maxTime - 20.;
			if (maxTime<1) maxTime=1;
			float onsetPosition = width()*(segmentation.onsets()[i]-minTime)/(maxTime-minTime);
			float offsetPosition = width()*(segmentation.offsets()[i]-minTime)/(maxTime-minTime);
			segmentBoxes << QRectF(onsetPosition,margin,offsetPosition-onsetPosition,height()-2*margin);
		}
		QPainter painter(this);
		painter.setPen(QPen(_lineColor,3));
		painter.setBrush(_pointColor);
		painter.drawRects(segmentBoxes);
		painter.setPen(Qt::black);
		for (unsigned i=0; i<segmentation.onsets().size(); i++)
		{
			painter.drawText(segmentBoxes[i], Qt::AlignCenter|Qt::TextWordWrap, segmentation.labels()[i].c_str());
		}
		_dataSource->release();
	}

	void setDataSource(CLAM::VM::SegmentationDataSource & dataSource)
	{
		_dataSource = & dataSource;
	}
	void setPointColor(const QColor & color)
	{
		_pointColor = color;
	}
	QColor pointColor() const
	{
		return _pointColor;
	}
	void setLineColor(const QColor & color)
	{
		_lineColor = color;
	}
	QColor lineColor() const
	{
		return _lineColor;
	}
	void timerEvent(QTimerEvent *event)
	{
		if ( !_dataSource) return;
		if ( !_dataSource->isEnabled()) return;
		update();
	}
private:
	CLAM::VM::SegmentationDataSource * _dataSource;
	QColor _lineColor;
	QColor _pointColor;
};


#endif//SegmentationView_hxx
