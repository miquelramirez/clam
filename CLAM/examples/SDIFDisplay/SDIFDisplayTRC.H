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

#ifndef __SDIFDisplayTRC__
#define __SDIFDisplayTRC__

#include "SDIFDisplay.H"

class SDIFDisplayTRC:public SDIFDisplay
//D/ Class to display an SDIF Sinusoidal Tracks stream.
{
protected:
	void ValuesAtTime(double *freq,double* amp,double time,bool quick=0);
public:
	SDIFDisplayTRC(int X,int Y,int W,int H,SDIF::Stream* s);
	void draw(void);

	double VValueOffset(void);
	double DVValue(void);
};

	
inline double SDIFDisplayTRC::VValueOffset(void) {
	//D/ Convert the vertical offset into a frequency offset
	// +++ should use value from command line instead of 22050
	return 11025.*(mVOffset);
}

inline double SDIFDisplayTRC::DVValue(void) {
	//D/ Convert the vertical scale into a frequency increment
	// +++ should use value from command line instead of 22050
	return 	11025.*(mVScale)/double(h());
}

#endif
