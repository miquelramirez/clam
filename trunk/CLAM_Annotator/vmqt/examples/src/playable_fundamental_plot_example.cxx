#include <QApplication>
#include "audio_file_utils.hxx"
#include "analysis_utils.hxx"
#include "vm_playable_fundamental_plot.hxx"

int main(int argc, char** argv)
{
	CLAM::Audio audio;
	CLAM::Segment segment;

	printf("loading audio...\n");
	qtvm_examples_utils::load_audio("../../data/birthday.wav",audio);
	printf("analysing");
	qtvm_examples_utils::analyze(audio,segment);
	printf("\ndone!\n");
   
	QApplication app( argc, argv );

	CLAM::VM::PlayableFundamentalPlot plot;
	plot.set_title("Fundamental");
	plot.set_geometry(100,100,600,300);
	plot.set_data(segment);
	plot.show();

	app.connect(&app,SIGNAL(lastWindowClosed()),&app,SLOT(quit()));
	return app.exec();
}

// END



