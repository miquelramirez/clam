#ifndef _progressGUI_hxx_
#define _progressGUI_hxx_

/*
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Slider.H>
#include <FL/Fl_Value_Output.H>

#include <iostream>
#include <cassert>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"
#include "includepaths.h"
#include "config_parser.h"
#include "dsp_parser.h"
*/

// forward declarations:
class Fl_Slider;
class Fl_Value_Output;



class FlProgress
{
public:
	FlProgress( bool moreDetails);
	
	void setFound(int);
	void setGuessed(int);
	void setPercent();
	
private:
	void initWindow( bool moreDetails );

	int _count;
	Fl_Slider* _progressBar;
	Fl_Value_Output* _found;
	Fl_Value_Output* _guessed;
	int _foundVal;
	int _guessedVal;
};

//-------------------------------------------------------
class ProgressGUIAdapter
{
	
public:
	ProgressGUIAdapter( int argc, char **argv, bool more=true );
	
	/** this method is the entry point or the background processing to be done.
	 * "background" in the sense that the gui-envent-loop is mandatorily in foreground
	 * (if we don't want to use threads)
	 */
	void process();

	static void addIdleProcess( ProgressGUIAdapter &process );
	static void removeIdleProcess( ProgressGUIAdapter &process );
	static int runEventsLoop();

	void updateProgressBar(int found, int guessed);
	
private:
	void setFound(int val) { _progress.setFound( val ); }
	void setGuessed(int val) { _progress.setGuessed( val ); }
	
	/** static Template Methods use by process. To be defined in the main filed */
	static void setUp(char **argv);
	static void finish(char **argv);


	static void staticProcess( void* thisObject );
	static void refresh();


	FlProgress _progress;
	int _argc;
	char ** _argv;
	
};



#endif