#include "ProgressGUI.hxx"

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Slider.H>
#include <FL/Fl_Value_Output.H>

#include <cassert>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <iostream> // used for cout. provisional

extern "C"
{
#include "parser.h"
#include "includepaths.h"
#include "config_parser.h"
#include "dsp_parser.h"
}

//-------------------------------------------------------

FlProgress::FlProgress( bool more=true ) : 
	_foundVal(0),
	_guessedVal(0),
	_found(0),
	_guessed(0)
{
	initWindow(more);
	setPercent();
}

void FlProgress::setFound(int found)
{
	if(_found) _found->value( found );
	_foundVal=found;
	setPercent();
}

void FlProgress::setGuessed(int guessed)
{
	if(_guessed) _guessed->value(guessed);
	_guessedVal=guessed;
}
void FlProgress::setPercent()
{
	assert( _foundVal<=_guessedVal );
	assert( 0<=_guessedVal && _guessedVal<1000 ); // todo provisional

	int threshold = 45;
	float ratio = _guessedVal < threshold? 
		float(_foundVal)/threshold : float(_foundVal)/float(_guessedVal);

	_progressBar->value(ratio*100+1);
	std::cout << ratio*100<< "\n";
}

void FlProgress::initWindow( bool more )
{
	Fl_Window *window = new Fl_Window(300,200);
	Fl_Box *box = new Fl_Box(20,10,260,40,"Building CLAM makefile\nwith srcdeps");
	box->labelsize(20);
	box->labelfont(FL_SCREEN);

	_progressBar = new Fl_Slider(20,60,260,30,"source files to find");
	_progressBar->type(FL_HOR_FILL_SLIDER);
	_progressBar->color(FL_WHITE, FL_CYAN);
	_progressBar->bounds(0,100);
	
	if (more)
	{
		box = new Fl_Box(20,120,260, 50); // group
		box->box(FL_UP_BOX);

		_found = new Fl_Value_Output(10+80,135, 40,20, "Found:"  );
		_guessed = new Fl_Value_Output(140+80,135, 40,20, "Guessed:");
	}
	window->end();
	window->show();
}



//-------------------------------------------------------
// ProgressGUIAdapter implementation
//-------------------------------------------------------



ProgressGUIAdapter::ProgressGUIAdapter( int argc, char **argv, bool more/*default true*/ ) : 
	_progress(more),
	_argc(argc),
	_argv(argv)
{
	addIdleProcess( *this );
	ProgressGUIAdapter::refresh();

}


void ProgressGUIAdapter::process()
{

	setUp(_argv);
	
	// begin srcdeps common code
	int cnt = 0;
	item* i = guessed_sources->first;
	while (i)
	{
		fprintf(stderr,"%s %d %d\n",i->str,cnt,list_size(guessed_sources));
		parser_run(i->str);
		
		i = i->next;
		cnt++;
		
		updateProgressBar(cnt, list_size(guessed_sources) );
	}
	// end srcdeps common code 

	finish(_argv);
	removeIdleProcess( *this );
}

void ProgressGUIAdapter::updateProgressBar(int found, int guessed)
{
	setGuessed( guessed );
	setFound( found);
	refresh();
}

void ProgressGUIAdapter::addIdleProcess( ProgressGUIAdapter &process )
{
	Fl::add_idle( &ProgressGUIAdapter::staticProcess, &process );
}

void ProgressGUIAdapter::removeIdleProcess( ProgressGUIAdapter &process )
{
	Fl::remove_idle( &ProgressGUIAdapter::staticProcess, &process );
}

int ProgressGUIAdapter::runEventsLoop()
{
	return Fl::run();
}




void ProgressGUIAdapter::staticProcess( void* thisObject )
{
	static_cast<ProgressGUIAdapter*>(thisObject)->process();
}
void ProgressGUIAdapter::refresh()
{
	Fl::wait();
}

