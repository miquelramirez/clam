#ifndef ControlSurfaceWidget_hxx
#define ControlSurfaceWidget_hxx

#include <cmath>
#include "ControlSurface.hxx"
#include <QtGui/QWidget>

class QLabel;
class QDoubleSpinBox;

class ControlSurfaceWidget : public QWidget
{
	Q_OBJECT
	Q_PROPERTY(bool spinBoxesHidden READ spinBoxesHidden WRITE hideSpinBoxes)
	Q_PROPERTY(QColor pointBrushColor READ pointBrushColor WRITE setPointBrushColor)
	Q_PROPERTY(QColor pointPenColor READ pointPenColor WRITE setPointPenColor)
public:
	ControlSurfaceWidget(CLAM::Processing * processing);
	~ControlSurfaceWidget();
	void paintEvent(QPaintEvent * event); 
	void mousePressEvent(QMouseEvent * event); 
	void mouseMoveEvent(QMouseEvent * event);
	void mouseReleaseEvent(QMouseEvent * event) ;
	void hideSpinBoxes(bool hide);
	bool spinBoxesHidden();
	const QColor & pointBrushColor() const { return _pointBrushColor; }
	void setPointBrushColor(const QColor & color) { _pointBrushColor=color; }
	const QColor & pointPenColor() const { return _pointPenColor; }
	void setPointPenColor(const QColor & color) { _pointPenColor=color; }

private slots:
	void moveSurface(int posX, int posY);
	void spinBoxChanged();
signals:
	void surfaceMoved(int posX, int posY);

private:
	double mapX(int x) const;
	double mapY(int y) const;
	enum { pointSize=10 };
	QLabel * _surface;
	QDoubleSpinBox * _spinBoxX;
	QDoubleSpinBox * _spinBoxY;
	CLAM::ControlSurface * _sender;
	double _minX;
	double _defaultX;
	double _maxX;
	double _stepX;
	double _minY;
	double _defaultY;
	double _maxY;
	double _stepY;
	bool _updating;
	bool _dragging;
	bool _areSpinBoxesHidden;
	QColor _pointPenColor;
	QColor _pointBrushColor;
};

#endif//ControlSurfaceWidget_hxx

