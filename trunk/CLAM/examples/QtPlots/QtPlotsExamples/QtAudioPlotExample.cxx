#include <qmessagebox.h>
#include "Audio.hxx"
#include "AudioFileLoader.hxx"
#include "QtAudioPlot.hxx"
#include "QtAppWrapper.hxx"

using CLAM::VM::QtAudioPlot;

int main()
{
	AudioFileLoader fLoader;
	Audio audio;

	CLAM::VM::QtAppWrapper::Init();

	int err = fLoader.Load("../../data/birthday.wav",audio);
	if(err)
	{
		QMessageBox message("File Error",
							"Unable to open \'birthday.wav\'",
							QMessageBox::Critical,
							QMessageBox::Ok,
							QMessageBox::NoButton,
							QMessageBox::NoButton); 
		message.exec();
		CLAM::VM::QtAppWrapper::Quit();
		return 0;
	}	

	QtAudioPlot aPlot;
	aPlot.Label("Audio");
	aPlot.Geometry(100,100,500,225);
	aPlot.SetData(audio);
	aPlot.Show();

	return CLAM::VM::QtAppWrapper::Run();
	
}


// END
