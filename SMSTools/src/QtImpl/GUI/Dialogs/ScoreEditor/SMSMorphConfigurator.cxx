#include <CLAM/Factory.hxx>
#include "Qt_Help_View.hxx"
#include "Qt_SMS_Morph_Control.hxx"
#include "SMSMorphConfigurator.hxx"

namespace QtSMS
{
	static CLAM::FactoryRegistrator<SMSConfiguratorFactory, SMSMorphConfigurator> regtPitchShiftCfg("SMSMorph");

	const char* SMSMorphConfigurator::mHelpText = 
	"<html>"
	"<body>"
	"<h2>Morph</h2>"
	"<p>Allows to 'morph' or hybridize two sounds, so the resulting one has intermediate "
	"characteristics. Control on the actual blending is provided to the user through a set "
	"of envelopes that define the evolution in time of several controls. These controls "
	"are:</p>"
	"<p><strong>Sinusoidal component amplitude: </strong>"
	"Allows to control an aspect of the timbre blending performed, concretely the amplitude "
	"of the sinusoidal component, that is the amplitude of the spectral peaks of the "
	"resulting sound expressed as a interpolation factor between the peaks of the two original "
	"sounds. The X axis is time, expressed in a parametric form,  and the Y axis is the "
	"interpolation factor between the two sinusoidal amplitude envelopes. An interpolation "
	"factor of 0.0 means that the resulting sound sinusoidal envelope will match the source's "
	"one, and a factor of 1.0 means that the resulting sound sinusoidal envelope matches "
	"exactly the target's one.</p>"
	"<p><strong>Sinusoidal component frequency hybridization: </strong>"
	"Similarly to the previous one, it allows to control an aspect of the timbre blending "
	"performed, concretely the frequency of the sinusoidal component, that is the frequency "
	"of the spectral peaks of the resulting sound expressed as a interpolation factor between "
	"the peaks of the two original sounds. The X axis is time, expressed in a parametric form,  "
	"and the Y axis is the interpolation factor between the two frequencies of peaks in the "
	"sinusoidal component. An interpolation factor of 0.0 means that the resulting sound "
	"sinusoidal frequencies will match the source's one, and a factor of 1.0 means that the "
	"resulting sound sinusoidal envelope matches exactly the target's one. Note that this "
	"control has rather weak effect on harmonic sounds analyzed as such. In that case the "
	"effect will be driven by the value in the pitch hybridation factor except in frames "
	"where no pitch has been found, it is then recommended that you synchronize this factor "
	"to pitch.</p>"
	"<p><strong>Pitch Hybridization: </strong>"
	"Allows to control sounds' pitches blending. Note that this involves not only to blend "
	"the fundamental frequency, but also any accompanying harmonics detected as tready pointed "
	"out in the previous control. The X axis is time, expressed in a parametric form, and the "
	"Y axis is the interpolation factor between the two sounds pitches. An interpolation "
	"factor of 0.0 means that the resulting sound harmonics will match the source's one, " 
	"and a factor of 1.0 means that the resulting sound harmonics will match exactly the "
	"target's one in their frequency distributions.</p>"
	"<p><strong>Residual component amplitude hybridization:</strong>"
	"This widget allows you to control the blending of the two sounds residual component "
	"amplitude envelope. The X axis represents transformation time, in a parametric form, and "
	"the Y axis represents the interpolation factor between the two sounds hoarseness. So a "
	"interpolation factor of 0.0 means that the resulting sound noisy component will match "
	"source's one, and an interpolation factor of 1.0 means that the resulting sound residual "
	"will  match target's one.</p>"
	"<p>Finally, for the sake of comodity, a <strong>'global' envelope control</strong> is "
	"provided, that allows to define morph behaviour across all dimensions. All the previously "
	"introduced controls can be synchronized to the global envelope thus becoming non editable "
	"(note that this is the default behaviour)</p>"
	"<p>The option to <strong> interpolate 'intermediate' frames </strong> is given so to "
	"define SMS Morph behaviour when source and target morph sounds' lenghts differ. If it is "
	"deactivated, Tools will resample shorter sound interpolating missing frames from the "
	"neighbourghing ones. Otherwise, the resampling will be accomplished by always taking the "
	"closest frame so sometimes frames will be repeated. Transformation result will sound "
	"better with linear interpolation but also will be a bit slower.</p>"
	"<p>The controls we will explain from now on could be considered as 'advanced' and we "
	"cannot hope their meaning to become completely clear in this context, without some usage "
	"tutorial, we hope though that with this brief description and your experimenting, their "
	"usage can become clear <p>"
	"<p>The <strong>Time Synchronization</strong> defines a temporal mapping between the "
	"source and the target sound. X axis is the normalized time for the source sound and Y is "
	"the normalized time for the target. Using this control you can achieve morphs on only "
	"some parts of the sound, define how you want to stretch the shortest sound or even "
	"reverse one of the sounds in respect to the other. </p>"
	"<p> The following three parameters form a non-separable set: <strong> SinusoidalShapeW1, "
	"SinusoidalShapeW2 and SinusoidalShapeInterpolationFactor </strong>. The first two define "
	"spectral envelopes where, and for every frequency, a Y value of 0 means that the "
	"amplitude of that frequency will be taken from the source spectrum and a value of 1 "
	"means that the amplitude should be that of the target spectrum. The third control is a "
	"temporal envelope that defines how much of each of the previous spectral shapes should "
	"be taken in every instant. An instantaneous value of 0 in this temporal envelope means "
	"that SinusoidalShape1 should be used and a value of 1 means that the SinusoidalShape2 "
	"should be used. </p>"
	"<p>The same happens with the controls <strong> ResidualShapeW1, ResidualShapeW2 and "
	"ResidualShapeInterpolationFactor </strong> although this spectral shape interpolation "
	"in the residual spectrum has not been implemented in the current release. </p>"
	"</body></html>";

	SMSMorphConfigurator::SMSMorphConfigurator()
	{
		mHelpWidget = new Qt_Help_View;
		mHelpWidget->SetText(mHelpText);

		mMorphEditor = new Qt_SMS_Morph_Control;
		connect(mMorphEditor,SIGNAL(configurationChanged()),this,SIGNAL(configurationChanged()));
	}

	SMSMorphConfigurator::~SMSMorphConfigurator()
	{
		delete mHelpWidget;
		delete mMorphEditor;
	}

	QWidget* SMSMorphConfigurator::GetParametersWidget()
	{
		return mMorphEditor;
	}

	QWidget* SMSMorphConfigurator::GetHelpWidget()
	{
		return mHelpWidget;
	}

	void SMSMorphConfigurator::Initialize( CLAM::ProcessingConfig& cfg)
	{
		CLAM::SMSMorphConfig& conCfg = static_cast<CLAM::SMSMorphConfig&>(cfg);

		if(conCfg.HasHybBPF())
		{
			conCfg.RemoveHybBPF();
			conCfg.UpdateData();
		}
		conCfg.AddHybBPF();
		conCfg.UpdateData();
		conCfg.GetHybBPF().Insert(0.0, 0.5);
		conCfg.GetHybBPF().Insert(1.0, 0.5);
	}

	void SMSMorphConfigurator::SetConfig( const CLAM::ProcessingConfig& cfg)
	{
		mConfig = static_cast<const CLAM::SMSMorphConfig&>(cfg);
		bool ckecked = (mConfig.GetInterpolateFrame()) ? true : false;
		mMorphEditor->FrameInterpolationChecked(ckecked);
		mMorphEditor->SetGlobalEnvelope(mConfig.GetHybBPF());
		mMorphEditor->SetSinAmpEnvelope(mConfig.GetHybSinAmp());
		mMorphEditor->SetSinFreqEnvelope(mConfig.GetHybSinFreq());
		mMorphEditor->SetPitchHybEnvelope(mConfig.GetHybPitch());
		mMorphEditor->SetResAmpEnvelope(mConfig.GetHybResAmp());
		mMorphEditor->SetTimeSyncEnvelope(mConfig.GetSynchronizeTime());
		mMorphEditor->SetSinShapeW1Envelope(mConfig.GetHybSinShapeW1());
		mMorphEditor->SetSinShapeW2Envelope(mConfig.GetHybSinShapeW2());
		mMorphEditor->SetResShapeW1Envelope(mConfig.GetHybResShapeW1());
		mMorphEditor->SetResShapeW2Envelope(mConfig.GetHybResShapeW2());
		mMorphEditor->SetSinShapeEnvelope(mConfig.GetHybSinSpectralShape());
		mMorphEditor->SetResShapeEnvelope(mConfig.GetHybResSpectralShape());
	}

	const CLAM::ProcessingConfig& SMSMorphConfigurator::GetConfig()
	{
		SetupConfigObject();
		return mConfig;
	}

	void SMSMorphConfigurator::SetupConfigObject()
	{
		mConfig.SetInterpolateFrame(mMorphEditor->FrameInterpolationChecked());
		mMorphEditor->RetrieveGlobalEnvelope(mConfig.GetHybBPF());
		mMorphEditor->RetrieveSinFreqEnvelope(mConfig.GetHybSinFreq());
		mMorphEditor->RetrieveSinAmpEnvelope(mConfig.GetHybSinAmp());
		mMorphEditor->RetrievePitchHybEnvelope(mConfig.GetHybPitch());
		mMorphEditor->RetrieveResAmpEnvelope(mConfig.GetHybResAmp());
		mMorphEditor->RetrieveTimeSyncEnvelope(mConfig.GetSynchronizeTime());
		mMorphEditor->RetrieveSinShapeW1Envelope(mConfig.GetHybSinShapeW1());
		mMorphEditor->RetrieveSinShapeW2Envelope(mConfig.GetHybSinShapeW2());
		mMorphEditor->RetrieveResShapeW1Envelope(mConfig.GetHybResShapeW1());
		mMorphEditor->RetrieveResShapeW2Envelope(mConfig.GetHybResShapeW2());
		mMorphEditor->RetrieveSinShapeEnvelope(mConfig.GetHybSinSpectralShape());
		mMorphEditor->RetrieveResShapeEnvelope(mConfig.GetHybResSpectralShape());
	}

}

// END

