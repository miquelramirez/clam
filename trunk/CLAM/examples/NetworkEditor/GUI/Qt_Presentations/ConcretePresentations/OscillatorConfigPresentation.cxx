
#include "OscillatorConfigPresentation.hxx"
#include <qlabel.h>
#include <qtextedit.h>
#include <qgroupbox.h>  
#include "ProcessingConfig.hxx"
#include "Oscillator.hxx"

namespace NetworkGUI
{

OscillatorConfigPresentation::OscillatorConfigPresentation( QWidget * parent )
	: Qt_ProcessingConfigPresentation( parent , "config" ),
	  mConfig(0)
{	

	resize( QSize(209, 220).expandedTo(minimumSizeHint()) );

	mFrequencyName = new QLabel( "Frequency" , this );
	mFrequencyName->setGeometry( QRect ( 10, 20,  120, 25 ) );

	mAmplitudeName = new QLabel( "Amplitude", this );
	mAmplitudeName->setGeometry( QRect ( 10, 50, 120, 25 ) );

	mModIndexName = new QLabel( "Modulation Index" , this );
	mModIndexName->setGeometry( QRect (  10, 80, 120, 25 ) );

	mPhaseName = new QLabel( "Phase", this );
	mPhaseName->setGeometry( QRect (  10, 110, 120, 25 ) );

	mSamplingRateName = new QLabel( "Sampling Rate", this );
	mSamplingRateName->setGeometry( QRect (  10, 140, 120, 25 ) );

	mGroupBox->setGeometry( QRect( 10, 170, 190, 40 ) );

	mFrequencyEdit = new QTextEdit(this);
	mFrequencyEdit->setGeometry( 120, 20 , 60, 20);
	mAmplitudeEdit = new QTextEdit(this);
	mAmplitudeEdit->setGeometry( 120, 50, 60, 20);
	mModIndexEdit = new QTextEdit(this);
	mModIndexEdit->setGeometry( 120, 80, 60, 20);
	mPhaseEdit = new QTextEdit(this);
	mPhaseEdit->setGeometry( 120, 110, 60, 20);
	mSamplingRateEdit = new QTextEdit(this);
	mSamplingRateEdit->setGeometry( 120, 140 , 60, 20);
       
}

OscillatorConfigPresentation::~OscillatorConfigPresentation()
{
}

void OscillatorConfigPresentation::Show()
{	
	if (mConfig)
		setCaption( "Oscillator Configuration" );
	else
		setCaption( "Unconfigured" );

	QString str;
	mFrequencyEdit->setText(str.sprintf("%7.2f", mConfig->GetFrequency()) );
	mAmplitudeEdit->setText(str.sprintf("%7.2f", mConfig->GetAmplitude()) );
	mModIndexEdit->setText(str.sprintf("%7.2f", mConfig->GetModIndex()) );
	mPhaseEdit->setText(str.sprintf("%7.2f", mConfig->GetPhase()) );
	mSamplingRateEdit->setText(str.sprintf("%7.2f", mConfig->GetSamplingRate()) );

	show();
}

void OscillatorConfigPresentation::Hide()
{
	hide();
}

void OscillatorConfigPresentation::ApplyChangesToConfig()
{

	mConfig->SetFrequency( mFrequencyEdit->text().toFloat());
	mConfig->SetAmplitude( mAmplitudeEdit->text().toFloat());
	mConfig->SetModIndex( mModIndexEdit->text().toFloat());
	mConfig->SetPhase( mPhaseEdit->text().toFloat());
	mConfig->SetSamplingRate( mSamplingRateEdit->text().toFloat());
	
	ApplyConfig.Emit(mConfig);
}

void OscillatorConfigPresentation::OnNewConfig( CLAM::ProcessingConfig* cfg)
{
	mConfig = (CLAM::OscillatorConfig*)cfg;
}

} // namespace NetworkGUI

