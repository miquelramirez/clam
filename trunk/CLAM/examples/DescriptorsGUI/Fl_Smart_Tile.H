#ifndef __FL_SMART_TILE__
#define __FL_SMART_TILE__

#include <FL/Fl_Group.H>
#include <FL/Fl.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Pixmap.H>

class Fl_Smart_Tile:public Fl_Group
{
protected:
	int _minsize;
	int _shadedsize;
public:
	Fl_Smart_Tile(int X,int Y,int W,int H);
	void init(int minsize,int shadedsize)
	{
		_minsize=minsize;
		_shadedsize=shadedsize;
	}
	void equalize(void)
	{
		shade(0);
	}
	void shade(Fl_Widget* c);
	void close(Fl_Widget* c);

	void draw(void);
	int handle(int e);
	
	static Fl_Pixmap pixmapClose;
	static Fl_Pixmap pixmapShade;
};

#endif
