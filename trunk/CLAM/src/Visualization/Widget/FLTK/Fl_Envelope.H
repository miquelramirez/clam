/*****************************************************************************h
Fl_Envelope Widgets

Filename:   Fl_Envelope.H
Programmer: Maarten de Boer
            mdeboer@iua.upf.es / maarten@things.nl
Date:       20 Jun 1999

This is free software; you can redistribute it and/or modify it under the terms
of the GNU General Public License as published by the Free Software Foundation;
either version 2 of the License, or (at your option) any later version.
******************************************************************************/
#ifndef __FL_ENVELOPE__
#define __FL_ENVELOPE__

#include <FL/Fl_Widget.H>
#include <FL/Fl.H>
#include "Array.hxx"
#include "Selection.H"
#include "Signalv2.hxx"
#include "Signalv0.hxx"

typedef struct {
	float x;
	float y;
	void* user_data;
} FLPOINT;

#define SNAPX 1
#define SNAPY 2

#define SNAPBOTH 3

class Fl_PointMover:public Fl_Widget {
protected:
	CLAM::Array<FLPOINT> points_;
	float	 xmin_;
	float	 xmax_;
	float	 ymin_;
	float	 ymax_;

	float	 xboundmin_;
	float	 xboundmax_;
	float	 yboundmin_;
	float	 yboundmax_;

	int 	dragid_;

	float	griddx_;
	float	griddy_;

	int		selectx_;
	int		selecty_;
	int		selectw_;
	int		selecth_;

	Selection	selection_;
	bool dragselecting_;

	char hformatstr[16];
	char vformatstr[16];

	void calc_formatstr(char* str,float grid,float max);
	void calc_formatstr(void);

	int leftmargin_;
	int topmargin_;
	int rightmargin_;
	int bottommargin_;
	
	int hmarkerspacing_;
	int vmarkerspacing_;

public:
	int vmargin(void){ return topmargin_+bottommargin_; }
	int hmargin(void){ return leftmargin_+rightmargin_; }

	int leftmargin(void){ return leftmargin_; }
	int topmargin(void){ return topmargin_; }
	int rightmargin(void){ return rightmargin_; }
	int bottommargin(void){ return bottommargin_; }

	Fl_PointMover(int x,int y,int w,int h);
	virtual void draw(void);

	/** Signal to send when the user adds a point*/
	SigSlot::Signalv2< double, double > PointAdded;
	SigSlot::Signalv0                   PointMoved;
	SigSlot::Signalv0                   PointRemoved;

	void margin(int left,int top,int right,int bottom) {
		leftmargin_=left;
		topmargin_=top;
		rightmargin_=right;
		bottommargin_=bottom;
	}

	void markerspacing(int h,int v) {
		hmarkerspacing_=h;
		vmarkerspacing_=v;
	}	
	void grid(float dx,float dy);
	void snap(int s) { snap_=s; }
	int  snap(void)  { return snap_; }
	void add_point(float px,float py,void* puser_data = 0);
	void remove_point(int id);
	void remove_points(int id,int n);
	void clear();
	int  points() const { return points_.Size();}
	
	void xscale(float min,float max);
	void yscale(float min,float max);

	void xbound(float min,float max);
	void ybound(float min,float max);

	const FLPOINT& point(int id) const
	{ 
		return points_[id];
	}

	FLPOINT& point( int id ) 
	{
		return points_[id];
	}

	FLPOINT* getPointPtr( int id )
	{
		if ( id < 0 || id >= points_.Size() ) 
			return NULL;
		else
			return &points_[id];
	}

	float xmin(void) { return xmin_; }
	float xmax(void) { return xmax_; }

	float ymin(void) { return ymin_; }
	float ymax(void) { return ymax_; }

	virtual int handle(int event);
	float distance(int pointid,float px,float py);
	float cdistance(int pointid,float px,float py);
	float distance(float px,float py,float px2,float py2);
	int nearest(float px,float py);
	int cnearest(float px,float py);

	void bound(int id);
		
	int snap_;
		
	float snapx(float px)
	{
		if (!(snap_&SNAPX)) return px;
		return float(int(px*(1./griddx_)+(px<0 ? -0.5 : 0.5)))*griddx_;
	}

	float snapy(float py)
	{
		if (!(snap_&SNAPY)) return py;
		return float(int(py*(1./griddy_)+(py<0 ? -0.5 : 0.5)))*griddy_;		
	}
		
	float pointx(float cx)
	{
		return (cx-float(x()+leftmargin_))*(xmax_-xmin_)/
			float(w()-1.-hmargin())+xmin_;
	}
	
	float pointy(float cy)
	{
		return (float(y()+h()-1.-bottommargin_)-cy)*(ymax_-ymin_)/
			float(h()-1.-vmargin())+ymin_;
	}

	int coorx(float px)
	{
		return x()+leftmargin_+int(0.5+(px-xmin_)*float(w()-1.-hmargin())/
			(xmax_-xmin_));
	}

	float fcoorx(float px)
	{
		return float(x())+leftmargin_+float(px-xmin_)*
			float(w()-1.-hmargin())/(xmax_-xmin_);
	}
	
	int coory(float py)
	{
		return y()+h()-1-bottommargin_-
			int(0.5+(py-ymin_)*float(h()-1.-vmargin())/(ymax_-ymin_));
	}

	float fcoory(float py)
	{
		return float(y()+h()-1.-bottommargin_)-
			(py-ymin_)*float(h()-1.-vmargin())/(ymax_-ymin_);
	}

	int dragging(void) { return dragid_; }

	void drawselect(void);

	void get_selection(void);
};

class Fl_Envelope:public Fl_PointMover {
public:
	Fl_Envelope(int x,int y,int w,int h);
	void draw(void);
	int handle(int);
	void sort(void);
};

#endif

