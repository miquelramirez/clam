#include <FL/Fl_Window.H>
#include <FL/Fl_Output.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Check_Button.H>
#include "Fl_Envelope_Scroll.H"

#include "CLAM_Math.hxx"
#include <cstdio>

void dragcb(Fl_Envelope_Scroll* e,Fl_Output* o)
{
	unsigned dragged = e->envelope->dragging();
	if (dragged > e->envelope->points()) return;
	FLPOINT& p=e->envelope->point(dragged);
	char tmp[256];
	sprintf(tmp,"%2.2f,%2.2f",p.x,p.y);
	o->value(tmp);
}

void writecb(Fl_Button* b,Fl_Envelope* e)
{
	int i=0;
	FILE* f=fopen("output.txt","w");
	const unsigned N = e->points();
	for (int i=0; i<N; i++)
		FLPOINT & p = e->point(i);
	fclose(f);
}

void snapcb(Fl_Check_Button* b,Fl_Envelope* e)
{
	if (b->value()) e->snap(SNAPBOTH);
	else e->snap(0);
}

int main( int argc, char** argv )
{
	Fl_Window w(300,300);

	Fl_Output output(5,5,90,20);
	Fl_Button write(100,5,90,20,"Write");
	Fl_Check_Button snap(200,5,90,20,"Snap");
	
	Fl_Envelope_Scroll s(5,30,265,265);
	w.end();
	w.resizable( s );

	//s.control.hvalue(0,1,-1,2);
	
	//s.control.hvalue( 0, 1, -1, 2 );
	s.control->hvalue( 0, 22050, 0, 22050 );
	//s.control.vvalue( 0, 1, -1, 2 );
	s.control->vvalue( 0, 150, -150, 150 );
	//s.envelope.grid(.1f,.1f);
	s.envelope->grid( 100, 5 );
	s.envelope->snap(0);

	s.envelope->margin(30,20,0,0);

	s.margin_adjust();

	s.callback((Fl_Callback*) dragcb,&output);

	write.callback((Fl_Callback*) writecb,&s.envelope);
	snap.callback((Fl_Callback*) snapcb,&s.envelope);

	/*
	for (float f=-1;f<1;f+=0.025f) 
	{
		s.envelope.add_point(f,sin(f*3.+2.*cos(f*6.)));
	}
	*/
	
	s.envelope->add_point( 0, -30 );
	s.envelope->add_point( 6000, -30 );
	s.envelope->add_point( 6500, -60 );
	s.envelope->add_point( 8500, -120 );
	s.envelope->add_point( 22050, -150 );
	

	w.show();
	Fl::run();

	return 0;
}

