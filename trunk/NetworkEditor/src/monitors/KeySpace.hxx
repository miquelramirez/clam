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

class KeySpaceDummySource : public CLAM::VM::FloatArrayDataSource
{
	std::vector<CLAM::TData> _data;
public:
	KeySpaceDummySource()
		: _data(nBins())
	{
		_data[5]=1; // C
		_data[17]=.8; // c
		_data[21]=.8; // e
		_data[14]=.8; // a
		_data[10]=.6; // F
		_data[9]=.6; // E
		_data[1]=.6; // G#
		_data[8]=.6; // D#
		_data[22]=.6; // f
		_data[2]=.6; // A
		_data[0]=.6; // G
		_data[12]=.6; // g
		_data[18]=.6; // c#
	}
	const std::string & getLabel(unsigned bin) const
	{
		static std::string a[] = {
			"G",
			"G#",
			"A",
			"A#",
			"B",
			"C",
			"C#",
			"D",
			"D#",
			"E",
			"F",
			"F#",
			"g",
			"g#",
			"a",
			"a#",
			"b",
			"c",
			"c#",
			"d",
			"d#",
			"e",
			"f",
			"f#",
			};
		return a[bin];
	}
	const CLAM::TData * frameData()
	{
		return &_data[0];
	}
	void release()
	{
	}
	unsigned nBins() const
	{
		return 24;
	}
	bool isEnabled() const
	{
		return false;
	}
	
};

class KeySpaceMonitor : public CLAM::PortMonitor<std::vector<CLAM::TData> >, public CLAM::VM::FloatArrayDataSource
{
public:
	KeySpaceMonitor()
		: _size(24)
	{
	}
private:
	const char* GetClassName() const { return "KeySpace"; };
	const std::string & getLabel(unsigned bin) const
	{
		static std::string a[] = {
			"G",
			"G#",
			"A",
			"A#",
			"B",
			"C",
			"C#",
			"D",
			"D#",
			"E",
			"F",
			"F#",
			"g",
			"g#",
			"a",
			"a#",
			"b",
			"c",
			"c#",
			"d",
			"d#",
			"e",
			"f",
			"f#",
			};
		return a[bin];
	}
	const CLAM::TData * frameData()
	{
		const std::vector<CLAM::TData> & pcp = FreezeAndGetData();
		_size = pcp.size()? pcp.size()-1: 0;
		return pcp.size()? &pcp[1] : 0;
	}
	void release()
	{
		UnfreezeData();
	}
	unsigned nBins() const
	{
		return _size;
	}
	bool isEnabled() const
	{
		return GetExecState() == CLAM::Processing::Running;
	}
private:
	unsigned _size;
	std::vector<CLAM::TData> _pcp;
};


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
	void RecomputeWeigths();
};

}
}

#endif

