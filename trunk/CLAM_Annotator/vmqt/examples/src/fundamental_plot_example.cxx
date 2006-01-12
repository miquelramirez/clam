#include <stdlib.h>
#include <QApplication>
#include <QFileDialog>
#include "audio_file_utils.hxx"
#include "analysis_utils.hxx"
#include "vm_fundamental_plot.hxx"

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
	CLAM::Segment segment;

	printf("loading audio...\n");
	qtvm_examples_utils::load_audio(fn.toAscii().data(),audio);
	printf("analysing");
	qtvm_examples_utils::analyze(audio,segment);
	printf("\ndone!\n");
	
	CLAM::VM::FundamentalPlot plot;
	plot.set_title("Fundamental");
	plot.set_geometry(100,100,600,300);
	plot.set_data(segment);
	plot.show();

// uncomment the following line to see the example on background black mode
//	plot.backgroundBlack();
	
	app.connect(&app,SIGNAL(lastWindowClosed()),&app,SLOT(quit()));
	return app.exec();
}

// END



