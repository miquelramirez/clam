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

#include "KeySpace.hxx"


/// Returns dummy data source for unbinded widget
static CLAM::VM::FloatArrayDataSource & getDummySource()
{
	// Data simulating a C major
	unsigned nBins=24;
	static std::vector<CLAM::TData> data(nBins);
	data[5]=1; // C
	data[17]=.8; // c
	data[21]=.8; // e
	data[14]=.8; // a
	data[10]=.6; // F
	data[9]=.6; // E
	data[1]=.6; // G#
	data[8]=.6; // D#
	data[22]=.6; // f
	data[2]=.6; // A
	data[0]=.6; // G
	data[12]=.6; // g
	data[18]=.6; // c#
	static CLAM::VM::DummyFloatArrayDataSource source(nBins, &data[0]);
	static const char * labels[] = {
		"G", "G#", "A", "A#",
		"B", "C", "C#", "D",
		"D#", "E", "F", "F#",
		"g", "g#", "a", "a#",
		"b", "c", "c#", "d",
		"d#", "e", "f", "f#",
		0
		};
	source.setLabels(labels);
	return source;
}


struct TKeyNode
{
	float x;
	float y;
};


TKeyNode * getKeyNodes()
{
	static TKeyNode keyNodes[] = 
	{
		{.50,.65}, {.91,.40}, {.33,.15}, {.74,.91}, {.16,.65}, {.60,.40},
		{.01,.15}, {.42,.91}, {.83,.65}, {.25,.40}, {.67,.15}, {.08,.91},
		{.66,.73}, {.08,.50}, {.50,.22}, {.91,.01}, {.33,.73}, {.74,.50},
		{.15,.22}, {.58,.01}, {.01,.73}, {.42,.50}, {.83,.22}, {.25,.01}
	};
	return keyNodes;
}
unsigned nKeyNodes=24;
// The number of 'pixels'

CLAM::VM::KeySpace::KeySpace(QWidget * parent) 
	: Tonnetz(parent)
	, _smooth(true)
	, _nX(128)
	, _nY(64)
{
	static const struct GradientPoint 
	{
		unsigned index;
		unsigned R;
		unsigned G;
		unsigned B;
	} gradient[] =
	{
		{  0,0x00,0x00,0x00},
		{ 30,0x0e,0x11,0x16},
		{ 70,0x50,0x64,0x99},
		{110,0x5a,0xb4,0x64},
		{145,0xe0,0xe0,0x2c},
		{200,0xff,0x3c,0x1e},
		{255,0xff,0x5c,0x1e},
	};
	for (unsigned i=0; i<6-1; i++)
	{
		unsigned index0 = gradient[i].index;
		unsigned index1 = gradient[i+1].index;
		unsigned indexStep = gradient[i+1].index - index0;
		float R = gradient[i].R/256.;
		float G = gradient[i].G/256.;
		float B = gradient[i].B/256.;
		float Rstep = (gradient[i+1].R/256.-R)/indexStep;
		float Gstep = (gradient[i+1].G/256.-G)/indexStep;
		float Bstep = (gradient[i+1].B/256.-B)/indexStep;
		for (unsigned k=index0; k<index1; k++)
		{
			_paletteR[k] = R;
			_paletteG[k] = G;
			_paletteB[k] = B;
			R += Rstep;
			G += Gstep;
			B += Bstep;
		}
	}
	_maxValue = 1;
	setWhatsThis(tr(
				"<p>The <b>Key Space view</b> represents the probability of each key/chord to be the one being played.</p>\n"
				"<p>Mayor chords/keys are displayed with uppercase letters and minor chords/keys are displayed with lowercase letters.\n"
				"Tonally close key/chords are displayed closer so normally you have a color stain covering several chords\n"
				"with the most probable chord as a central color spot.</p>\n"
				));
	setDataSource(getDummySource());
}

void CLAM::VM::KeySpace::initializeGL()
{
	glClearColor(0,0,0,0); // rgba
	glShadeModel(GL_SMOOTH);
//	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//	glEnable(GL_BLEND);
	glEnable(GL_CULL_FACE);
	glGenTextures(1,&_textureId);
//	glEnable (GL_LINE_SMOOTH);
//	glHint (GL_LINE_SMOOTH_HINT, GL_DONT_CARE);
}
void CLAM::VM::KeySpace::resizeGL(int width, int height)
{
	glViewport(0,0,width,height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0,1,1,0,-2,2);
	glMatrixMode(GL_MODELVIEW);
}
void CLAM::VM::KeySpace::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT);
	if (!_dataSource) return;
	_data = _dataSource->frameData();
	if (_data) DrawTiles();
	_dataSource->release();
	DrawLabels();

	_updatePending=0;
}

void CLAM::VM::KeySpace::RecomputeWeights()
{
	std::cout << "Precomputing KeySpace weights... " << std::flush;
	TKeyNode *pKeyNodes = getKeyNodes();
	_weights.resize(_nX*_nY*nKeyNodes);
	for(unsigned i=0; i<_nX; i++)
	{
		float x1 = i / float(_nX);
		for(unsigned k=0; k<_nY; k++)
		{
			float y1 = k / float(_nY);
			for(unsigned m=0; m<nKeyNodes; m++)
			{
				double d1 = wdist(x1,pKeyNodes[m].x);
				double d2 = wdist(y1,pKeyNodes[m].y);
				double dist = d1*d1+d2*d2;											   
				double g = dist*dist;
				if (g < 1E-5)
					g = 1E-5;
				_weights[m+nKeyNodes*(k+_nY*i)] = 1. / g;
			}
		}
	}
	std::cout << "done" << std::endl;
}

void CLAM::VM::KeySpace::DrawTiles()
{
	if (_weights.size()!=_nX*_nY*nKeyNodes)
		RecomputeWeights();
	float mean = 0;
	_maxValue*=.5;
	for (unsigned i=0; i<_nBins; i++)
	{
		mean+=_data[i];
		if (_maxValue<_data[i]) _maxValue=_data[i];
	}
	if (_maxValue<1e-10) _maxValue=1e-10;
	if (_maxValue<1e-10) _maxValue=1e-10;
	if (_maxValue<1.5*mean/_nBins) _maxValue=1.5*mean/_nBins;

	if (_nBins!=nKeyNodes) return;

	if (_texture.size()!=_nY*_nX*3) _texture.resize(_nY*_nX*3);
	unsigned texel=0;
	for(unsigned k=0; k<_nY; k++)
	{
		for(unsigned i=0; i<_nX; i++)
		{
			double num = 0.;
			double den = 0.;
			for(unsigned m=0; m<nKeyNodes; m++)
			{
				unsigned weightIndex = m+nKeyNodes*(k+_nY*i);
				num += _data[m] * _weights[weightIndex] /_maxValue;
				den += _weights[weightIndex];
			}
			double value = (den != 0.) ? num / den : 0;

			float ColorIndex = value;
			if (ColorIndex > 1.0)
				ColorIndex = 1.0;	
			ColorIndex *= ColorIndex;
			ColorIndex *= 200.f;
			int cidx = floorf(ColorIndex);
			_texture[texel++] = _paletteR[cidx];
			_texture[texel++] = _paletteG[cidx];
			_texture[texel++] = _paletteB[cidx];
		}
	}
	glBindTexture(GL_TEXTURE_2D, _textureId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _nX, _nY, /*border*/ 0,
		GL_RGB, GL_FLOAT, &_texture[0]);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, _smooth? GL_LINEAR : GL_NEAREST );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, _smooth? GL_LINEAR : GL_NEAREST );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

	float vertexes[] = {0,0,0,1,1,1,1,0};
	glEnable(GL_TEXTURE_2D);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glTexCoordPointer(2,GL_FLOAT,0,vertexes);
	glVertexPointer(2,GL_FLOAT,0,vertexes);
	glDrawArrays(GL_QUADS,0,5);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

void CLAM::VM::KeySpace::DrawLabels()
{
	TKeyNode *pKeyNodes = getKeyNodes();
	for(unsigned i=0; i<nKeyNodes; i++)
	{
		float x1 = pKeyNodes[i].x;
		float y1 = pKeyNodes[i].y;

		if (y1 < 4./_nY)
			y1 = 4./_nY;

		float value = _data ? _data[i]/_maxValue : 0; 
		if (value>.6) glColor3d(.1,0,0);
		else          glColor3d(1,1,1);

		renderText(x1, y1+.02, .6, _dataSource->getLabel(i).c_str(), font());
	}
}

