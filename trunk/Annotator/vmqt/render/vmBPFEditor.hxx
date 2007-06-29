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

#ifndef __VMQT_BPF_EDITOR_H__
#define __VMQT_BPF_EDITOR_H__

#include "BPF.hxx"
#include "vmRenderer2D.hxx"
#include <QtGui/QColor>

namespace CLAM
{
	namespace VM
	{
		class BPFEditor : public Renderer2D
		{
			Q_OBJECT
		public:
			BPFEditor();
			~BPFEditor();

			void SetData(BPF* bpf);
			void SetColors(const QColor& cline, const QColor& chandler);
			void SetFlags(int flags);
			int GetFlags() const;
			const BPF& GetData() const;

		signals:
			void xValueChanged(QString, unsigned, double);
			void yValueChanged(QString, unsigned, double);
			void elementAdded(QString, unsigned, double, double);
			void elementRemoved(QString, unsigned);
			
		protected:
			void Render();
			void SetHBounds(double left, double right);
			void MousePressEvent(double x, double y);
			void MouseReleaseEvent(double x, double y);
			void MouseMoveEvent(double x, double y);
			void KeyPressEvent(int key);
			void KeyReleaseEvent(int key);		   
			void LeaveEvent();

		private:
			int  mglList;
			int  mFlags;
			long mCurrentIndex;
			bool mRebuildglList;

			BPF* mBPF;
			
			QColor mLineColor;
			QColor mHandlerColor;

			enum { LINE_WIDTH=2, POINT_SIZE=2, TOLERANCE=5 };

			enum EditionMode
			{
				Idle,
				DraggingPoint
			};

			EditionMode mEditionMode;

			void Remove();
			long Add(double x, double y);
			void Update(double x, double y);

			long PickPoint(double x, double y);
			long GetBound(double search_value);
			std::pair<long, long> GetBounds();

			double Round(double x);
			QString GetToolTip(double x, double y);

			void ChooseCurrentPoint(long index);
			void MoveCurrentPointdx(double dx);
			void MoveCurrentPointdy(double dy);

			double GetXStep();
			double GetYStep();
		};
	}
}

#endif

