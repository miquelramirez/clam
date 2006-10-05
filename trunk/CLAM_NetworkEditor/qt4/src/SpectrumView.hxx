#ifndef SpectrumView_hxx
#define SpectrumView_hxx

#include <QtOpenGL/QGLWidget>
#include <QtGui/QLabel>
#include <QtGui/QPolygonF>
#include <QtGui/QPainter>
#include <CLAM/Processing.hxx>
#include <CLAM/PortMonitor.hxx>
#include <CLAM/DataTypes.hxx>
#include <CLAM/SpecTypeFlags.hxx>
#include <CLAM/Spectrum.hxx>
#include "FloatArrayDataSource.hxx"


//TODO move to a clam lib

class SpectrumViewMonitor : public CLAM::PortMonitor<CLAM::Spectrum>, public CLAM::VM::FloatArrayDataSource
{
	const char* GetClassName() const { return "SpectrumView"; };
	const std::string & getLabel(unsigned bin) const
	{
		static std::string a("A");
		return a;
	}
	const CLAM::TData * frameData()
	{
		_spectrum = FreezeAndGetData();
		if (not _spectrum.HasMagBuffer())
		{
			CLAM::SpecTypeFlags flags;
			flags.bMagPhase=true;
			_spectrum.SetType(flags);
			_spectrum.SetTypeSynchronize(flags);
		}
		const CLAM::Array<CLAM::TData> & data = _spectrum.GetMagBuffer();
		_size = data.Size();
		return &data[0];
	}
	void release()
	{
		UnfreezeData();
	}
	unsigned nBins() const
	{
		return _size;
	}
	bool isEnabled() const
	{
		return GetExecState() == CLAM::Processing::Running;
	}
private:
	unsigned _size;
	CLAM::Spectrum _spectrum;
};


class SpectrumView : public QWidget
{
	enum Dimensions {
	};
public:
	SpectrumView(QWidget * parent=0)
		: QWidget(parent)
		, _dataSource(0)
	{
		startTimer(50);
	}
	void setDataSource( CLAM::VM::FloatArrayDataSource & dataSource)
	{
		_dataSource = &dataSource;
	}
	void paintEvent(QPaintEvent * event)
	{
		if (not _dataSource) return;
		const CLAM::TData * data = _dataSource->frameData();
		int size = _dataSource->nBins();

		QPolygonF _line;
		QPainter painter(this);
		painter.scale(width(),height()/7.0);
		painter.setPen(Qt::black);
		for (int i=0; i<size; i++)
			_line << QPointF(double(i)/size, -std::log10(data[i]));
		_dataSource->release();
		painter.drawPolyline(_line);
	}
	void timerEvent(QTimerEvent *event)
	{
		if (not _dataSource) return;
		if (not _dataSource->isEnabled()) return;
		update();
	}
private:
	CLAM::VM::FloatArrayDataSource * _dataSource;
};



#endif//SpectrumView_hxx
