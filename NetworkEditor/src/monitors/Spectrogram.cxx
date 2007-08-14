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

#include "Spectrogram.hxx"
#include <cmath>
#include <iostream>

/// Returns dummy data source for unbinded widget
static CLAM::VM::FloatArrayDataSource & getDummySource()
{
	unsigned nBins=12;
	static std::vector<CLAM::TData> data(nBins);
	data[0]=.5;
	data[4]=.3;
	data[9]=.2;
	data[7]=.4;
	static const char * labels[] = {
		"G", "G#", "A", "A#",
		"B", "C", "C#", "D",
		"D#", "E", "F", "F#",
		0
		};
	static CLAM::VM::DummyFloatArrayDataSource dataSource(nBins, &data[0]);
	dataSource.setLabels(labels);
	return dataSource;
}

CLAM::VM::Spectrogram::Spectrogram(QWidget * parent) 
	: QGLWidget(parent)
	, _nBins(0)
	, _nFrames(256)
	, _currentFrame(0)
	, _smooth(true)
{
	_data = 0;
	_dataSource = 0;
	_updatePending=0;
	//_nBins=0;
	_maxValue=1;
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
				"<p>The <b>Spectrogram view</b> displays the spectrogram of the sound.</p>\n"
				));
	setDataSource(getDummySource());
	startTimer(50);
}

void CLAM::VM::Spectrogram::initializeGL()
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
void CLAM::VM::Spectrogram::resizeGL(int width, int height)
{
	glViewport(0,0,width,height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0,1,1,0,-2,2);
	glMatrixMode(GL_MODELVIEW);
}

void CLAM::VM::Spectrogram::timerEvent(QTimerEvent *event)
{
	if ( !_dataSource) return;
	if ( !_dataSource->isEnabled()) return;
	updateIfNeeded();
}

void CLAM::VM::Spectrogram::updateIfNeeded()
{
	if (!_updatePending++) update();
}

void CLAM::VM::Spectrogram::clearData()
{
	_maxValue=1;
}

void CLAM::VM::Spectrogram::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT);
	if (!_dataSource) return;
	_data = _dataSource->frameData();
	_nBins = _dataSource->nBins();
	if (_data) DrawTiles();
	_dataSource->release();

	_updatePending=0;
}

void CLAM::VM::Spectrogram::DrawTiles()
{
	// make sure nBins is greater than 16 so texture draws correctly
	unsigned realBinSize = 16;
	while (realBinSize<_nBins) realBinSize<<=1;
	
	float mean = 0;
	_maxValue*=.5;
	for (unsigned i=0; i<_nBins; i++)
	{
		mean+=_data[i];
		if (_maxValue<_data[i]) _maxValue=_data[i];
	}
	if (_maxValue<1e-10) _maxValue=1e-10;
	if (_maxValue<1.5*mean/_nBins) _maxValue=1.5*mean/_nBins;
	
	if (_texture.size()!=realBinSize*_nFrames*3) _texture.resize(realBinSize*_nFrames*3);
	unsigned texel=0;
	
	for (unsigned k = 0; k < _nBins; k++)
	{		
		for(unsigned i=0; i<_nFrames; i++)
		{
			if (i!=_currentFrame)
			{
				texel+=3;
				continue;
			}
			
			double value = _data[k] / _maxValue;
			value = value / 1.1;
			
			float cidx = value;
			if (cidx > 1.0)
				cidx = 1.0;	

			cidx *= 200.f;
			cidx = floorf(cidx);
			_texture[texel++] = _paletteR[(int)cidx];
			_texture[texel++] = _paletteG[(int)cidx];
			_texture[texel++] = _paletteB[(int)cidx];
		}
		
	}
	
	// increment our frame
	_currentFrame++;
	if (_currentFrame > _nFrames) _currentFrame = 0;
	
	glBindTexture(GL_TEXTURE_2D, _textureId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _nFrames, realBinSize, /*border*/ 0,
		GL_RGB, GL_FLOAT, &_texture[0]);
	
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, _smooth? GL_LINEAR : GL_NEAREST );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, _smooth? GL_LINEAR : GL_NEAREST );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
	float vertexes[] = {0,0,0,1,1,1,1,0};
	float scale = float(_nBins)/realBinSize;
	float xscroll = _scrolling? (float(_currentFrame) / _nFrames) : 0;
	float texvertexes[] = {0+xscroll,0,0+xscroll,scale,1+xscroll,scale,1+xscroll,0};
	
	glEnable(GL_TEXTURE_2D);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glTexCoordPointer(2,GL_FLOAT,0,texvertexes);
	glVertexPointer(2,GL_FLOAT,0,vertexes);
	glDrawArrays(GL_QUADS,0,5);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

