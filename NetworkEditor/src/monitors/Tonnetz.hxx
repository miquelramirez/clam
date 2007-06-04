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

#ifndef Tonnetz_hxx
#define Tonnetz_hxx

#include <QtOpenGL/QGLWidget>
#undef GetClassName
#include "FloatArrayDataSource.hxx"
#include <CLAM/PortMonitor.hxx>

#include <vector>

class TonnetzMonitor : public CLAM::PortMonitor<std::vector<CLAM::TData> >, public CLAM::VM::FloatArrayDataSource
{
public:
	TonnetzMonitor()
		: _size(12)
	{
	}
private:
	const char* GetClassName() const { return "Tonnetz"; };
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
			};
		return a[bin];
	}
	const CLAM::TData * frameData()
	{
		const std::vector<CLAM::TData> & pcp = FreezeAndGetData();
		_size = pcp.size();
		return &pcp[0];
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
};


#include <QtDesigner/QDesignerExportWidget>

namespace CLAM
{
namespace VM
{

	class QDESIGNER_WIDGET_EXPORT Tonnetz : public QGLWidget
	{
		Q_OBJECT

		public:
			Tonnetz(QWidget * parent);
			~Tonnetz();
			virtual void initializeGL();
			virtual void resizeGL(int width, int height);
			virtual void paintGL();
			virtual void timerEvent(QTimerEvent * event);
		private:
			unsigned BinAtPosition(int x, int y);
			void Draw();
			void DrawTile(int x, int y);
			void DrawLabel(int x, int y);
			void DrawChordsShapes();
		public:
			void updateIfNeeded();
			void setDataSource( FloatArrayDataSource & dataSource );
			void clearData();
		protected:
			int _updatePending;
			double _maxValue;
			unsigned _nBins;
			FloatArrayDataSource * _dataSource;
			const CLAM::TData * _data;
			QFont _font;
	};


}
}



#endif// Tonnetz_hxx

