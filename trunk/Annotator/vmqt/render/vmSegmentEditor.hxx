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

#ifndef __VMQT_SEGMENT_EDITOR_H__
#define __VMQT_SEGMENT_EDITOR_H__

#include <vector>
#include "vmRenderer2D.hxx"

namespace CLAM
{
	class Segmentation;
	
	namespace VM
	{
		class SegmentEditor : public Renderer2D
		{
			Q_OBJECT
		public:
			SegmentEditor();
			~SegmentEditor();

			void SetSegmentation(Segmentation* s);
			
		signals:
			void segmentOnsetChanged(unsigned,double);
			void segmentOffsetChanged(unsigned,double);
			void segmentInserted(unsigned);
			void segmentDeleted(unsigned);
			void currentSegmentChanged();

		public slots:
			void checkCurrent(double);
			void allowChangeCurrent(bool);

		protected:
			void Render();
			void SetHBounds(double left, double right);
			void SetVBounds(double bottom, double top);
			void MousePressEvent(double x, double y);
			void MouseReleaseEvent(double x, double y);
			void MouseMoveEvent(double x, double y);
			void KeyPressEvent(int key);
			void KeyReleaseEvent(int key);
			void LeaveEvent();
			void SetViewport(const GLViewport& v);

		private:
			Segmentation* mSegmentation;
			
			bool     mAllowChangeCurrent;
			unsigned mCurrentSegment;
		    
			enum { TOLERANCE=3, PIXMARGIN=5 };

			enum EditionMode 
			{
				Idle,
				DraggingOnset,
				DraggingOffset,
				DraggingBody,
				DraggingHeight
			};

			EditionMode mEditionMode;
			unsigned    mDraggedSegment;

			enum { NORMAL=0, 
				   CURRENT=1, 
				   SELECTED=2, 
				   HIGHLIGHTED=3,
				   STIPPLED=4, 
				   FACTOR=4, 
				   PATTERN=0x5555 };

			enum BoundType { LEFT_BOUND=0, RIGHT_BOUND=1, BOTTOM_BOUND=2, TOP_BOUND=3 };

			std::vector<QColor> mColors; 

			int       mHighlighted;
			double    mVMargin;
			BoundType mBoundType;

			void Colorize();
			void PlainRect(double left, double right, double bottom, double top);
			void StippledRect(double left, double right, double bottom, double top);
			bool IsVisible(double left, double right, double lBound, double rBound);
			void DrawSegment(double left, double right, double bottom, double top, bool isCurrent, bool isSelected);
			void DrawVHighlighted(double bottom,  double top);

		};
	}
}

#endif

