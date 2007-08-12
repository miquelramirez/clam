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

#include <QtOpenGL/QGLWidget>
#undef GetClassName
#include <QtDesigner/QDesignerExportWidget>
#include "FloatArrayDataSource.hxx"

namespace CLAM {
namespace VM {

class QDESIGNER_WIDGET_EXPORT KeySpace : public QGLWidget
{
	Q_OBJECT
	Q_PROPERTY(bool smooth READ smooth WRITE setSmooth)
	Q_PROPERTY(int xPixels READ xPixels WRITE setXPixels)
	Q_PROPERTY(int yPixels READ yPixels WRITE setYPixels)
public:
	KeySpace(QWidget * parent);

	virtual void initializeGL();
	virtual void resizeGL(int width, int height);
	virtual void paintGL();
	virtual void timerEvent(QTimerEvent * event);

	bool smooth() const { return _smooth; }
	void setSmooth(bool beSmooth=true) { _smooth=beSmooth; initializeGL(); }
	int xPixels() const { return _nX; }
	void setXPixels(int xPixels) { if (xPixels>5) _nX=xPixels; } 
	int yPixels() const { return _nY; }
	void setYPixels(int yPixels) { if (yPixels>10) _nY=yPixels; } 

	void setDataSource(FloatArrayDataSource & dataSource )
	{
		_dataSource = &dataSource;
		_nBins = _dataSource->nBins();
	}
	void updateIfNeeded();
	void clearData();

private:
	void DrawTiles();
	void DrawLabels();
	void RecomputeWeights();
	double wdist(double x1,double x2)
	{
		if (x2 > x1+.5) return 1. - (x2-x1);
		if (x2 < x1-.5) return 1. - (x1-x2);
		if (x2 >= x1)   return x2-x1;
		else            return x1-x2;
	}

	unsigned _nBins;
	FloatArrayDataSource * _dataSource;
	double _maxValue;
	int _updatePending;
	const CLAM::TData * _data;
	bool _smooth;
	int _nX;
	int _nY;
	std::vector<float> _weights;
	std::vector<float> _texture;
	GLuint _textureId;
	float _paletteR[256],_paletteG[256],_paletteB[256];
};

}
}

#endif

