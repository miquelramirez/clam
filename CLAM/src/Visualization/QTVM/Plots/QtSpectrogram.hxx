#ifndef __QTSPECTROGRAM__
#define __QTSPECTROGRAM__

#include "Spectrum.hxx"
#include "QtPresentation.hxx"

class QLabel;

namespace CLAM
{
    namespace VM
    {

	class ColorScale;

	/**
	 * Allows viewing sonogram.
	 *
	 * @ingroup QTVM
	 */

	class QtSpectrogram : public QtPresentation
	{
	    Q_OBJECT

	public:
	    QtSpectrogram(QWidget* parent=0,const char * name = 0, WFlags f = 0);
	    ~QtSpectrogram();

	    void SetData(const Array<Spectrum>& specMtx, const TData& dur);

	    void SetMarks(std::vector<unsigned>& marks);
	    std::vector<unsigned>& GetMarks();
	    void SetMarksColor(Color c);
	    
	    std::vector<QString> GetSegmentationTags();

	protected slots:
	    void updateLabels(QString, QString, QString);
	    void updateColorScale(int);

	protected:
	    void keyPressEvent(QKeyEvent* e);
	    void keyReleaseEvent( QKeyEvent* e);
	    void SetPlotController();				
	    void Connect();

	    void DisplayBackgroundBlack();
	    void DisplayBackgroundWhite();

	private:
	    QLabel *mFrequency, *mDecibels, *mSlice;
	    QLabel *mFFTSize, *mTotalSlices, *mSpectralRange;
	    ColorScale* mColorScale;
	    bool mColorSonogram;

	    void InitSpectrogram();

	    QWidget* LeftGroup();
	    QWidget* RightGroup();

	    void FillRightGroupLabels();
	};
    }
}

#endif



