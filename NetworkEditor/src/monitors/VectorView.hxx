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

#ifndef VectorView_hxx
#define VectorView_hxx

#include <QtOpenGL/QGLWidget>
#undef GetClassName
#include "FloatArrayDataSource.hxx"
#include <CLAM/PortMonitor.hxx>

#include <vector>

class VectorViewMonitor :
	public CLAM::PortMonitor<std::vector<CLAM::TData> >,
	public CLAM::VM::FloatArrayDataSource
{
public:
	VectorViewMonitor()
		: _size(0)
	{
	}
private:
	const char* GetClassName() const { return "VectorView"; };
	std::string getLabel(unsigned bin) const
	{
		static std::string dummyLabel;
		return dummyLabel;
	}
	const CLAM::TData * frameData()
	{
		const std::vector<CLAM::TData> & data = FreezeAndGetData();
		_size = data.size();
//		if (!_size) return 0;
		return & data[0];
	}
	void release()
	{
		UnfreezeData();
	}
	unsigned nBins() const
	{
		return _size;
	}
	virtual bool hasUpperBound() const { return false; }
	virtual bool hasLowerBound() const { return false; }
	virtual CLAM::TData upperBound() const {return 1;}
	virtual CLAM::TData lowerBound() const {return 0;}
	bool isEnabled() const
	{
		return IsRunning();
	}
private:
	unsigned _size;
};


#include <QtDesigner/QDesignerExportWidget>
#include "BarGraph.hxx"

namespace CLAM
{
namespace VM
{
	class QDESIGNER_WIDGET_EXPORT VectorView : public BarGraph
	{
		Q_OBJECT
		public:
			VectorView(QWidget * parent, FloatArrayDataSource * dataSource = 0)
				: BarGraph(parent,dataSource)
			{}
			~VectorView();
	};
}
}



#endif// VectorView_hxx

