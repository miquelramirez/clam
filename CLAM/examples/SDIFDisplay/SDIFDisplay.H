/***************************************************************************
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************
 *   Maarten de Boer <maarten.deboer@iua.upf.es>, 1999                     *
 *   Music Technology Group                                                *
 *   Institut de l'Audiovisual, Universitat Pompeu Fabra, Barcelona, Spain *
 *   http://www.iua.upf.es/mtg/                                            *
 ***************************************************************************/

#ifndef __SDIFDisplay__
#define __SDIFDisplay__

#include <FL/Fl_Widget.H>
#include <FL/Fl.H>

#include "SDIFMatrix.hxx"
#include "SDIFFrame.hxx"
#include "SDIFStream.hxx"

class SDIFDisplayColors;

class SDIFDisplay:public Fl_Widget
/*D* Base class for the different SDIF Display classes. */
{
protected:
	SDIF::Stream* mStream; //D/ Pointer to the SDIF Stream which data is displayed.
	SDIF::Stream::FrameIterator mFrameIterator;

	float mHOffset; //D/ Horizontal offset for scrolling and zooming.
	float mHScale;  //D/ Horizontal scale for scrolling and zooming.
	float mVOffset; //D/ Vertical offset for scrolling and zooming.
	float mVScale;  //D/ Vertical scale for scrolling and zooming.

	SDIFDisplayColors* mColorMapping; //D/ Pointer to the color map.
	
	bool mQuick;
public:
	SDIFDisplay(int X,int Y,int W,int H,SDIF::Stream* s); 
	//D/ Create a new SDIFDisplay
	
	virtual void draw(void){}
	static void slow_redraw(void* ptr)
	{
		((SDIFDisplay*)ptr)->mQuick = 0;
		((Fl_Widget*)ptr)->redraw();
	}
	void redraw(void)
	{
		Fl::remove_timeout(slow_redraw,this);
		mQuick=1;
		Fl_Widget::redraw();
		Fl::add_timeout(0.1,slow_redraw,this);		
	}
	void HScale(float v);
	void HOffset(float v); 
	void VScale(float v);
	void VOffset(float v); 

	void MoveIteratorTo(double time);

	SDIFDisplayColors* ColorMapping() { return mColorMapping; }

	virtual double HValueOffset();
	virtual double DHValue(void);

	virtual double VValueOffset();
	virtual double DVValue(void);
};

inline void SDIFDisplay::HScale(float v)
{	
	//D/ Set horizontal scale.
	mHScale = v; 
} 
inline void SDIFDisplay::HOffset(float v) 
{
	//D/ Set horizontal offset.
	mHOffset = v;
} 
inline void SDIFDisplay::VScale(float v)
{
	//D/ Set vertical scale.
	mVScale = v;
}
inline void SDIFDisplay::VOffset(float v) 
{	
	//D/ Set vertical offset.
	mVOffset = v;	
}

inline double SDIFDisplay::HValueOffset()
{
	// Convert the horizontal offset into a time value to access the stream.
	return 
		mStream->Front()->Time() + 
		(mStream->Back()->Time()-mStream->Front()->Time())*mHOffset;
}

inline double SDIFDisplay::DHValue(void)
{
	// Convert the horizontal scale into a time increment to access the stream.
	return 	(mStream->Back()->Time()-mStream->Front()->Time())*
		mHScale/double(w());
}

inline double SDIFDisplay::VValueOffset()
{
	return mVOffset;
}

inline double SDIFDisplay::DVValue(void)
{
	return mVScale/double(h());
}

#endif
