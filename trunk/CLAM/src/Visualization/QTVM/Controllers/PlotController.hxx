/*
 * Copyright (c) 2004 MUSIC TECHNOLOGY GROUP (MTG)
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

#ifndef __PLOTCONTROLLER__
#define __PLOTCONTROLLER__

#include <qobject.h>
#include <qstring.h>
#include <qcursor.h>
#include "DataTypes.hxx"

namespace CLAM
{
	namespace VM
	{
		typedef struct
		{
			float left;
			float right;
			float bottom;
			float top;
		} View;

		typedef struct
		{
			int x;
			int y;
			int w;
			int h;
		} Viewport;

		class PlotController : public QObject
		{
			Q_OBJECT

			public:
				PlotController();
				virtual ~PlotController();

				virtual void Draw() = 0;
				virtual void SetSelPos(const TData& value);
				virtual void SurfaceDimensions(int w,int h);
				virtual void SetMousePos(TData x,TData y);

				void HZoomIn();
				void HZoomOut();
				void VZoomIn();
				void VZoomOut();

				void UpdateHViewport(int value);
				TData GetLeftBound() const;
				TData GetRightBound() const;

				void UpdateVViewport(int value);
				TData GetBottomBound() const;
				TData GetTopBound() const;

		                virtual void SetLeftButtonPressed(bool pressed);
		                virtual void LeaveMouse();
		                virtual void EnterMouse();

		                virtual void InsertMark(unsigned elem)=0;
		                virtual void RemoveMark(int index, unsigned elem)=0;
		                virtual void UpdateMark(int index, unsigned elem)=0;
		    

			signals:
				void requestRefresh();
				void sendView(View);

				void hZoomRatio(int);
				void hScrollValue(int);
		    void hScrollMaxValue(int);

				void vZoomRatio(int);
				void vScrollValue(int);
				void vScrollMaxValue(int);

				void selPos(TData);
				void toolTip(QString);

				void leavingMouse();
		    
		                void changeCursor(QCursor);

		                void insertedMark(unsigned);
		                void removedMark(int, unsigned);
		                void updatedMark(int, unsigned);

			protected:
				View _view;
				Viewport _viewport;

				virtual void SetHBounds(const TData& left, const TData& right);
				virtual void SetVBounds(const TData& bottom, const TData& top);

				TData GetSelPos() const;

				void SetnSamples(const TSize& n);
				TSize GetnSamples() const;

				void SetvRange(const TData& vr);
				TData GetvRange() const;

				void SetHMin(const TData& min);
				TData GetHMin() const;

				TData GetVCur() const;
				void SetVMin(const TData& min);
				TData GetVMin() const;

				TData GetCurrent() const;

				TData GetMouseXPos() const;
				TData GetMouseYPos() const;

		                bool IsLeftButtonPressed();
		                bool IsAbleToEdit();

			private:
				TData _lBound;
				TData _rBound;
				TData _bBound;
				TData _tBound;

				TData _selPos;
				TData _hMin;
				TData _vMin;

				TSize _nSamples;
				TData _current;

				TData _vRange;
				TData _vcur;

				int _hzRatio;
				int _vzRatio;

				TData _mouseXPos;
				TData _mouseYPos;

		                bool _isLeftButtonPressed;
		                bool _isAbleToEdit;

				void InitView();
				
				void UpdateHBounds(bool zin=false);
				void UpdateVBounds(bool zin=false);

				void InitHRatio();
				void InitVRatio();

				int GetnxPixels() const;
				int GetHScrollValue() const;

				int GetnyPixels() const;
				int GetVScrollValue() const;

				bool IsVisibleSelPos();

		};
	}
}

#endif

