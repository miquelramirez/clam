#include "Fl_Smart_Tile.H"
#include <FL/Fl_Window.H>

static char *image_shade[] = {
"11 11 2 1",
".\tc #000000",
"+\tc #FFFFFF",
"...........",
"...........",
"...........",
"...........",
"...........",
"..+++++++..",
"..+++++++..",
"..+++++++..",
"..+++++++..",
"............",
"............",
};
Fl_Pixmap Fl_Smart_Tile::pixmapShade(image_shade);

static char *image_close[] = {
"11 11 2 1",
".\tc #000000",
"+\tc None",
"...+++++...",
"....+++....",
".....+.....",
"+.........+",
"++.......++",
"+++.....+++",
"++.......++",
"+.........+",
".....+.....",
"....+++....",
"...+++++..."
};
Fl_Pixmap Fl_Smart_Tile::pixmapClose(image_close);

Fl_Smart_Tile::Fl_Smart_Tile(int X,int Y,int W,int H)
:Fl_Group(X,Y,W,H) {
	Fl_Group::resizable();
	init(40,20);
	color(FL_BLACK);
}

void Fl_Smart_Tile::draw(void)
{
	Fl_Group::draw();
	fl_color(color());
	if (children())
		fl_rectf(x(),child(children()-1)->y()+child(children()-1)->h(),w(),
			h()-(child(children()-1)->y()+child(children()-1)->h()));
	else
		fl_rectf(x(),y(),w(),h());
}

void Fl_Smart_Tile::close(Fl_Widget* c)
{
	if (c==child(0) && children()>1)
	{
		child(1)->position(child(1)->x(),child(0)->y());
	}
	remove(c);
	equalize();
}

void Fl_Smart_Tile::shade(Fl_Widget* c)
{
	int i;
	int nshaded = 0;

	if (c) {
		if (c->h()==mShadedsize) {
			c->size(c->w(),0);
			c=0;
		}
		else
			c->size(c->w(),mShadedsize);	
	}
		
	for (i=0;i<children();i++)
	{
		if (child(i)->h()==mShadedsize)
		{
			nshaded++;
		}
	}

	float newsize;
	if (nshaded==children()) newsize=mShadedsize;
	else
	newsize = float(h()-(nshaded*mShadedsize))/float(children()-nshaded);

	float newsizetotal = 0;
	int inewsizetotal = 0;

	if (children())
		child(0)->position(x(),y());

	for (i=0;i<children();i++)
	{

		if (child(i)!=c) {
			if (child(i)->h()!=mShadedsize)
			{
				newsizetotal+=newsize;
				int inewsize = int(newsizetotal)-inewsizetotal;
				inewsizetotal+=inewsize;
				child(i)->size(child(i)->w(),inewsize);
			}
		}
		if (i<children()-1)
			child(i+1)->position(child(i)->x(),child(i)->y()+child(i)->h());
	}
	
	redraw();
}

int Fl_Smart_Tile::handle(int e)
{
	int i;
	static int moving = -1;
	static int cursor = 0;
	static int prevy;
	if (e==FL_MOVE)
	{
		moving = -1;
		for (i=1;i<children();i++)
		{
			if (Fl::event_y()>child(i)->y()-2 && Fl::event_y()<child(i)->y()+2)
			{
				moving = i;
				if (!cursor) {
					window()->cursor(FL_CURSOR_NS);
					cursor = 1;
				}
				return 1;
			}
		}
		if (cursor) {
			window()->cursor(FL_CURSOR_DEFAULT);
			cursor = 0;
		}
	}
	if (e==FL_PUSH && moving!=-1) {
		prevy = Fl::event_y();
		return 1;
	}
	if (e==FL_DRAG && moving!=-1) {
		int move = Fl::event_y()-prevy;
		prevy = Fl::event_y();
		if (child(moving)->h()-move<mMinsize) return 1;
		if (child(moving-1)->h()+move<mMinsize) return 1;
		child(moving)->resize(child(moving)->x(),child(moving)->y()+move,
			child(moving)->w(),child(moving)->h()-move);
		child(moving-1)->resize(child(moving-1)->x(),child(moving-1)->y(),
			child(moving-1)->w(),child(moving-1)->h()+move);
		redraw();
		return 1;
	}
	return Fl_Group::handle(e);
}

