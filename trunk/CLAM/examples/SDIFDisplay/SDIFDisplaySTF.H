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

#ifndef __SDIFDisplaySTF__
#define __SDIFDisplaySTF__

#include "SDIFDisplay.H"

class SDIFDisplaySTF:public SDIFDisplay
//D/ Class to display an SDIF Short-Term Fourier Transform stream.
{
protected:
	void ValuesAtTime(double* amp,double time,bool quick=0);
public:
	SDIFDisplaySTF(int X,int Y,int W,int H,SDIF::Stream* s);
	void draw(void);

	double VValueOffset();
	double DVValue(void);
};

inline double SDIFDisplaySTF::VValueOffset() {
	return mVOffset * float((*mFrameIterator)->GetMatrixAt(1)->Rows());
}

inline double SDIFDisplaySTF::DVValue(void) {
//	return (h()/float(mStream->Current()->Matrix(1)->RowCount()))/mVScale;
	return float((*mFrameIterator)->GetMatrixAt(1)->Rows()) * mVScale / double(h());
}


#endif
