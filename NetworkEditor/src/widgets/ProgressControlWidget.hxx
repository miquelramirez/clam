#ifndef __ProgressControlWidget_hxx__
#define __ProgressControlWidget_hxx__

#include <CLAM/Processing.hxx>
#include <QtGui/QWidget>
#include <QtGui/QSlider>
#include <QtDesigner/QDesignerExportWidget>

class QDESIGNER_WIDGET_EXPORT ProgressControlWidget : public QSlider
{
	Q_OBJECT
public:
	ProgressControlWidget(CLAM::Processing *processing = 0, QWidget *parent = 0);
	~ProgressControlWidget();

	CLAM::Processing* GetProcessing();
	void SetProcessing(CLAM::Processing *processing);
	
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
