#ifndef Vumeter_hxx
#define Vumeter_hxx

#include "Oscilloscope.hxx"
class VumeterMonitor : public OscilloscopeMonitor
{
	const char* GetClassName() const { return "Vumeter"; };
};

#include <QtOpenGL/QGLWidget>
#undef GetClassName
#include <CLAM/DataTypes.hxx>
#include <QtDesigner/QDesignerExportWidget>

class QDESIGNER_WIDGET_EXPORT Vumeter : public QWidget
{
	Q_OBJECT
	Q_PROPERTY(int ledHeight READ ledHeight WRITE setLedHeight)
	Q_PROPERTY(int interLedGap READ interLedGap WRITE setInterLedGap)
	Q_PROPERTY(int peakMemory READ peakMemory WRITE setPeakMemory)
	Q_PROPERTY(int peakDecay READ peakDecay WRITE setPeakDecay)
	Q_PROPERTY(QColor color0 READ color0 WRITE setColor0)
	Q_PROPERTY(QColor color1 READ color1 WRITE setColor1)
	Q_PROPERTY(QColor color2 READ color2 WRITE setColor2)
	Q_PROPERTY(QColor color3 READ color3 WRITE setColor3)
	Q_PROPERTY(QBrush outlineBrush READ outlineBrush WRITE setOutlineBrush)
	enum Dimensions {
		margin=4,
	};
public:
	Vumeter(QWidget * parent=0, CLAM::VM::FloatArrayDataSource * dataSource=0)
		: QWidget(parent)
		, _dataSource( dataSource)
		, _memorizedPeak(0)
		, _remainingPeakMemory(0)
		, _ledHeight(10)
		, _interLedGap(4)
		, _peakMemory(10)
		, _peakDecay(5)
		, _color0(Qt::green)
		, _color1(Qt::yellow)
		, _color2(QColor(0xff,0x77,0x00))
		, _color3(Qt::red)
	{
		setDataSource(dataSource ? *dataSource : Oscilloscope::dummySource());
		startTimer(50);
	}
	void paintEvent(QPaintEvent * event);
	QSize minimumSizeHint() const
	{
		return QSize(2*margin+10,2*margin+2*_ledHeight);
	}
	void setDataSource(CLAM::VM::FloatArrayDataSource & source)
	{
		_dataSource = &source;
	}
	void timerEvent(QTimerEvent *event)
	{
		if ( !_dataSource) return;
		if ( !_dataSource->isEnabled()) return;
		update();
	}
	int ledHeight() const { return _ledHeight;}
	void setLedHeight(int height) { _ledHeight=height;}
	int interLedGap() const { return _interLedGap;}
	void setInterLedGap(int gap) { _interLedGap=gap;}
	int peakMemory() const { return _peakMemory;}
	void setPeakMemory(int memory) { _peakMemory=memory;}
	int peakDecay() const { return _peakDecay;}
	void setPeakDecay(int decay) { _peakDecay=decay>1?decay:1;}
	const QColor & color0() const {return _color0;}
	void setColor0(const QColor & color) { _color0=color; }
	const QColor & color1() const {return _color1;}
	void setColor1(const QColor & color) { _color1=color; }
	const QColor & color2() const {return _color2;}
	void setColor2(const QColor & color) { _color2=color; }
	const QColor & color3() const {return _color3;}
	void setColor3(const QColor & color) { _color3=color; }
	const QBrush & outlineBrush() const {return _oulineBrush;}
	void setOutlineBrush(const QBrush & brush) { _oulineBrush=brush; }
private:
	double energy();
	CLAM::VM::FloatArrayDataSource * _dataSource;
	double _memorizedPeak;
	int _remainingPeakMemory;
	int _ledHeight;
	int _interLedGap;
	int _peakMemory;
	int _peakDecay;
	QColor _color0;
	QColor _color1;
	QColor _color2;
	QColor _color3;
	QBrush _oulineBrush;
};



#endif//Vumeter_hxx

