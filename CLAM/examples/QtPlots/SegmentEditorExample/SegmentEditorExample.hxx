#include "audio_file_utils.hxx"
#include "QtAppWrapper.hxx"
#include "QtAudioPlot.hxx"
#include "DiscontinuousSegmentation.hxx"
#include "ContiguousSegmentation.hxx"
#include <qpushbutton.h>
#include <qdialog.h>
#include <qlayout.h>
#include <qhbox.h>
#include <qmessagebox.h>

class SegmentEditorExample : public QDialog
{
	CLAM::VM::QtAudioPlot * mPlot;
	CLAM::Audio & mAudio;
	CLAM::Segmentation * mSegmentation;
	Q_OBJECT
public:
	SegmentEditorExample(CLAM::Audio & audio)
		: mAudio(audio)
		, mSegmentation(0)
	{
		setGeometry(100,100,700,350);
		mPlot = new CLAM::VM::QtAudioPlot(this);
		mPlot->Label("Audio");
		mPlot->Geometry(100,100,700,350);
		mPlot->SetData(audio);
		mPlot->SetSegmentation(0);
		mPlot->SetBackgroundColor(CLAM::VM::VMColor::White());
		mPlot->SetForegroundColor(CLAM::VM::VMColor::Blue());
//		mPlot->Show();
		QHBox * buttons = new QHBox(this);
		QPushButton * buttonNull = new QPushButton("No segmentation",buttons);
		QPushButton * buttonUnsized = new QPushButton("Unsized",buttons);
		QPushButton * buttonContinuous = new QPushButton("Continuous",buttons);
		QPushButton * buttonDisontinuous = new QPushButton("Discontinuous",buttons);
		QPushButton * buttonOverlapping = new QPushButton("Overlapping",buttons);
		QVBoxLayout * box = new QVBoxLayout(this);
		box->addWidget(mPlot);
		box->addWidget(buttons);
		connect(buttonNull, SIGNAL(clicked()), this, SLOT(changeNone()));
		connect(buttonContinuous, SIGNAL(clicked()), this, SLOT(changeContinuous()));
		connect(buttonDisontinuous, SIGNAL(clicked()), this, SLOT(changeDiscontinuous()));
		connect(buttonOverlapping, SIGNAL(clicked()), this, SLOT(changeOverlapped()));
		connect(buttonUnsized, SIGNAL(clicked()), this, SLOT(changeUnsized()));
	}
	~SegmentEditorExample()
	{
		setSegmentation(0);
	}
	void setSegmentation(CLAM::Segmentation * segmentation)
	{
		if (mSegmentation) delete mSegmentation;
		mSegmentation = segmentation;
		mPlot->SetSegmentation(segmentation);
	}
public slots:
	void changeNone()
	{
		setSegmentation(0);
	}
	void changeContinuous()
	{
		CLAM::Segmentation* seg = new CLAM::ContiguousSegmentation(mAudio.GetSize());
		unsigned nSegments = 15;
		double step = double(mAudio.GetSize())/nSegments;
		// make some divisions
		for(unsigned i=1; i < nSegments; i++)
		{
			seg->insert(double(i)*step);
		}
		seg->current(2);
		setSegmentation(seg);
	}
	void changeUnsized()
	{
		QMessageBox::information(this,"TODO","Not yet implemented");
	}
	void changeDiscontinuous()
	{
		std::vector<double> limits;
		unsigned nSegments = 15;
		double step = double(mAudio.GetSize())/nSegments;
		for(unsigned i=1; i < nSegments; i++)
		{
			limits.push_back(double(i)*step);
		}
		CLAM::Segmentation* seg = new CLAM::DiscontinuousSegmentation(
				mAudio.GetSize(), limits.begin(), limits.end());
		seg->current(2);
		setSegmentation(seg);
	}
	void changeOverlapped()
	{
		QMessageBox::information(this,"TODO","Not yet implemented");
	}
};


// END

