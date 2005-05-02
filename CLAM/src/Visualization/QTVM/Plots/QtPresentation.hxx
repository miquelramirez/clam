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

#ifndef __QTPRESENTATION__
#define __QTPRESENTATION__

#include "QtPlot.hxx"

class QLayout;
class QBoxLayout;
class QFrame;
class QPushButton;

namespace CLAM
{
    namespace VM
    {
	class PlotController;
	class DisplaySurface;
	class Ruler;
	class HScrollGroup;
	class VScrollGroup;
		
	/*
	 * This class implements a common presentation. 
	 * A view with: 
	 *   - surface 
	 *   - scroll bars
	 *   - rulers: horizontal/vertical 
	 *   - zoom controls: in/out 
	 */ 
	class QtPresentation : public QtPlot
	{
	    Q_OBJECT		
	
	public:
	    QtPresentation(QWidget* parent = 0);
	    virtual ~QtPresentation();
			
	    virtual void SetMarks(std::vector<unsigned>& marks)=0;
	    virtual std::vector<unsigned>& GetMarks()=0;
	    virtual void SetMarksColor(Color c)=0;

	    void SetBackgroundColor(Color c);
	    void Label(const std::string& label);
	    void Geometry(int x,int y,int w,int h);
	    void Show();
	    void Hide();

	    void SetToggleColorOn(bool b);
	    void SwitchDisplayColors(bool b);

	    void RemoveXRuler();
	    void RemoveYRuler();
	    void RemoveHScrollGroup();
	    void RemoveVScrollGroup();

	    void SetFlag(bool f);
	    
	    QFont& RulerFont();
	    void SetYRulerWidth(int);

	signals:
	    void HZoomIn();
	    void HZoomOut();
	    void HScrollValue(int);
	    void HZoomRatio(double);
	    void HMaxScroll(int);
	    void UpdatedHScroll(int);
	    void HScrollClicked();
	    void HScrollReleased();

	    void VZoomIn();
	    void VZoomOut();
	    void VScrollValue(int);
	    void VZoomRatio(double);
	    void VMaxScroll(int);
	    void UpdatedVScroll(int);
	    void VScrollClicked();
	    void VScrollReleased();

	    void xRulerRange(double, double);
	    void yRulerRange(double, double);

	    void switchColorsRequested();

	    void selPos(TData);

	    void insertedMark(unsigned);
	    void removedMark(int, unsigned);
	    void updatedMark(int, unsigned);

	    void selectedXPos(double);
	    
	public slots:
	    void hZoomIn();
	    void hZoomOut();
	    void hScrollValue(int);
	    void receivedHZoomRatio(double);
	    void setMaxHScroll(int);
	    void updateHScroll(int);
				
	    void vZoomIn();
	    void vZoomOut();				
	    void vScrollValue(int);
	    void receivedVZoomRatio(double);
	    void setMaxVScroll(int);
	    void updateVScroll(int);
				
	    void setXRulerRange(double, double);
	    void setYRulerRange(double, double);

	    void switchColors();

	    void setSelPos(TData);

	    void insertMark(unsigned);
	    void removeMark(int, unsigned);
	    void updateMark(int, unsigned);

	protected slots:
	    virtual void initialYRulerRange(double, double);

	protected:
	    PlotController* _controller;
				
	    void SetController(PlotController* controller);

	    void AddToMainLayout(QLayout* layout);
	    void RemoveFromMainLayout(QLayout* layout);

	    virtual void SetPlotController()=0;
	    virtual void Connect()=0;

	    virtual void DisplayBackgroundBlack()=0;
	    virtual void DisplayBackgroundWhite()=0;

	    int YRulerWidth();

	private:
	    DisplaySurface* _surf;
	    QBoxLayout *_mainLayout,*_top,*_middle,*_bottom;
	    Ruler *_xRuler, *_yRuler; 
	    HScrollGroup* _hs;
	    VScrollGroup* _vs;

	    QPushButton* _btoggle_color;

	    bool flag;

	    // holes
	    QFrame *topLeftHole,*topRightHole;
	    QFrame *bottomLeftHole,*bottomRightHole;

	    void Init();
	};
    }
}

#endif

