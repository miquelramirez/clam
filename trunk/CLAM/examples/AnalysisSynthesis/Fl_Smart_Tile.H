#ifndef __FL_SMART_TILE__
#define __FL_SMART_TILE__

#include <FL/Fl_Group.H>
#include <FL/Fl.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Pixmap.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Button.H>
#include <stdio.h>

class Fl_Smart_Tile:public Fl_Group
{
private:
	class TitledWidget:public Fl_Group
	{
	public:
		Fl_Widget* sub;
		Fl_Group* title;
		Fl_Box* box;
		Fl_Button* shade;
		Fl_Button* close;

		TitledWidget(Fl_Widget* s):Fl_Group(s->x(),s->y(),s->w(),s->h())
		{
			sub = s;
			s->resize(x()+2,y()+22,w()-4,h()-24);

			title = new Fl_Group(x()+2,y()+2,w()-4,20);

			box = new Fl_Box(x()+2,y()+2,w()-44,20,s->label());

			box->box(FL_THIN_UP_BOX);
			box->align(FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
			box->color(FL_DARK2);
			box->labelcolor(FL_BLACK);
			box->labelfont(FL_HELVETICA_BOLD);

			shade = new Fl_Button(x()+w()-42,y()+2,20,20);
			shade->box(FL_THIN_UP_BOX);
			shade->color(FL_DARK2);
			
			Fl_Smart_Tile::pixmapShade.label(shade);		

			shade->callback(shade_cb);

			close = new Fl_Button(x()+w()-22,y()+2,20,20);
			close->box(FL_THIN_UP_BOX);
			close->color(FL_DARK2);
			Fl_Smart_Tile::pixmapClose.label(close);		

			close->callback(close_cb);

			title->resizable(box);
			title->end();
			

			Fl_Group::box(FL_THIN_UP_FRAME);

			add_resizable(*s);

			end();
		}
		static void shade_cb(Fl_Widget* w)
		{
			Fl_Smart_Tile* s = (Fl_Smart_Tile*) (w->parent()->parent()->parent());
			s->shade(w->parent()->parent());
		}
		static void close_cb(Fl_Widget* w)
		{
			Fl_Smart_Tile* s = (Fl_Smart_Tile*) (w->parent()->parent()->parent());
			s->close(w->parent()->parent());
			((TitledWidget*)(w->parent()->parent()))->sub->do_callback();
		}
		void draw(void)
		{
			fl_color(color());
			fl_rect(x()+1,y()+1,w()-2,h()-2);
			box->label(sub->label());

			if (h()<40) sub->hide();
			else sub->show();

			Fl_Group::draw();
		}
	};
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

	void add_titled(Fl_Widget* w)
	{
		add(new TitledWidget(w));
	}
	
	static Fl_Pixmap pixmapClose;
	static Fl_Pixmap pixmapShade;
};

#endif
