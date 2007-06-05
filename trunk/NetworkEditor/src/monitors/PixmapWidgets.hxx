#include <QtGui/QPushButton>
#include <QtGui/QSlider>
#include <QtGui/QPixmap>
#include <QtGui/QWheelEvent>
#include <QtGui/QPaintEvent>
#include <QtGui/QMouseEvent>
#include <QtGui/QPainter>
#include <QtCore/QTimer>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <vector>

class PixmapCache
{
	typedef std::map <QString, QPixmap *> Cache;
public:
	QPixmap * GetPixmap(const QString & file)
	{
		Cache::iterator it = _pixmapMap.find(file);
		if (it!=_pixmapMap.end()) return it->second;

		QPixmap * pixmap = new QPixmap(file);
		if (!pixmap->isNull())
			_pixmapMap.insert(std::make_pair(file, pixmap));
		return pixmap;

	}
	int fillBitMaps(std::vector<QPixmap*> & vector, unsigned toRead, const QString & basePath, unsigned digits, const char * extension)
	{
		vector.clear();
		for (unsigned i=0; ; i++)
		{
			QPixmap * pixmap = GetPixmap( QString("%1%2%3")
				.arg(basePath)
				.arg(i,digits,10,QChar('0'))
				.arg(extension)
				);
			if (!pixmap) return i;
			if (pixmap->isNull()) return i;
			vector.push_back(pixmap);
		}
	}
	~PixmapCache()
	{
		for (Cache::iterator it=_pixmapMap.begin(); it!=_pixmapMap.end(); it++)
			delete it->second;
	}
	static PixmapCache & GetInstance()
	{
		static PixmapCache theInstance;
		return theInstance;
	}
private:
	Cache _pixmapMap;
};


class QCicleButton : public QWidget
{
	Q_OBJECT
public:
	QCicleButton(QWidget * parent=0) : QWidget(parent)
	{
		PixmapCache::GetInstance().fillBitMaps(_pixmaps, 9, getBasePath(), 3, ".png");
		setFixedSize(_pixmaps[0]->size());
		_current=0;
	}
	const char * getBasePath() const
	{
		return ":/pkwidgets/thinButton_";
	}
	void paintEvent(QPaintEvent * event)
	{
		QPainter paint(this);
		paint.drawPixmap(0,0,*_pixmaps[_current]);
	}
	void mouseReleaseEvent(QMouseEvent * event)
	{
		_current++;
		_current %= _pixmaps.size();
		update();
	}

private:
	unsigned _current;
	std::vector<QPixmap*> _pixmaps;
	QTimer _timer;
};

class QPixmapButton : public QAbstractButton
{
	Q_OBJECT
	Q_PROPERTY ( QString BasePath READ getBasePath WRITE setBasePath )
	Q_PROPERTY(int ButtonColor READ color WRITE setColor)
public:
	enum ButtonColor {red=0, green=1, purple=2};
	enum ButtonPos   {up=0, middle=1, down=2};
	enum ButtonAnim  {none=0, todown=1, toup=2};
	QPixmapButton(QWidget * parent=0, const char * basePath=":/pkwidgets/thinButton_") 
		: QAbstractButton(parent)
	{
		setBasePath(basePath);
		_position = up;
		_color = red;
		_animation = none;
		connect(&_timer, SIGNAL(timeout()), this, SLOT(nextAnimationFrame()));
		QPalette palette;
		palette.setBrush(backgroundRole(), QBrush(getPixmap()));
		setPalette(palette);
		setAutoFillBackground(true);

		startAnimation(toup);
	}
	void setColor(int color)
	{
		_color = _nColors? color%_nColors : 0;
		update();
	}
	int color() const
	{
		return _color;
	}
	const QString & getBasePath() const
	{
		return _basePath;
	}
	void setBasePath(const QString & path)
	{
		_basePath = path;
		int nImages = PixmapCache::GetInstance().fillBitMaps(_pixmaps, 9, getBasePath(), 3, ".png");
		setFixedSize(_pixmaps[0]->size());
		_nColors = nImages/3;
		setColor(_color);
		update();
	}
	void paintEvent(QPaintEvent * event)
	{
		if (!_animation) _position=isChecked()||isDown()?down:up;
		QPainter painter(this);
		if (_nColors)
			painter.drawPixmap(0,0,getPixmap());
		int off = _position*1;
		painter.setPen(Qt::black);
		QRect rect1(off-1, off-1, width()-1, height()-1);
		painter.drawText(rect1, Qt::AlignCenter, text());
		painter.setPen(Qt::white);
		QRect rect2(off, off, width(), height());
		painter.drawText(rect2, Qt::AlignCenter, text());

	}
	void mousePressEvent(QMouseEvent * event)
	{
		startAnimation(todown);
		QAbstractButton::mousePressEvent(event);
	}
	void mouseReleaseEvent(QMouseEvent * event)
	{
		startAnimation( (isCheckable() && !isChecked()) ? todown : toup);
		QAbstractButton::mouseReleaseEvent(event);
	}
	const QPixmap & getPixmap()
	{
		return *(_pixmaps[_position + _color*3]);
	}
	
	void startAnimation(ButtonAnim anim)
	{
		_animation=anim;
		_timer.setSingleShot(false);
		_timer.start(30);
	}
	void stopAnimation()
	{
		_animation=none;
		_timer.stop();
	}

public slots:
	void nextAnimationFrame()
	{
		if (_animation==toup)
		{
			if      (_position == down)   _position = middle;
			else if (_position == middle) _position = up;

			if (_position==up) stopAnimation();
		}
		if (_animation==todown)
		{
			if      (_position == up)     _position = middle;
			else if (_position == middle) _position = down;

			if (_position==down) stopAnimation();
		}
		update();
	}


private:
	enum ButtonAnim  _animation;
	enum ButtonPos   _position;
	int _color;
	std::vector<QPixmap*> _pixmaps;
	QTimer _timer;
	int _nColors;
	QString _basePath;
};

class QPixmapRange : public QAbstractSlider
{
	Q_OBJECT
	Q_PROPERTY ( QString ClamControlName READ getClamControlName WRITE setClamControlName )
	Q_PROPERTY ( QString BasePath READ getBasePath WRITE setBasePath )
public:
	QPixmapRange(const char * basePath=":/pkwidgets/thinMixerSlider_", QWidget*parent=0)
		: QAbstractSlider(parent)
	{
		setRange(0,127);
		setValue(0);
		setBasePath(basePath);
		setAutoFillBackground(true);
	}

	QString _clamControlName;
	const QString & getClamControlName() const { return _clamControlName; } 
	void setClamControlName(const QString & clamControlName) { _clamControlName = clamControlName; }

	const QString & getBasePath() const
	{
		return _basePath;
	}
	void setBasePath(const QString & path)
	{
		_basePath = path;
		_nFrames = PixmapCache::GetInstance().fillBitMaps(_pixmaps, 128, getBasePath(), 3, ".png");
		setFixedSize(_pixmaps[0]->size());
		setMinimumSize(_pixmaps[0]->size());
		sliderChange(SliderValueChange);
	}
	void sliderChange(SliderChange change)
	{
		if (change==SliderValueChange)
		{
			unsigned frame = (maximum()==minimum())? 0: 
				(_nFrames-1) * (value()-minimum()) / (maximum()-minimum());
			QPalette palette;
			palette.setBrush(backgroundRole(), QBrush(*_pixmaps[frame]));
			setPalette(palette);
		}
		QAbstractSlider::sliderChange(change);
	}
	void paintEvent(QPaintEvent * event)
	{
		// Nothing to do, it was just the background
	}
	void mousePressEvent(QMouseEvent * event)
	{
		if (event->button() == Qt::LeftButton)
		{
			_dragging=true;
			_lastX = event->x();
			_lastY = event->y();
		}
	}
	void mouseReleaseEvent(QMouseEvent * event)
	{
		if (event->button() == Qt::LeftButton)
			_dragging=false;
	}
	void mouseMoveEvent(QMouseEvent * event)
	{
		if (!_dragging) return;

		int newValue = value() - ((event->y() - _lastY) + (_lastX - event->x() ));
		_lastX = event->x();
		_lastY = event->y();
		setValue(newValue);
	}
	void wheelEvent(QWheelEvent * event)
	{
		if (event->delta() < 0 )
			setValue(value() - pageStep());
		else
			setValue(value() + pageStep());
	}
private:
	QString _basePath;
	std::vector<QPixmap*> _pixmaps;
	int _lastX, _lastY;
	bool _dragging;
	int _nFrames;

};

/**
 * @todo: Timer + Update method
 */
class QPeakMeter : public QPixmapRange
{
public:
	QPeakMeter(const char * basePath, QWidget*parent=0)
		: QPixmapRange(basePath, parent)
	{
	}
	void mousePressEvent(QMouseEvent * event)
	{
	}
	void mouseReleaseEvent(QMouseEvent * event)
	{
	}
	void mouseMoveEvent(QMouseEvent * event)
	{
	}
	void wheelEvent(QWheelEvent * event)
	{
	}
};

/**
 * @todo SevenSegments (LEDDigit)
 * @todo JogWheel (VerticalJogWheel)
 * @todo SevenSegmentsArray (LEDDigitDisplay)
 * @todo LED
 * 
 */




