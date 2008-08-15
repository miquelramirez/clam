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

#ifndef BarGraph_hxx
#define BarGraph_hxx

#include "FloatArrayDataSource.hxx"
#include <QtDesigner/QDesignerExportWidget>
#include <QtGui/QWidget>

namespace CLAM
{
namespace VM
{

	class QDESIGNER_WIDGET_EXPORT BarGraph : public QWidget
	{
		Q_OBJECT
		Q_PROPERTY(QColor barGradientBegin READ barGradientBegin WRITE setBarGradientBegin)
		Q_PROPERTY(QColor barGradientEnd READ barGradientEnd WRITE setBarGradientEnd)

		public:
			BarGraph(QWidget * parent, FloatArrayDataSource * dataSource = 0);
			~BarGraph();
		public:
			virtual void paintEvent(QPaintEvent * event);
			virtual void timerEvent(QTimerEvent *event);
			void updateIfNeeded();
			void setDataSource( FloatArrayDataSource & dataSource );
			void noDataSource();
			void clearData();
			// Designer properties
			const QColor & barGradientBegin() const { return _barGradientBegin; }
			void setBarGradientBegin(const QColor & color) { _barGradientBegin=color; }
			const QColor & barGradientEnd() const { return _barGradientEnd; }
			void setBarGradientEnd(const QColor & color) { _barGradientEnd=color; }
		private:
			void findBounds(double & min, double & max, unsigned size, const CLAM::TData * data);
		protected:
			int _updatePending;
			FloatArrayDataSource * _dataSource;
			QColor _barGradientBegin;
			QColor _barGradientEnd;
	};


}
}



#endif// BarGraph_hxx

