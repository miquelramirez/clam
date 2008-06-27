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
	};


}
}


#endif// Tonnetz_hxx
