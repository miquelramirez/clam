/*****************************************************************************h
Fl_Envelope Widgets

Filename:   Fl_Envelope.cxx
Programmer: Maarten de Boer
            mdeboer@iua.upf.es / maarten@things.nl
Date:       20 Jun 1999

This is free software; you can redistribute it and/or modify it under the terms
of the GNU General Public License as published by the Free Software Foundation;
either version 2 of the License, or (at your option) any later version.
******************************************************************************/
#include <FL/fl_draw.H>
#include <FL/Fl_Window.H>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "Fl_Envelope.H"
#include <iostream>

void Fl_PointMover::bound(int id) 
{
	if (points_[id].x>xboundmax_) {
		points_[id].x=xboundmax_;
	}else if (points_[id].x<xboundmin_) {
		points_[id].x=xboundmin_;
	}

	if (points_[id].y>yboundmax_) {
		points_[id].y=yboundmax_;
	}else if (points_[id].y<yboundmin_) {
		points_[id].y=yboundmin_;
	}
}

void Fl_PointMover::calc_formatstr(char* str,float grid,float max)
{
	float m;
	int i,d;

	m=10.;
	d=1;
	while (fabs((float(int(grid*m))/m)-grid)>grid/100.) {
		d++;
		m=m*10.;
	}
	i=0;
	while (max>1) {
		max=max/10.;
		i++;
	}
	if (i==0) i=1;
	
	sprintf(str,"%%%d.%df",i,d);
}

void Fl_PointMover::calc_formatstr(void)
{
	if (hformatstr[0]=='\0' && griddx_>0) {
		float v=(fabs(xmin_) > fabs(xmax_) ) ? fabs(xmin_) : fabs(xmax_);
		calc_formatstr(hformatstr,griddx_,v);
	}

	if (vformatstr[0]=='\0' && griddy_>0) {
		float v=(fabs(ymin_) > fabs(ymax_) ) ? fabs(ymin_) : fabs(ymax_);
		calc_formatstr(vformatstr,griddy_,v);
	}
}

void Fl_PointMover::grid(float dx,float dy) { 	
	griddx_=dx; griddy_=dy;
}

float Fl_PointMover::distance(float px,float py,float px2,float py2)
{
	return sqrt((px2-px)*(px2-px)+(py2-py)*(py2-py));
}

float Fl_PointMover::distance(int pointid,float px,float py)
{
	return distance(points_[pointid].x,points_[pointid].y,px,py);
}

float Fl_PointMover::cdistance(int pointid,float px,float py)
{
	return distance(fcoorx(points_[pointid].x),fcoory(points_[pointid].y),px,py);
}

int Fl_PointMover::nearest(float px,float py)
{
	float min=distance(0,px,py);
	int retid=0;
	for (int i=1;i<points_.Size();i++) {
		float d=distance(i,px,py);
		if (d<min) {
			min = d;
			retid = i;
		}
	}
	return retid;
}

int Fl_PointMover::cnearest(float cx,float cy)
{
	float min=cdistance(0,cx,cy);
	int retid=0;
	for (int i=1;i<points_.Size();i++) {
		float d=cdistance(i,cx,cy);
		if (d<min) {
			min = d;
			retid = i;
		}
	}
	return retid;
}

void Fl_PointMover::add_point(float px,float py,void* puser_data)
{
	FLPOINT p;
	p.x=px;
	p.y=py;
	p.user_data=puser_data;
	points_.AddElem( p );
	redraw();
}

void Fl_PointMover::remove_point(int id)
{
	points_.DeleteElem( id );
	redraw();
}

void Fl_PointMover::remove_points(int id,int n)
{
	for ( int j = id + n; j < points_.Size(); j++ )
	{
		points_[ j - n ] = points_[j];
	}

	int oldSize = points_.Size();
	points_.Resize( oldSize - n );
	points_.SetSize( oldSize - n );
	redraw();

}

void Fl_PointMover::clear()
{
	points_.Resize(0);
	points_.SetSize(0);
	redraw();
}


void Fl_PointMover::drawselect(void)
{
	
	if (dragselecting_)
	{
		fl_color(FL_DARK1);
		fl_rect(selectx_,selecty_,selectw_,selecth_);
	}
}

/*static FLPOINT* copy=0;
static int copysize=0;
*/


int Fl_PointMover::handle(int event) {
	static int dragdx=0;
	static int dragdy=0;
	static int prevx=0;
	static int prevy=0;

	static int dragdir=0;
	static Fl_Cursor move_cursor[3]={FL_CURSOR_MOVE,FL_CURSOR_WE,FL_CURSOR_NS};
		
	switch (event) {
	case FL_PUSH:
		{
			float px=pointx(Fl::event_x());
			float py=pointy(Fl::event_y());
			dragselecting_=0;
			if (Fl::event_clicks()){

				selection_.Clear();
				
				add_point(snapx(px),snapy(py));
				
				/** The user has added a point*/
				PointAdded.Emit( snapx(px), snapy(py) );

				dragid_=points_.Size()-1;
				do_callback();
				damage(FL_DAMAGE_EXPOSE,
					Fl::event_x()-1,
					Fl::event_y()-1,
					Fl::event_x()+1,
					Fl::event_y()+1);					
				dragid_=-1;
			} else {
				if (points_.Size()>0) {
					dragid_=cnearest(Fl::event_x(),Fl::event_y());
					dragdx = coorx(points_[dragid_].x)-Fl::event_x();
					dragdy = coory(points_[dragid_].y)-Fl::event_y();

					if (abs(dragdx)>3 ||
						abs(dragdy)>3)
					{
						dragid_=-1;
					}

					if (!(Fl::event_state()&FL_SHIFT) && 
						!(Fl::event_state()&FL_CTRL) && 
						!selection_.Selected(dragid_)) {
							selection_.Clear();			
					}

					if (dragid_==-1) {
						dragselecting_ = 1;				
						selectx_ = Fl::event_x();
						selecty_ = Fl::event_y();
						selectw_ = 0;
						selecth_ = 0;
					}else{
						if ((Fl::event_state()&FL_CTRL) && 
							selection_.Selected(dragid_)) {
							selection_.Remove(dragid_);			
						}else{
							selection_.Add(dragid_);
						}
					}
					prevx = coorx(snapx(pointx(Fl::event_x())));
					prevy = coory(snapy(pointy(Fl::event_y())));

					if (dragid_!=-1) {
						window()->cursor(move_cursor[dragdir]);
					}

				}
			}
		}
		take_focus();
		return 1;	
	case FL_RELEASE:
		selection_.StartGet();
				
		if (dragselecting_) {
			get_selection();			
			selection_.Print();
		}
		dragselecting_=0;
		do_callback();		

		dragid_=-1;

		window()->cursor(FL_CURSOR_DEFAULT);						

		return 1;	
	case FL_FOCUS:
		return 1;
	case FL_KEYBOARD:
		{
			switch (Fl::event_key()) {
				case FL_Down:
				{
					float add=pointy(1)-pointy(0);
					selection_.StartGet();
					while (!selection_.DoneGet()) {	
						int id=selection_.Get();
						points_[id].y+=add;
						PointMoved.Emit();
						bound(id);
					}	
					redraw();
					
					return 1;
				}
				case FL_Up:
				{
					float add=pointy(0)-pointy(1);
					selection_.StartGet();
					while (!selection_.DoneGet()) {	
						int id=selection_.Get();
						points_[id].y+=add;
						PointMoved.Emit();
						bound(id);
					}	
					redraw();
					return 1;
				}
				case FL_Right:
				{
					float add=pointx(1)-pointx(0);
					selection_.StartGet();
					while (!selection_.DoneGet()) {	
						int id=selection_.Get();
						points_[id].x+=add;
						PointMoved.Emit();
						bound(id);
					}	
					redraw();
					return 1;
				}
				case FL_Left:
				{
					float add=pointx(0)-pointx(1);
					selection_.StartGet();
					while (!selection_.DoneGet()) {	
						int id=selection_.Get();
						points_[id].x+=add;
						PointMoved.Emit();
						bound(id);
					}	
					redraw();
					return 1;
				}
			}

			char key = Fl::event_text()[0];

			switch (key) {
				case '\t':
					dragdir++;
					if (dragdir==3) dragdir=0;
					if (dragid_!=-1) {
						window()->cursor((Fl_Cursor) move_cursor[dragdir]);						
					}
					return 1;
				case 'X'^0x40:
				{
					int offset=0;
					selection_.StartGet();
					while (!selection_.DoneGet()) {	
						int idfrom=selection_.GetFrom();
						int idto=selection_.GetTo();
						int n=idto-idfrom;
						remove_points(idfrom-offset,n);
						PointRemoved.Emit();
						offset+=n;
					}	
					selection_.Clear();
					do_callback();
					redraw();
					return 1;
				}
/*				case 'C'^0x40:
				{
					int offset=0;
					selection_.StartGet();
					copysize=0;
					while (!selection_.DoneGet()) {	
						int idfrom=selection_.GetFrom();
						int idto=selection_.GetTo();
						copysize+=idto-idfrom;
					}	
					if (copy) delete copy;
					copy=new FLPOINT[copysize];
					selection_.StartGet();
					int i=0;
					while (!selection_.DoneGet()) {	
						int idfrom=selection_.GetFrom();
						int idto=selection_.GetTo();
						for (int j=idfrom;j<idto;j++) {
							copy[i++]=points_[j];
						}
					}	
					return 1;
				}
				case 'V'^0x40:
				{
					selection_.Clear();
					int from=points_.Size();
					for (int i=0;i<copysize;i++) {
						add_point(copy[i].x,copy[i].y);
					}
					selection_.Add(from,points_.Size());
					redraw();
					return 1;
				}
*/				case '-':
				{
					selection_.StartGet();
					int n=0;
					float sum=0;
					while (!selection_.DoneGet()) {	
						int id=selection_.Get();
						sum+=points_[id].y;
						PointMoved.Emit();
						n++;
					}	
					float average=sum/float(n)*0.1;
					selection_.StartGet();
					while (!selection_.DoneGet()) {	
						int id=selection_.Get();
						points_[id].y=(points_[id].y*0.9+average);
						PointMoved.Emit();
						bound(id);
					}	
					redraw();
					return 1;
				}
				case '+':
				{
					selection_.StartGet();
					int n=0;
					float sum=0;
					while (!selection_.DoneGet()) {	
						int id=selection_.Get();
						sum+=points_[id].y;
						n++;
					}	
					float average=sum/float(n)*0.1;
					selection_.StartGet();
					while (!selection_.DoneGet()) {	
						int id=selection_.Get();
						points_[id].y=(points_[id].y*1.1-average);
						PointMoved.Emit();
						bound(id);
					}	
					redraw();
					return 1;
				}
				case '*':
				{
					selection_.StartGet();
					int n=0;
					float sum=0;
					while (!selection_.DoneGet()) {	
						int id=selection_.Get();
						sum+=points_[id].y;
						n++;
					}	
					float average=2.*sum/float(n);
					selection_.StartGet();
					while (!selection_.DoneGet()) {	
						int id=selection_.Get();
						points_[id].y=average-points_[id].y;
						PointMoved.Emit();
						bound(id);
					}	
					redraw();
					return 1;
				}
				case 'A'^0x40:
				{
					selection_.Add(0,points_.Size());
					redraw();
				}
			}
		}
		return 1;
	case FL_DRAG:
		if (dragselecting_) {
			selectw_ = Fl::event_x() - selectx_;
			selecth_ = Fl::event_y() - selecty_;
			redraw();
		}else if (dragid_!=-1) {
			damage(FL_DAMAGE_EXPOSE,
				prevx+dragdx-1,
				prevy+dragdy-1,
				prevx+dragdx+1,
				prevy+dragdy+1);
	
			selection_.StartGet();
			
			int newx;
			int newy;

			newx = coorx(snapx(pointx(Fl::event_x())));
			newy = coory(snapy(pointy(Fl::event_y())));

			if (dragdir==1) newy=prevy;
			if (dragdir==2) newx=prevx;
			
			while (!selection_.DoneGet()) {	
				int id=selection_.Get();
				points_[id].x=snapx(
					pointx(fcoorx(points_[id].x)+newx-prevx));
				points_[id].y=snapy(
					pointy(fcoory(points_[id].y)+newy-prevy));
				PointMoved.Emit();
				bound(id);
			}
			
			prevx = newx;
			prevy = newy;

			damage(FL_DAMAGE_EXPOSE,
				Fl::event_x()+dragdx-1,
				Fl::event_y()+dragdy-1,
				Fl::event_x()+dragdx+1,
				Fl::event_y()+dragdy+1);

			do_callback();					
		}
		return 1;	
	}
	
	return 0;
}

void Fl_PointMover::xscale(float min,float max)
{
	xmin_=min;
	xmax_=max;
}

void Fl_PointMover::yscale(float min,float max)
{
	ymin_=min;
	ymax_=max;
}

void Fl_PointMover::xbound(float min,float max)
{
	xboundmin_=min;
	xboundmax_=max;
}

void Fl_PointMover::ybound(float min,float max)
{
	yboundmin_=min;
	yboundmax_=max;
}

Fl_PointMover::Fl_PointMover(int x,int y,int w,int h)
	:Fl_Widget(x,y,w,h)
{
	points_.Resize(0);
	points_.SetSize(0);
	xscale(0.,1.);	
	yscale(0.,1.);	
	xbound(0.,1.);
	ybound(0.,1.);
	dragid_=-1;
	snap_=0;
	strcpy(hformatstr,"");
	strcpy(vformatstr,"");
	
	leftmargin_=0;
	topmargin_=0;
	rightmargin_=0;
	bottommargin_=0;

	hmarkerspacing_=50;
	vmarkerspacing_=50;
}

void Fl_PointMover::get_selection(void) 
{
	float xm=float(w()-1.-hmargin())/(xmax_-xmin_);
	float ym=float(h()-1.-vmargin())/(ymax_-ymin_);
	
	for (int i=0;i<points_.Size();i++) {
		int xnew = int (x()+leftmargin_+(points_[i].x-xmin_)*xm+0.5);
		int ynew = int (y()+h()-1.-bottommargin_-(points_[i].y-ymin_)*ym+0.5);		
		if (xnew>=selectx_ && xnew<=selectx_+selectw_ &&
			ynew>=selecty_ && ynew<=selecty_+selecth_
		) {
			if (Fl::event_state()&FL_CTRL) {
				if (selection_.Selected(i))
					selection_.Remove(i);
				else
					selection_.Add(i);					
			}else{
				selection_.Add(i);
			}
		}
	}
}

void Fl_PointMover::draw(void) 
{
	fl_color(FL_WHITE);
	fl_rectf(x(),y(),w(),h());

	float xm=float(w()-1.-hmargin())/(xmax_-xmin_);
	float ym=float(h()-1.-vmargin())/(ymax_-ymin_);
	
	fl_color(FL_BLACK);
		
	for (int i=0;i<points_.Size();i++) {
		int xnew = int (x()+(points_[i].x-xmin_)*xm+0.5);
		int ynew = int (y()+h()-1-bottommargin_-(points_[i].y-ymin_)*ym+0.5);		
		fl_line(xnew-1,ynew,xnew+1,ynew);
		fl_line(xnew,ynew-1,xnew,ynew+1);
	}
}

int point_x_compar(const void* a,const void* b)
{
	if (((FLPOINT*)b)->x<((FLPOINT*)a)->x) return 1;
	if (((FLPOINT*)b)->x>((FLPOINT*)a)->x) return -1;
	return 0;
}

void Fl_Envelope::sort(void)
{
	qsort((void*) points_.GetPtr(),points_.Size(),sizeof(FLPOINT),point_x_compar);
}

Fl_Envelope::Fl_Envelope(int x,int y,int w,int h)
:Fl_PointMover(x,y,w,h)
{
}

int Fl_Envelope::handle(int e)
{
	if (Fl_PointMover::handle(e)) {
		switch (e) {
		case FL_PUSH:
			if (Fl::event_clicks()) sort();
			break;
		case FL_KEYBOARD:
		case FL_DRAG:
			{
				selection_.StartGet();
				while (!selection_.DoneGet()) {	
					int idfrom=selection_.GetFrom();
					int idto=selection_.GetTo();
					int i=idfrom-1;
					while (i>=0 && points_[i].x>points_[idfrom].x) {
						points_[i].x=points_[idfrom].x;
						i--;
					}
					i=idto;
					idto--;
					while (i<points_.Size() && points_[i].x<points_[idto].x) {
						points_[i].x=points_[idto].x;
						i++;
					}
				}
			}
		}
		redraw();
		return 1;
	}
	return 0;
}

void Fl_Envelope::draw(void) 
{
	calc_formatstr();
	
	fl_color(FL_RED);

	if (leftmargin_>rightmargin_) {
		if (topmargin_>bottommargin_) {
			fl_rectf(x()+leftmargin_-1,y()+topmargin_-1,1,1);
		}else{
			fl_rectf(x()+leftmargin_-1,y()+h()-bottommargin_,1,1);		
		}
	}else{
		if (topmargin_>bottommargin_) {
			fl_rectf(x()+w()-rightmargin_,y()+topmargin_-1,1,1);
		}else{
			fl_rectf(x()+w()-rightmargin_,y()+h()-bottommargin_,1,1);		
		}
	}
	fl_push_clip(x()+leftmargin_,y()+topmargin_,w()-hmargin(),h()-vmargin());
	fl_color(FL_WHITE);
	fl_rectf(x()+leftmargin_,y()+topmargin_,w()-hmargin(),h()-vmargin());

	fl_color(FL_BLACK);

	float xm=float(w()-1.-hmargin())/(xmax_-xmin_);
	float ym=float(h()-1.-vmargin())/(ymax_-ymin_);
	
	int xprev=0,yprev=0;

	fl_color(200,200,255);

	if (griddx_>0.0) {
		float from=
			fcoorx(float(int(pointx(x()+leftmargin_)/griddx_))*griddx_);
		float to=
			fcoorx(float(int(pointx(x()+w()-rightmargin_)/griddx_))*griddx_);
		float d=griddx_*xm;
		
		from+=0.5;
		to+=0.5;
		
		while (from<=to+d) {
			fl_line(from,y()+topmargin_,from,y()+h()-bottommargin_);
			from+=d;
		}		
	}

	if (griddy_>0.0) {
		float from=
			fcoory(float(int(pointy(y()+topmargin_)/griddy_))*griddy_);
		float to=
			fcoory(float(int(pointy(y()+h()-bottommargin_)/griddy_))*griddy_);
			
		from+=0.5;
		to+=0.5;
			
		float d=griddy_*ym;
		while (from<=to+d) {
			fl_line(x(),from,x()+w()-rightmargin_,from);
			from+=d;
		}		
	}
	
	drawselect();

#define snh 0
	if (snh) {
		fl_color(FL_DARK1);	
	}else{
		fl_color(FL_DARK1);	
	}
	
	for (int i=0;i<points_.Size();i++) {
		float fx = float(x()+leftmargin_)+(points_[i].x-xmin_)*xm;
		float fy = float(y()+h()-1-bottommargin_)-(points_[i].y-ymin_)*ym;
		float xextra = fx<0 ? -0.5 : 0.5;
		float yextra = fy<0 ? -0.5 : 0.5;
		int xnew = int (fx+xextra);
		int ynew = int (fy+yextra);
		if (i) {
			if (snh) {
				fl_line(xprev,yprev,xnew,yprev);
				fl_line(xnew,yprev,xnew,ynew);
			} else {
				fl_line(xprev,yprev,xnew,ynew);
			}
		}
		xprev=xnew;
		yprev=ynew;
	}
	
	for (int i=0;i<points_.Size();i++) {
		int xnew = int(x()+leftmargin_+(points_[i].x-xmin_)*xm+0.5);
		int ynew = int(y()+h()-1-bottommargin_-(points_[i].y-ymin_)*ym+0.5);		

		if (selection_.Selected(i)) {
			fl_color(FL_RED);
			fl_line(xnew-2,ynew,xnew+2,ynew);
			fl_line(xnew,ynew-2,xnew,ynew+2);
		}else{
			fl_color(FL_BLACK);
			fl_line(xnew-1,ynew,xnew+1,ynew);
			fl_line(xnew,ynew-1,xnew,ynew+1);
		}
	}

	fl_pop_clip();

	fl_color(color());
	if (topmargin_>bottommargin_) {
		fl_push_clip(x()+leftmargin_,y(),w()-hmargin(),topmargin_);
		fl_draw_box( FL_FLAT_BOX, x()+leftmargin_,y(),w()-hmargin(),topmargin_, fl_color() );
	}else{
		fl_push_clip(x()+leftmargin_,y()+h()-bottommargin_,w()-hmargin(),bottommargin_);
		fl_draw_box( FL_FLAT_BOX, x()+leftmargin_,y()+h()-bottommargin_,w()-hmargin(),bottommargin_, fl_color() );
	}

	fl_color(FL_BLACK);
	fl_font(FL_COURIER,10);

	int drawy1;
	int drawy2;
	
	if (topmargin_>bottommargin_) {
		drawy1=y();
		drawy2=y()+topmargin_-1;
		fl_xyline(x()+leftmargin_,drawy2,x()+w()-rightmargin_);	
	}else{
		drawy1=y()+h()-bottommargin_;
		drawy2=y()+h();
		fl_xyline(x()+leftmargin_,drawy1,x()+w()-rightmargin_);
	}

	if (griddx_>0.0) {
		float d=griddx_*xm;
		float pd=griddx_;
		float p;
		int m=1;
		if (d<hmarkerspacing_) {
			m=int(float(hmarkerspacing_)/d);
			d*=float(m);
			pd*=float(m);
		}

		float from=
			fcoorx(float(int(pointx(x()+leftmargin_)/
				(griddx_*float(m))))*griddx_*float(m));
			
		float to=
			fcoorx(float(int(pointx(x()+w()-rightmargin_)/
				(griddx_*float(m))))*griddx_*float(m));

		p=pointx(from);

		from+=0.5;
		to+=0.5;

		while (from<=to+d) {
			fl_line(from,drawy1,from,drawy2);
			char val[8];
			if (p>-0.0000001 && p<0.0000001) p=0;
			sprintf(val,hformatstr,p);
			fl_draw(val,from+2,drawy2-2);
			from+=d;
			p+=pd;
		}		
	}

	fl_pop_clip();
	fl_color(color());
	if (leftmargin_>rightmargin_) {
		fl_push_clip(x(),y()+topmargin_,leftmargin_,h()-vmargin());
		fl_draw_box( FL_FLAT_BOX, x(),y()+topmargin_,leftmargin_,h()-vmargin(), fl_color() );
	}else{
		fl_push_clip(x()+w()-rightmargin_,y()+topmargin_,rightmargin_,h()-vmargin());
		fl_draw_box( FL_FLAT_BOX, x()+w()-rightmargin_,y()+topmargin_,rightmargin_,h()-vmargin(), fl_color() );
	}
	int drawx1;
	int drawx2;

	fl_color(FL_BLACK);
	
	if (leftmargin_>rightmargin_) {
		drawx1=x();
		drawx2=x()+leftmargin_-1;
		fl_yxline(drawx2,y()+topmargin_,y()+h()-bottommargin_);
	}else{
		drawx1=x()+w()-rightmargin_+2;
		drawx2=x()+w()-rightmargin_;
		fl_yxline(drawx2,y()+topmargin_,y()+h()-bottommargin_);
	}

	if (griddy_>0.0) {
		float d=griddy_*ym;
		float pd=griddy_;
		float p;
		int m=1;
		if (d<vmarkerspacing_) {
			m=int(float(vmarkerspacing_)/d);
			d*=float(m);
			pd*=float(m);
		}

		float from=
			fcoory(float(int(pointy(y()+topmargin_)/
				(griddy_*float(m))))*griddy_*float(m));
			
		float to=
			fcoory(float(int(pointy(y()+h()-bottommargin_)/
				(griddy_*float(m))))*griddy_*float(m));

		p=pointy(from);

		from+=0.5;
		to+=0.5;

		while (from<=to+d) {
			fl_line(drawx1,from,drawx2,from);
			char val[8];
			if (p>-0.0000001 && p<0.0000001) p=0;
			sprintf(val,hformatstr,p);
			fl_draw(val,drawx1,from-2);
			from+=d;
			p-=pd;
		}		
	}

	fl_pop_clip();
}
