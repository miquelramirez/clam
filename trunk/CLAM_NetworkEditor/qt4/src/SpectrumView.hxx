#ifndef SpectrumView_hxx
#define SpectrumView_hxx

#include <QtOpenGL/QGLWidget>
#include <QtGui/QLabel>
#include <CLAM/Processing.hxx>
#include <CLAM/PortMonitor.hxx>
#include <CLAM/DataTypes.hxx>
#include <CLAM/SpecTypeFlags.hxx>


class SpectrumViewWidget : public QWidget
{
	enum Dimensions {
	};
public:
	SpectrumViewWidget(CLAM::Processing * processing, QWidget * parent=0)
		: QWidget(parent)
		, _monitor(dynamic_cast<CLAM::PortMonitor<CLAM::Spectrum>*>(processing))
	{
		startTimer(50);
	}
	void paintEvent(QPaintEvent * event)
	{
		if (not _monitor) return;
		CLAM::Spectrum spectrum = _monitor->FreezeAndGetData();
		_monitor->UnfreezeData();
		if (not spectrum.HasMagBuffer())
		{
			CLAM::SpecTypeFlags flags;
			flags.bMagPhase=true;
			spectrum.SetType(flags);
			spectrum.SetTypeSynchronize(flags);
		}
		const CLAM::Array<CLAM::TData> & data = spectrum.GetMagBuffer();
		int size = data.Size();

		QPolygonF _line;
		QPainter painter(this);
		painter.scale(width(),height()/7.0);
		painter.setPen(Qt::black);
		for (int i=0; i<size; i++)
			_line << QPointF(double(i)/size, -std::log10(data[i]));
		painter.drawPolyline(_line);
	}
	void timerEvent(QTimerEvent *event)
	{
		if (not _monitor) return;
		if (_monitor->GetExecState() != CLAM::Processing::Running)
			return;
		update();
	}
private:
	CLAM::PortMonitor<CLAM::Spectrum> * _monitor;
	std::vector<CLAM::TData> _data;
};



#endif//SpectrumView_hxx
