#include <stdlib.h>
#include <QtGui/QApplication>
#include <QtGui/QFileDialog>
#include "vmPlayableAudioStereoPlot.hxx"
#include "audio_file_utils.hxx"

int main(int argc, char** argv)
{
	QApplication app( argc, argv );
	QString fn = QFileDialog::getOpenFileName(0,"Choose a stereo audio file",".","Audio (*.wav *.ogg *.mp3)");
	if(fn.isEmpty())
	{
		printf("No file was choosen.\n");
		exit(1);
	}
	std::vector<CLAM::Audio> channels;
	printf("loading audio...\n");
	qtvm_examples_utils::load_audio_st(fn.toAscii().data(),channels);
	printf("done\n");
	
	CLAM::VM::PlayableAudioStereoPlot plot;
	plot.SetTitle("Audio Stereo");
	plot.SetGeometry(100,50,640,400);
	plot.SetData(channels[0],channels[1]);
	plot.show();

	app.connect(&app,SIGNAL(lastWindowClosed()),&app,SLOT(quit()));
	return app.exec();
}

// END



