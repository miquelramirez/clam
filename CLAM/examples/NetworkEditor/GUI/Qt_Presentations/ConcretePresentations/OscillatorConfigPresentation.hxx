
#ifndef __OSCILLATORCONFIGPRESENTATION_HXX__
#define __OSCILLATORCONFIGPRESENTATION_HXX__

#include "Qt_ProcessingConfigPresentation.hxx"

class QLabel;
class QTextEdit;

namespace CLAM
{
	class OscillatorConfig;
	class ProcessingConfig;
}

namespace NetworkGUI
{

class OscillatorConfigPresentation : public Qt_ProcessingConfigPresentation
{
	CLAM::OscillatorConfig * mConfig;
	
	QLabel * mFrequencyName;
	QLabel * mAmplitudeName;
	QLabel * mModIndexName;
	QLabel * mPhaseName;
	QLabel * mSamplingRateName;

	QTextEdit * mFrequencyEdit;
	QTextEdit * mAmplitudeEdit;
	QTextEdit * mModIndexEdit;
	QTextEdit * mPhaseEdit;
	QTextEdit * mSamplingRateEdit;
protected:
	virtual void OnNewConfig( CLAM::ProcessingConfig* );
	virtual void ApplyChangesToConfig();
public:
	OscillatorConfigPresentation( QWidget * parent = 0 );
	virtual ~OscillatorConfigPresentation();
	virtual void Show();
	virtual void Hide();
};

} // namespace NetworkGUI

#endif // __OSCILLATORCONFIGPRESENTATION_HXX__
