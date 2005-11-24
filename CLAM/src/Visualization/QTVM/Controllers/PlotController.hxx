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

#include <vector>
#include <qobject.h>
#include <qstring.h>
#include <qcursor.h>
#include "GLView.hxx"
#include "VLineArrayRenderer.hxx"
#include "Dial.hxx"
#include "SegmentEditor.hxx"

namespace CLAM
{
	class Segmentation;

    namespace VM
    {
		class PlotController : public QObject
		{
			Q_OBJECT

		public:
			PlotController();
			virtual ~PlotController();

			virtual void SetSelPos(const double& value, bool render);
			virtual void DisplayDimensions(const int& w, const int& h);
			virtual void SetMousePos(const double& x, const double& y);
			const double& GetLeftBound() const;
			const double& GetRightBound() const;
			const double& GetBottomBound() const;
			const double& GetTopBound() const;
			virtual void SetLeftButtonPressed(bool pressed);
			virtual void LeaveMouse();
			virtual void EnterMouse();

			virtual void Draw();
			void SetMarks(std::vector<unsigned>& marks);
			std::vector<unsigned>& GetMarks();
			void SetMarksColor(Color c);
			void KeyPressEvent(QKeyEvent* e);
			void KeyReleaseEvent(QKeyEvent* e);
			void OnDoubleClick();
			void SetSegmentationTag(const QString& tag);
			QString GetTag() const;
			unsigned GetMarkIndex() const;
			std::vector<QString> GetTags();

			void SetDialColor(Color c);

			virtual bool IsPlayable();
			
			void SetSegmentationMarksEnabled(bool e);
			void ClearMarks();

			void SetSegmentation(Segmentation* s);

			void MousePressEvent(QMouseEvent* e);
			void MouseReleaseEvent(QMouseEvent* e);
			void MouseMoveEvent(QMouseEvent* e);
		    
		signals:
			void requestRefresh();
			void viewChanged(GLView);
			void hZoomRatio(double);
			void hScrollValue(int);
			void hScrollMaxValue(int);
			void vZoomRatio(double);
			void vScrollValue(int);
			void vScrollMaxValue(int);
			void toolTip(QString);
			void cursorChanged(QCursor);
			void insertedMark(unsigned);
			void removedMark(int, unsigned);
			void updatedMark(int, unsigned);
			void updatedTag(int, QString);
			void initialYRulerRange(double,double);
			void xRulerRange(double,double);
			void yRulerRange(double,double);
			void selectedXPos(double);
			void requestSegmentationTag();
			void mouseOverDisplay(bool);

		public slots:
			void hZoomIn();
			void hZoomOut();	
			void vZoomIn();
			void vZoomOut();
			void updateHScrollValue(int);
			void updateVScrollValue(int);

			void insertMark(unsigned);
			void removeMark(int, unsigned);
			void updateMark(int, unsigned);
			void updateTag(int, QString);

			void enableRendering();
			void disableRendering();
			
			virtual void setHBounds(double, double)=0;
			virtual void setVBounds(double, double)=0;
			virtual void setSelectedXPos(double)=0;

		protected:
			GLView mView;

			virtual void SetHBounds(const double& left, const double& right);
			virtual void SetVBounds(const double& bottom, const double& top);
			const double& GetSelPos() const;
			void SetnSamples(const double& samples);
			const double& GetnSamples() const;
			void SetYRange(const double& min, const double& max);
			const double& GetMaxSpanY() const;
			void SetMinSpanX(const double& min);
			const double& GetMinSpanX() const;
			const double& GetSpanY() const;
			void SetMinSpanY(const double& min);
			const double& GetMinSpanY() const;
			const double& GetSpanX() const;
			const double& GetMouseXPos() const;
			const double& GetMouseYPos() const;
			bool IsLeftButtonPressed();
			bool IsKeyShiftPressed();
			bool IsAbleToEdit();
			const int& GetDisplayWidth() const;
			const int& GetDisplayHeight() const;

			virtual bool CanDrawSelectedPos();
			bool HasSentTag() const;

			void UpdateDial(const double& value);
			const double& GetDialPos() const;

			const double& GetMinY() const;
			const double& GetMaxY() const;

			virtual void FullView()=0;

			bool IsRenderingEnabled() const;

		private slots:
			void segmentEditorWorking(bool);

		private:
			int    mDisplayWidth;
			int    mDisplayHeight;
			double mLeftBound;
			double mRightBound;
			double mBottomBound;
			double mTopBound;
			double mSelPos;
			double mMinSpanX;
			double mMinSpanY;
			double mSamples;
			double mSpanX;
			double mMaxSpanY;
			double mSpanY;
			double mMinY;
			double mMaxY;
			double mHZRatio;
			double mVZRatio;
			double mMouseXPos;
			double mMouseYPos;
			bool   mIsLeftButtonPressed;
			bool   mIsAbleToEdit;

			std::vector<unsigned> mMarks;
			std::vector<QString>  mTags;
			VLineArrayRenderer    mMarksRenderer;
			bool                  mMustProcessMarks;
			bool                  mHit;
			unsigned              mCurrentElem;
			int                   mCurrentIndex;
			bool                  mKeyInsertPressed;
			bool                  mKeyDeletePressed;
			bool                  mKeyShiftPressed;
			bool                  mHasSentTag;
			bool                  mSegmentationMarksEnabled;
			bool                  mIsRenderingEnabled;
			bool                  mSegmentEditorWorking;
			
			Dial mDial;

			SegmentEditor mSegmentEditor;

			void UpdateHBounds(bool zin);
			void UpdateVBounds(bool zin);
			void InitHRatio();
			void InitVRatio();
			int GetnxPixels() const;
			int GetHScrollValue() const;
			int GetnyPixels() const;
			int GetVScrollValue() const;
			bool ReferenceIsVisible();
			const double& GetReference() const;

			void ProcessMarks();
			bool HaveElem(unsigned elem);
			void InsertElem(unsigned elem);
			void RemoveElem(int index);
			void Update(int index, unsigned elem);
			unsigned GetPixel(const double& x) const;
			int Hit(const double& x);
			std::pair<double,double> GetXY(QMouseEvent* e);

		};
    }
}

#endif

