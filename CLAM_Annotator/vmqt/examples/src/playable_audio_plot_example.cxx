#include <stdlib.h>
#include <QApplication>
#include <QFileDialog>
#include "vm_playable_audio_plot.hxx"
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
	plot.set_title("Audio");
	plot.set_geometry(100,50,600,300);
	plot.set_data(audio);
	plot.show();

	app.connect(&app,SIGNAL(lastWindowClosed()),&app,SLOT(quit()));
	return app.exec();
}

// END



