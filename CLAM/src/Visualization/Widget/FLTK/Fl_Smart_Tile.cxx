/*
 * Copyright (c) 2001-2002 MUSIC TECHNOLOGY GROUP (MTG)
 *                         UNIVERSITAT POMPEU FABRA
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#ifdef __APPLE__
#define MDB_FLTK_MACOSX_OPENGL_WORKAROUND
#endif

#include <FL/Fl_Group.H>
#include <FL/Fl_Window.H>
#include <FL/Fl.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Pixmap.H>

#include "Fl_Smart_Tile.hxx"

#define FL_DAMAGE_CUSTOM 64
#define TITLE_H 21

// TODO: these should be a member
static int movingnewsize_ = 0; 
static int movement_ = 0;
#define MAX_CHILDREN 256 /* more than 256 in a smart tile is crazy */
static char visible_[MAX_CHILDREN]; 

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
Fl_Pixmap pixmapShade(image_shade);

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
Fl_Pixmap pixmapClose(image_close);

Fl_Smart_Tile::Fl_Smart_Tile(int X,int Y,int W,int H,const char*l)
:Fl_Group(X,Y,W,H,l)
{
	current_ = -1;	
	action_ = ' '; // 's'==shade; 'c'==close; 'm'==move
	buttondown_ = 0;
	minsize_ = 50;
}

void Fl_Smart_Tile::add_adjust(Fl_Widget* widget)
{
	/* resize the widget automatically to a size that will look good 
	** inside the smart tile, before adding */
	if (children()==0)
	{
		widget->resize(0,0,w(),h()-TITLE_H);
	}
	else
	{
		/* for new widgets, we use the size of the largest divided by 2 */
		int max = 0;
		for ( int i = 0; i < children(); i++ )
			if ( child(i)->h() > max )
				max = child(i)->h();

		widget->resize( 0, 0, w(), max );
	}
	add(widget);
#ifdef MDB_FLTK_MACOSX_OPENGL_WORKAROUND
	widget->hide();
	widget->show();
	Fl::flush();
	redraw();
#endif
}


int Fl_Smart_Tile::handle(int e)
{
	static int cursor = 0;
	static int prevy = 0;
	
	if (e==FL_MOVE)
	{
		int i;
		for (i=1;i<children();i++)
		{
			if (
				Fl::event_x()<x()+w()-TITLE_H-TITLE_H &&
				Fl::event_y()>child(i)->y()-TITLE_H && Fl::event_y()<child(i)->y())
			{
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
		return Fl_Group::handle(e);
	}
	
	if (e==FL_LEAVE )
	{
		window()->cursor( FL_CURSOR_DEFAULT );
		return Fl_Group::handle(e);
	}
	
	if (e==FL_DRAG)
	{
		switch (action_)
		{
			case 's':
				{
					if (
						Fl::event_y()>child(current_)->y()-TITLE_H && 
						Fl::event_y()<child(current_)->y() &&
						Fl::event_x()>=x()+w()-TITLE_H-TITLE_H && 
						Fl::event_x()<x()+w()-TITLE_H)
					{
						if (!buttondown_)
						{
							buttondown_ = 1;
							damage(FL_DAMAGE_CUSTOM);
						}
					}else{
						if (buttondown_)
						{
							buttondown_ = 0;			
							damage(FL_DAMAGE_CUSTOM);
						}
					}
				}
				break;
			case 'c':
				if (
					Fl::event_y()>child(current_)->y()-TITLE_H &&
					Fl::event_y()<child(current_)->y() &&
					Fl::event_x()>=x()+w()-TITLE_H &&
					Fl::event_x()<x()+w())
				{
					if (!buttondown_)
					{
						buttondown_ = 1;			
						damage(FL_DAMAGE_CUSTOM);
					}
				}else{
					if (buttondown_)
					{
						buttondown_ = 0;			
						damage(FL_DAMAGE_CUSTOM);
					}
				}
				break;
			case 'm':
				{
					movingnewsize_ = child(current_)->h()-Fl::event_y()+prevy;
					movement_ = 1;
					for (int j=0;j<children();j++)
						if (visible_[j] && child(j)->visible()) 
							child(j)->hide();
					recalc();
					prevy = Fl::event_y();
				}
				break;
			return 1;
		}
	}
	
	if (e==FL_RELEASE)
	{
		switch (action_)
		{
			case 's': // shading
			{
				if (
					Fl::event_y()>child(current_)->y()-TITLE_H && 
					Fl::event_y()<child(current_)->y() &&
					Fl::event_x()>=x()+w()-TITLE_H-TITLE_H && 
					Fl::event_x()<x()+w()-TITLE_H)
				{
					/* released inside the shading button */

					/* toggle shading, using standard FLTK visibility */
					if (child(current_)->visible())
						child(current_)->hide();
					else
						child(current_)->show();

					recalc();
#ifdef MDB_FLTK_MACOSX_OPENGL_WORKAROUND
					for (int i = 0;i<children();i++)
					{
						Fl_Widget* widget = child(i);
						if (widget->visible())
						{
						widget->hide();
						widget->show();
						}
					}
					Fl::flush();
					redraw();
#endif
				}
				
				
								
				break;
			}
			case 'c': // closing
			{
				if (
					Fl::event_y()>child(current_)->y()-TITLE_H && 
					Fl::event_y()<child(current_)->y() &&
					Fl::event_x()>=x()+w()-TITLE_H && 
					Fl::event_x()<x()+w())
				{
					/* released inside the closing button */
					Fl_Widget* w = child(current_);
					remove(w);
					w->do_callback(); // SHOULD THIS CALL THE CALLBACK??
					//delete w;	
#ifdef MDB_FLTK_MACOSX_OPENGL_WORKAROUND
					recalc();
					for (int i = 0;i<children();i++)
					{
						Fl_Widget* widget = child(i);
						if (widget->visible())
						{
						widget->hide();
						widget->show();
						}
					}
					Fl::flush();
#endif
					redraw();
				}
				break;
			}
			case 'm':
			{
				for (int j=0;j<children();j++)
				{
					if (visible_[j]) child(j)->show();
				}
				redraw();
				break;
			}
		}
		action_ = ' ';
		return 1;
	}
	
	if (e==FL_PUSH)
	{
		action_ = ' ';
		int i;
		for (i=0;i<children();i++)
		{
			if (Fl::event_y()>child(i)->y()-TITLE_H && Fl::event_y()<child(i)->y())
			{
				current_ = i;
				if (i>0 && Fl::event_x()<x()+w()-TITLE_H-TITLE_H)
				{
					action_ = 'm';
					for (int j=0;j<children();j++)
					{
						visible_[j] = child(j)->visible();
					}
					prevy = Fl::event_y();
				}
				if (Fl::event_x()>=x()+w()-TITLE_H-TITLE_H)
				{
					if (Fl::event_x()<x()+w()-TITLE_H)
					{
						action_ = 's';
						buttondown_ = 1;
						damage(FL_DAMAGE_CUSTOM);
					}else{
						action_ = 'c';
						buttondown_ = 1;
						damage(FL_DAMAGE_CUSTOM);
					}
				}
				return 1;
			}
		}
	}
	return Fl_Group::handle(e);
}

void Fl_Smart_Tile::recalc(void)
{
	int ty = y();
	int n = children();
	int rh = h()-n*TITLE_H;
	int mh = 0;
	int i;
	int m = 0;
	int th = 0;
	int changed = 0;
	int minsize = minsize_;
	
	int *sizes = new int[n];
	
	if (action_!='m')
	  for (i = 0; i < n; i++)
			visible_[i] = child(i)->visible();
	
  for (i = 0; i < n; i++)
	{
		sizes[i] = 
			(action_=='m' && i==current_) ? 
			movingnewsize_ : child(i)->h();
		if (visible_[i])
		{
			th += sizes[i];
			m++;
		}
	}

	if (m*minsize>rh)
	{
		minsize = rh/m;
	}
	
	if (th!=rh)
	{	
		int d = rh-th;
		if (action_=='m')
		{
			int i;
			
			i = current_;
			if (sizes[i]<minsize)
			{
				int d2 = sizes[i]-minsize;
				sizes[i]=(minsize);
				i++;
				while (d2!=0 && i!=n)
				{
					if (visible_[i])
					{
						int ch = sizes[i];
						int nh = ch+d2;
						if (nh<minsize) nh = minsize;
						sizes[i]=nh;
						d2 -= (nh-ch);
					}
					i++;
				}
				d += d2;
			}
			
			i = current_;
			while (d!=0 && i)
			{
				i--;
				if (visible_[i])
				{
					int ch = sizes[i];
					int nh = ch+d;
					if (nh<minsize) nh = minsize;
					sizes[i]=nh;
					d -= (nh-ch);
				}
			}
			i = current_ + 1;
			while (d!=0 && i!=n)
			{
				if (visible_[i])
				{
					int ch = sizes[i];
					int nh = ch+d;
					if (nh<minsize) nh = minsize;
					sizes[i]=nh;
					d -= (nh-ch);
				}
				i++;
			}
			if (d)
			{
				sizes[current_] = sizes[current_]+d;
			}
		}
		else
		{
			if (action_=='s' && child(current_)->visible()) m--;
			// avoid resizing the just unshaded

			while (d&&m)
			{
				int dh = d/m;
				m = 0;
				if (dh==0) dh = d<0 ? -1 : 1;
				for (i=0;i<n && d;i++)
				{
					if (!(action_=='s' && i==current_) && visible_[i])
					{
						int ch = sizes[i];
						int nh = ch+dh;
						if (nh<minsize) nh = minsize;
						if (nh!=minsize) m++;
						
						sizes[i] = nh;						
						
						d -= (nh-ch);
					}
				}
			}
		}
	}

	ty = y();	

	n = children();

	for (i=0;i<n;i++)
	{
		ty += TITLE_H;
		if (ty!=child(i)->y() || sizes[i]!=child(i)->h())
		{
			changed = 1; child(i)->redraw();
		}
		child(i)->resize(x(),ty,w(),sizes[i]);
		if (visible_[i])
		{
			ty += sizes[i];
		}
	}
		
	if (changed) damage(FL_DAMAGE_CHILD);

	delete sizes;
}

void Fl_Smart_Tile::draw(void)
{
	int i;
	int n = children();

	recalc();
	int ty = y();
	
	if (action_!='m') 
	{
	  for (i = 0; i < n; i++)
		{
			visible_[i] = child(i)->visible();
		}
	}

	for (i=0;i<n;i++)
	{
		fl_draw_box(FL_THIN_UP_BOX,x(),ty,w()-TITLE_H-TITLE_H,TITLE_H,color());
		fl_color(labelcolor());
		// children don't show their labels when inside a Smart_Tile
		child(i)->labeltype( FL_NO_LABEL );
		fl_font(labelfont(),labelsize());
		fl_draw(child(i)->label(),x()+5,ty,w()-35,TITLE_H,
			Fl_Align(FL_ALIGN_LEFT|FL_ALIGN_INSIDE));
		fl_draw_box(action_=='s' && current_==i && buttondown_ 
			? FL_THIN_DOWN_BOX : FL_THIN_UP_BOX,
			x()+w()-TITLE_H-TITLE_H,ty,TITLE_H,TITLE_H,color());
		pixmapShade.draw(x()+w()-TITLE_H-TITLE_H+5,ty+5);
		fl_draw_box(action_=='c' && current_==i && buttondown_ 
			? FL_THIN_DOWN_BOX : FL_THIN_UP_BOX,
			x()+w()-TITLE_H,ty,TITLE_H,TITLE_H,color());
		pixmapClose.draw(x()+w()-TITLE_H+5,ty+5);
		ty += TITLE_H;
		if (visible_[i]) ty += child(i)->h();
	}
	
	
//	fl_color(color());
//	fl_rectf(x(),ty,w(),h()-(ty-y()));
	if (action_=='m') 
	{
		if (movement_)
		{
			for (i=0;i<n;i++)
				if (visible_[i])
					fl_draw_box(FL_FLAT_BOX,child(i)->x(),child(i)->y(),child(i)->w(),child(i)->h(),color());

			movement_ = 0;
		}
	}
	else
	{
		if (damage()!=FL_DAMAGE_CUSTOM)
		{
			Fl_Group::draw();
		}
		// else: only a button was changed, no full redraw required
	}
}
		
