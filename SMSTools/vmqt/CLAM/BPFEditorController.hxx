#ifndef __BPFEDITORCONTROLLER__
#define __BPFEDITORCONTROLLER__

#include <map>
#include <vector>
#include <string>
#include <list>
#include <qobject.h>
#include <qstring.h>
#include <qcursor.h>
#include <CLAM/BPF.hxx>
#include <CLAM/Point.hxx>
#include <CLAM/GLView.hxx>
#include <CLAM/BPFEditorRenderer.hxx>
#include <CLAM/Dial.hxx>

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
					: mMin(0.0) 
					, mMax(1.0)
					{
					}
				RulerRange(const double& min, const double& max)
					: mMin(min)
					, mMax(max)
					{
					}
				RulerRange(const RulerRange& range) 
					: mMin(range.mMin)
					, mMax(range.mMax)
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
					: mXRange(xRange)
					, mYRange(yRange)
					, mView(view) 
					{
					}
				BPFEditorSettings(const BPFEditorSettings& settings)
					: mXRange(settings.mXRange)
					, mYRange(settings.mYRange)
					, mView(settings.mView)
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

			typedef std::map<std::string, unsigned> IndexTable;
			typedef std::vector<BPF>                BPFs;
			typedef std::vector<BPFEditorRenderer>  Renderers;
			typedef std::list<BPFEditorSettings>   SettingStack;

			Q_OBJECT

		public:
			BPFEditorController(int eFlags=CLAM::VM::AllowAll);
			~BPFEditorController();

			void AddData(const std::string& key, const BPF& bpf);
			BPF& GetData(const std::string& key);

	        void SetCurrentBPF(const std::string& key);
			void SetDataColor(const std::string& key, const Color& lines_color, const Color& handlers_color);
			void SetDataColor(const Color& lines_color, const Color& handlers_color);
			void SetRectColor(const Color& c);
			void SetDialColor(const Color& c);

			void SetXRange(const double& min, const double& max, const EScale& scale=EScale::eLinear);
			void SetYRange(const double& min, const double& max, const EScale& scale=EScale::eLinear);

			void SetXScale(const EScale& scale);
			void SetYScale(const EScale& scale);

			void SetKeyInsertPressed(bool pressed);
			void SetKeyDeletePressed(bool pressed);
			void SetKeyControlPressed(bool pressed);
	    
			void SetLeftButtonPressed(bool pressed);
			void SetRightButtonPressed(bool pressed);

			void SetPoint(const TData& x, const TData& y);
			void UpdatePoint(const TData& x, const TData& y);
			void MoveCurrentPointDelta(int stepX, int stepY);
			void ChooseCurrentPointByJumping(int step);
			void ChooseCurrentPoint(int pointIndex);

			void Draw();

			void DisplayDimensions(const int& w, const int& h);

			void SetHBounds(double left, double right);
			void SetVBounds(double bottom, double top);

			void SelectPointFromXCoord(double xcoord);

			void UpdateTimePos(const TData& time);
			void StopPlaying(const TData& time);

			void MouseOverDisplay(bool over);
			
			bool HasMultipleBPF() const;
			std::list<QString> GetBPFNamesList();

			void ActiveRendering(bool active);

			void ActiveDial(bool active);
			void ShowGrid(bool show);
			void SnapToGrid(bool snap);
			void SetGridSteps(double xstep, double ystep);
			void SetGridColor(const Color& c);

			void AllowInsertPoints(bool allow);
			void AllowDeletePoints(bool allow);
			void AllowModifyPoints(bool allow);

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
			void hZoomRatio(double);
			void hScrollMaxValue(int);
			void hScrollValue(int);
			void elementAdded(int, float, float);
			void elementRemoved(int);
			void rightButtonPressed();
			void startPlaying();
			void stopPlaying();
			void currentPlayingTime(float);
			void stopPlaying(float);
			void pointsChanged();

		public slots:
			void vZoomIn();
			void vZoomOut();
			void hZoomIn();
			void hZoomOut();
			void updateVScrollValue(int);
			void updateHScrollValue(int);

			void updateXValue(int, float);
			void updateYValue(int, float);
			void addElement(int, float, float);
			void removeElement(int);

		private:
			IndexTable   mIndexTable;
			BPFs         mBPFs;
			Renderers    mRenderers;
			SettingStack mSettingStack;
			Dial         mDial;
			GLView       mView;
			RulerRange   mXRulerRange;
			RulerRange   mYRulerRange;
			int          mEFlags;
			EScale       mXScale;
			EScale       mYScale;
			bool         mLeftButtonPressed;
			bool         mRightButtonPressed;
			bool         mKeyInsertPressed;
			bool         mKeyDeletePressed;
			bool         mKeyControlPressed;
			bool         mMouseOverDisplay;
			bool         mProcessingSelection;
			bool         mHit;
			bool         mQueryZoomOut;
			Color        mRectColor;
			Point        mCorners[2];
			int          mDisplayWidth;
			int          mDisplayHeight;
			Point        mCurrentPoint;
			TIndex       mCurrentIndex;
			bool         mXModified;
			bool         mYModified;
			bool         mSelectPoint;
			double       mSpanX;
			double       mSpanY;
			double       mMinSpanX;
			double       mMinSpanY;
			double       mMinX;
			double       mMaxX;
			double       mMinY;
			double       mMaxY;
			double       mVCurrent;
			double       mHCurrent;
			double       mVZoomRatio;
			double       mHZoomRatio;
			bool         mIsPlaying;
			TIndex       mLightedPointIndex;
			std::string  mCurrentBPF;
			bool         mIsModified;
			bool         mActiveRendering;

			bool         mActiveDial;
			bool         mShowGrid;
			bool         mSnapToGrid;
			double       mXGridStep;
			double       mYGridStep;
			Color        mGridColor;

			bool         mAllowInsertPoints;
			bool         mAllowDeletePoints;
			bool         mAllowModifyPoints;
       
			enum { Selection=0, Edition=1, LINE_WIDTH=1 };
	    
			void PushSettings();
			void PopSettings();

			void DrawRect();

			double SelectionBoxMinX();
			double SelectionBoxMaxX();
			double SelectionBoxMinY();
			double SelectionBoxMaxY();

			double Max(double a, double b);
			double Min(double a, double b);

			Pixel GetPixel(const TData& x, const TData& y);
			bool Match(const Pixel& p, const Pixel& q, TData tolerance);
			int Hit(const TData& x, const TData& y);
			void UpdateXYLabels(TData x, TData y);

			int GetMode();
	    
			void UpdateBPF(TData x, TData y);

			int GetnyPixels() const;
			int GetVScrollValue() const;

			void InitVZoomRatio();
			void InitVScroll();

			void UpdateVBounds(bool zin);

			int GetnxPixels() const;
			int GetHScrollValue() const;

			void InitHZoomRatio();
			void InitHScroll();

			void UpdateHBounds(bool zin);

			TIndex GetBound(unsigned bpf_index, const TData& searchValue, bool left=true);

			void InsertBPFNode(TData x, TData y);

			bool ReferenceIsVisible();
			double GetReference();

			void InitTables();
			void AddBPF(const std::string& key, const BPF& bpf);
			bool HaveKey(const std::string& key);
			unsigned GetBPFIndex(const std::string& key);

			void Render();
			void CheckForModifiedBPF();
			void SetRenderersBounds();
			void CheckReferencePoint();

			void DrawGrid();
			double round(double x) const;

			void SetMasterRenderer();
			void ActiveMasterRenderer();
		};
    }
}

#endif

