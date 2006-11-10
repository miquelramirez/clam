#ifndef SpectrumView_hxx
#define SpectrumView_hxx

#include <QtOpenGL/QGLWidget>
#undef GetClassName
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
		if ( !_spectrum.HasMagBuffer())
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

#include <QtDesigner/QDesignerExportWidget>

class QDESIGNER_WIDGET_EXPORT SpectrumView : public QWidget
{
	Q_OBJECT
	Q_PROPERTY(QColor lineColor READ lineColor WRITE setLineColor)
	enum Dimensions {
	};
public:
	SpectrumView(QWidget * parent=0)
		: QWidget(parent)
		, _dataSource(0)
		, _lineColor(Qt::black)
	{
		startTimer(50);
	}
	void setDataSource( CLAM::VM::FloatArrayDataSource & dataSource)
	{
		_dataSource = &dataSource;
	}
	void paintEvent(QPaintEvent * event)
	{
		if ( !_dataSource) return;
		const CLAM::TData * data = _dataSource->frameData();
		int size = _dataSource->nBins();

		QPolygonF _line;
		QPainter painter(this);
		painter.scale(width(),height()/5.0);
		painter.setPen(_lineColor);
		for (int i=0; i<size; i++)
			_line << QPointF(double(i)/size, -std::log10(data[i]));
		_dataSource->release();
		painter.drawPolyline(_line);
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
	CLAM::VM::FloatArrayDataSource * _dataSource;
	QColor _lineColor;
};



#endif//SpectrumView_hxx
