#ifndef __ProgressControlWidget_hxx__
#define __ProgressControlWidget_hxx__

#include <CLAM/Processing.hxx>
#include <QtGui/QSlider>

class ProgressControlWidget : public QSlider
{
	Q_OBJECT
public:
	ProgressControlWidget(CLAM::Processing *processing);
	~ProgressControlWidget();

protected:
	void timerEvent(QTimerEvent *event);

private slots:
	void sliderValueChanged(int value);
	void sliderPressed();
	void sliderReleased();

private:
	CLAM::Processing *_processing;
	bool             _updating;
	bool             _jumping;
};

#endif //__ProgressControlWidget_hxx__
