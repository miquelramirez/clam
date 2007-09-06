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


class SpectrumViewMonitor :
	public CLAM::PortMonitor<CLAM::Spectrum>,
	public CLAM::VM::FloatArrayDataSource
{
	const char* GetClassName() const { return "SpectrumView"; };
	std::string getLabel(unsigned bin) const
	{
		static std::string a;
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
			_spectrum.ToLinear();
		}
		const CLAM::Array<CLAM::TData> & data = _spectrum.GetMagBuffer();
		_size = data.Size();
		if (_size==0) return 0;
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
		return IsRunning();
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
	SpectrumView(QWidget * parent=0, CLAM::VM::FloatArrayDataSource * dataSource=0)
		: QWidget(parent)
		, _dataSource(0)
		, _lineColor(Qt::black)
	{
		setDataSource( dataSource ? *dataSource : dummySource());
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
		if (!data)
		{
			_dataSource->release();
			return;
		}
		int size = _dataSource->nBins();

		QPainter painter(this);
//		painter.setRenderHint(QPainter::Antialiasing);
		painter.scale(width(),height()/7.0);
		painter.setPen(_lineColor);
		QPolygonF _line;
		for (int i=0; i<size; i++)
		{
			double value=std::log10(data[i]/size);
			if (value!=value) continue;
			_line << QPointF(double(i)/size, -value);
		}
		_dataSource->release();
		painter.drawPolyline(_line);
	}
	void timerEvent(QTimerEvent *event)
	{
		if ( !_dataSource) return;
		if ( !_dataSource->isEnabled()) return;
		update();
	}
	QColor lineColor() const { return _lineColor; }
	void setLineColor(const QColor & color) { _lineColor = color; }
	CLAM::VM::FloatArrayDataSource & dummySource()
	{
		static CLAM::VM::DummyFloatArrayDataSource sDummy(257, dummyData());
		return sDummy;
	}
	CLAM::TData * dummyData()
	{
		// Sample data taken from the trumpet analysis
		static CLAM::TData data[] = {
			0.0009, 0.0011, 0.0015, 0.0016, 0.0105, 0.0181, 0.0143, 0.0063, 0.0068, 0.0193,
			0.0307, 0.0242, 0.0096, 0.0084, 0.0192, 0.0309, 0.0247, 0.0116, 0.0094, 0.0162,
			0.0316, 0.0244, 0.0019, 0.0075, 0.0083, 0.0199, 0.0173, 0.0110, 0.0108, 0.0029,
			0.0108, 0.0122, 0.0095, 0.0113, 0.0046, 0.0039, 0.0068, 0.0064, 0.0088, 0.0066,
			0.0219, 0.0327, 0.0177, 0.0013, 0.0054, 0.0026, 0.0017, 0.0026, 0.0034, 0.0031,
			0.0022, 0.0011, 0.0016, 0.0023, 0.0027, 0.0023, 0.0021, 0.0029, 0.0036, 0.0045,
			0.0183, 0.0313, 0.0225, 0.0052, 0.0023, 0.0018, 0.0009, 0.0007, 0.0014, 0.0011,
			0.0005, 0.0005, 0.0002, 0.0004, 0.0001, 0.0003, 0.0005, 0.0005, 0.0002, 0.0034,
			0.0142, 0.0236, 0.0182, 0.0061, 0.0005, 0.0003, 0.0004, 0.0004, 0.0005, 0.0005,
			0.0005, 0.0004, 0.0004, 0.0005, 0.0004, 0.0003, 0.0003, 0.0003, 0.0002, 0.0013,
			0.0080, 0.0191, 0.0205, 0.0099, 0.0016, 0.0002, 0.0002, 0.0002, 0.0001, 0.0001,
			0.0001, 0.0001, 8.e-05, 0.0001, 0.0002, 0.0001, 5.e-05, 0.0001, 2.e-05, 0.0001,
			0.0001, 0.0007, 0.0055, 0.0140, 0.0158, 0.0081, 0.0015, 0.0012, 0.0067, 0.0136,
			0.0127, 0.0054, 0.0008, 0.0001, 0.0001, 0.0001, 0.0002, 0.0002, 0.0002, 0.0002,
			0.0002, 0.0001, 0.0003, 0.0035, 0.0089, 0.0098, 0.0050, 0.0010, 0.0020, 0.0073,
			0.0108, 0.0074, 0.0022, 0.0003, 0.0001, 0.0001, 0.0001, 0.0001, 0.0001, 0.0001,
			0.0001, 0.0001, 0.0001, 0.0001, 0.0001, 0.0001, 9.e-05, 8.e-05, 0.0003, 0.0022,
			0.0059, 0.0071, 0.0040, 0.0010, 0.0002, 0.0001, 0.0001, 6.e-05, 9.e-05, 0.0001,
			0.0001, 0.0001, 0.0001, 6.e-05, 7.e-05, 6.e-05, 3.e-05, 2.e-05, 8.e-05, 0.0005,
			0.0021, 0.0036, 0.0029, 0.0011, 0.0002, 0.0001, 0.0001, 0.0001, 0.0001, 0.0001,
			9.e-05, 9.e-05, 9.e-05, 8.e-05, 5.e-05, 5.e-05, 6.e-05, 5.e-05, 4.e-05, 4.e-05,
			3.e-05, 1.e-05, 0.0004, 0.0015, 0.0024, 0.0017, 0.0005, 0.0001, 8.e-05, 9.e-05,
			6.e-05, 4.e-05, 4.e-05, 3.e-05, 6.e-05, 7.e-05, 5.e-05, 5.e-05, 6.e-05, 0.0001,
			0.0012, 0.0027, 0.0028, 0.0012, 0.0001, 2.e-05, 9.e-06, 1.e-05, 2.e-05, 2.e-05,
			1.e-05, 3.e-05, 3.e-05, 4.e-05, 5.e-05, 5.e-05, 4.e-05, 3.e-05, 2.e-05, 1.e-05,
			0.0002, 0.0014, 0.0028, 0.0025, 0.0010, 0.0001, 4.e-05,
		};
		return data;
	}
private:
	CLAM::VM::FloatArrayDataSource * _dataSource;
	QColor _lineColor;
};



#endif//SpectrumView_hxx

