/*
 * Copyright (c) 2001-2006 MUSIC TECHNOLOGY GROUP (MTG)
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

#ifndef KeySpace_hxx
#define KeySpace_hxx

#include "Tonnetz.hxx"
#include <QtDesigner/QDesignerExportWidget>

namespace CLAM {
namespace VM {

class QDESIGNER_WIDGET_EXPORT KeySpace : public Tonnetz
{
	Q_OBJECT
protected:
	float x_res, y_res;

public:
	KeySpace(QWidget * parent);

	virtual void initializeGL();
	virtual void resizeGL(int width, int height);
	virtual void paintGL();
private:
	void DrawTiles();
	void DrawLabels();
	double wdist(double x1,double x2)
	{
		if (x2 > x1+.5) return 1. - (x2-x1);
		if (x2 < x1-.5) return 1. - (x1-x2);
		if (x2 >= x1)   return x2-x1;
		else            return x1-x2;
	}

	float centroidx_,centroidy_;
	float pKeySpaceValue_[24];
	unsigned pRColor[201],pGColor[201],pBColor[201];
	int ColorsIndex[6];
	void RecomputeWeights();
};

}
}

#endif

