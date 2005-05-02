#ifndef __BPFEDITORCONTROLLER__
#define __BPFEDITORCONTROLLER__

#include <list>
#include <qobject.h>
#include <qstring.h>
#include <qcursor.h>
#include "BPF.hxx"
#include "Point.hxx"
#include "GLView.hxx"
#include "BPFEditorRenderer.hxx"

namespace CLAM
{
    namespace VM
    {
	enum EFlags { AllowVerticalEdition=0x01, AllowHorizontalEdition=0x02, AllowInsert=0x04, AllowRemove=0x08, 
		      AllowZoomByMouse=0x10, HasVerticalScroll=0x20, HasHorizontalScroll=0x40, HasPlayer=0x80,
		      AllowAll=0xff };

	class BPFEditorController : public QObject
	{
	    struct RulerRange
	    {
		double mMin, mMax;
		
		RulerRange()
		    : mMin(0.0), mMax(1.0)
		    {
		    }
		RulerRange(const double& min, const double& max)
		    : mMin(min), mMax(max)
		    {
		    }
		RulerRange(const RulerRange& range) 
		    : mMin(range.mMin), mMax(range.mMax)
		    {
		    }
		~RulerRange(){}

		RulerRange& operator=(const RulerRange& range)
		    {
			mMin = range.mMin;
			mMax = range.mMax;
			return *this;
		    }
	    };

	    struct BPFEditorSettings
	    {
		RulerRange mXRange, mYRange;
		GLView mView;

		BPFEditorSettings(){}
		BPFEditorSettings(const RulerRange& xRange, const RulerRange& yRange, const GLView& view)
		    : mXRange(xRange), mYRange(yRange), mView(view) 
		    {
		    }
		BPFEditorSettings(const BPFEditorSettings& settings)
		    : mXRange(settings.mXRange), mYRange(settings.mYRange), mView(settings.mView)
		    {
		    }
		~BPFEditorSettings(){}

		BPFEditorSettings& operator=(const BPFEditorSettings& settings)
		    {
			mView = settings.mView;
			mXRange = settings.mXRange;
			mYRange = settings.mYRange;
			return *this;
		    }
	    };

	    struct Pixel
	    {
		unsigned x, y;
	    };

	    typedef std::list<BPFEditorSettings> SettingStack;

	     Q_OBJECT

	public:
	    BPFEditorController(int eFlags=CLAM::VM::AllowAll);
	    ~BPFEditorController();

	    void SetData(const BPF& bpf);
	    BPF& GetData();
	        
	    void SetDataColor(const Color& c);
	    void SetHandlersColor(const Color& c);
	    void SetRectColor(const Color& c);

	    void SetXRange(const double& min, const double& max, const EScale& scale=EScale::eLinear);
	    void SetYRange(const double& min, const double& max, const EScale& scale=EScale::eLinear);

	    void SetXScale(const EScale& scale);
	    void SetYScale(const EScale& scale);

	    void SetKeyInsertPressed(bool pressed);
	    void SetKeyDeletePressed(bool pressed);
	    
	    void SetLeftButtonPressed(bool pressed);
	    void SetRightButtonPressed(bool pressed);

	    void SetPoint(const TData& x, const TData& y);
	    void UpdatePoint(const TData& x, const TData& y);
	    void MoveCurrentPointDelta(int stepX, int stepY);
	    void ChooseCurrentPointByJumping(int step);

	    void MouseOverDisplay(bool over);

	    void Draw();

	    void DisplayDimensions(const int& w, const int& h);

	    void SetHBounds(double left, double right);
	    void SetVBounds(double bottom, double top);

	    void SelectPointFromXCoord(double xcoord);

	signals:
	    void viewChanged(GLView);
	    void cursorChanged(QCursor);
	    void xRulerRange(double, double);
	    void yRulerRange(double, double);
	    void labelsText(QString, QString);
	    void requestRefresh();

	    void xValueChanged(int, float);
	    void yValueChanged(int, float);

	    void selectedXPos(double);

	    void vZoomRatio(double);
	    void vScrollMaxValue(int);
	    void vScrollValue(int);

	    void elementAdded(int, float, float);
	    void elementRemoved(int);

	    void rightButtonPressed();

	public slots:
	    void vZoomIn();
	    void vZoomOut();

	    void updateVScrollValue(int);

	private:
	    BPF mData;
	    BPFEditorRenderer mRenderer;
	    GLView mView;
	    RulerRange mXRulerRange, mYRulerRange;
	    int mEFlags;
	    EScale mXScale, mYScale;
	    bool mLeftButtonPressed, mRightButtonPressed;
	    bool mKeyInsertPressed, mKeyDeletePressed;
	    bool mMouseOverDisplay;
	    bool mProcessingSelection;
	    bool mHit;
	    Color mRectColor;
	    Point mCorners[2];
	    SettingStack mSettingStack;
	    int mDisplayWidth, mDisplayHeight;
       
	    enum { Selection=0, Edition };

	    Point mCurrentPoint;
	    TIndex mCurrentIndex;

	    bool mXModified, mYModified;

	    bool mSelectPoint;
	    double mSpanX,mSpanY;
	    double mMinSpanX,mMinSpanY;
	    double mMinX,mMaxX;
	    double mMinY,mMaxY;

	    double mVCurrent;
	    double mVZoomRatio;
	    
	    void PushSettings();
	    void PopSettings();

	    void DrawRect();

	    void ProcessData();

	    double XMin();
	    double XMax();
	    double YMin();
	    double YMax();

	    double Max(double a, double b);
	    double Min(double a, double b);

	    Pixel GetPixel(const TData& x, const TData& y);
	    bool Match(const Pixel& p, const Pixel& q);
	    int Hit(const TData& x, const TData& y);
	    void UpdateXYLabels(TData x, TData y);

	    int GetMode();
	    
	    void UpdateBPF(TData x, TData y);

	    bool IsValid(const TData& x0, const TData& x1);

	    int GetnyPixels() const;
	    int GetVScrollValue() const;

	    void InitVZoomRatio();
	    void InitVScroll();

	    void UpdateVBounds(bool zin);

	    TIndex GetLeftBound();
	    TIndex GetRightBound();
	};
    }
}

#endif

