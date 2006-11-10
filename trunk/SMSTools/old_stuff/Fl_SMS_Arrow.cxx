#include "Fl_SMS_Arrow.hxx"
#include <FL/fl_draw.H>

Fl_SMS_Arrow::Fl_SMS_Arrow( int X, int Y, int W, int H, char* L ) 
	: Fl_Button( X, Y, W, H, L )
{
}

Fl_SMS_Arrow::~Fl_SMS_Arrow()
{
}

void Fl_SMS_Arrow::draw( void )
{
	int X = x();
	int Y = y();
	int W = w();
	int H = h();

	draw_box();
		
	if( strcmp( label(), "/>" ) == 0 )
	{
		fl_color(labelcolor());
		fl_xyline(X,Y+H-6,X+W/2);
		fl_yxline(X+W/2,Y+H-6,Y+5);
		fl_xyline(X+W/2,Y+5,X+W-1);
		fl_line(X+W-6,Y,X+W-1,Y+5);
		fl_line(X+W-6,Y+10,X+W-1,Y+5);
	}
	else if( strcmp( label(),"\\>" ) == 0 )
	{
		fl_color(labelcolor());
		fl_xyline(X,Y+5,X+W/2);
		fl_yxline(X+W/2,Y+5,Y+H-6);
		fl_xyline(X+W/2,Y+H-6,X+W-1);
		fl_line(X+W-6,Y+H-11,X+W-1,Y+H-6);
		fl_line(X+W-6,Y+H-1,X+W-1,Y+H-6);
	}
	else if( strcmp( label(),"->" ) == 0 )
	{
		fl_color(labelcolor());
		fl_xyline(X,Y+H/2,X+W-1);
		fl_line(X+W-6,Y+H/2-5,X+W-1,Y+H/2);
		fl_line(X+W-6,Y+H/2+5,X+W-1,Y+H/2);
	}
	else if( strcmp( label(),"-" ) == 0 )
	{
		fl_color(labelcolor());
		fl_xyline(X,Y+H/2,X+W-1);
	}
}
