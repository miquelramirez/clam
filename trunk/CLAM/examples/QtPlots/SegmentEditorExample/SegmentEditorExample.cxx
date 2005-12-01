#include <stdlib.h>
#include "audio_file_utils.hxx"
#include "QtAppWrapper.hxx"
#include "SegmentEditorExample.hxx"


int main()
{
	CLAM::Audio audio;
	if(qtvm_examples_utils::load_audio("../data/birthday.wav",audio))
	{
	    printf("File Error: \'birthday.wav\' audio file not found!\n");
	    exit(1);
	}

	CLAM::VM::QtAppWrapper::Init();

	QWidget * example = new SegmentEditorExample(audio);
	example->show();
	
	return CLAM::VM::QtAppWrapper::Run();



}

// END

