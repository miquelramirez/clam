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

#ifndef MelCepstrumView_hxx
#define MelCepstrumView_hxx

#include <QtOpenGL/QGLWidget>
#undef GetClassName
#include "FloatArrayDataSource.hxx"
#include <CLAM/PortMonitor.hxx>

#include <CLAM/MelCepstrum.hxx>

class MelCepstrumViewMonitor :
	public CLAM::PortMonitor<CLAM::MelCepstrum>,
	public CLAM::VM::FloatArrayDataSource
{
public:
	MelCepstrumViewMonitor()
		: _size(0)
	{
	}
private:
	const char* GetClassName() const { return "MelCepstrumView"; };
	const std::string & getLabel(unsigned bin) const
	{
		static std::string dummyLabel;
		return dummyLabel;
	}
	const CLAM::TData * frameData()
	{
		const CLAM::MelCepstrum & data = FreezeAndGetData();
		const CLAM::Array<CLAM::TData> & buffer = data.GetCoefficients();
		_size = buffer.Size();
		return & buffer[0];
	}
	void release()
	{
		UnfreezeData();
	}
	unsigned nBins() const
	{
		return _size;
	}
	virtual bool hasUpperBound() const { return true; }
	virtual bool hasLowerBound() const { return true; }
	virtual CLAM::TData upperBound() const {return .5;}
	virtual CLAM::TData lowerBound() const {return -.5;}
	bool isEnabled() const
	{
		return GetExecState() == CLAM::Processing::Running;
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
	class QDESIGNER_WIDGET_EXPORT MelCepstrumView : public BarGraph
	{
		Q_OBJECT
		public:
			MelCepstrumView(QWidget * parent, FloatArrayDataSource * dataSource = 0)
				: BarGraph(parent,dataSource)
			{}
			~MelCepstrumView();
	};
}
}



#endif// MelCepstrumView_hxx
