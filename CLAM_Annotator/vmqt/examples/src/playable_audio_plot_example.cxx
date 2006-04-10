#include <stdlib.h>
#include <QtGui/QApplication>
#include <QtGui/QFileDialog>
#include "vmPlayableAudioPlot.hxx"
#include "audio_file_utils.hxx"

int main(int argc, char** argv)
{
	QApplication app( argc, argv );
	QString fn = QFileDialog::getOpenFileName(0,"Choose a mono audio file",".","Audio (*.wav *.ogg)");
	if(fn.isEmpty())
	{
		printf("No file was choosen.\n");
		exit(1);
	}
	CLAM::Audio audio;
	printf("loading audio...\n");
	qtvm_examples_utils::load_audio(fn.toAscii().data(),audio);
	printf("done\n");
   
	CLAM::VM::PlayableAudioPlot plot;
	plot.SetTitle("Audio");
	plot.SetGeometry(100,50,600,300);
	plot.SetData(audio);
	plot.show();

	app.connect(&app,SIGNAL(lastWindowClosed()),&app,SLOT(quit()));
	return app.exec();
}

// END



