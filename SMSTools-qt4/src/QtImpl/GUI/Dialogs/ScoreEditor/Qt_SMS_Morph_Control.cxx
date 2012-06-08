#include <qlayout.h>
#include <qcheckbox.h>
#include <qcombobox.h>
#include <qpushbutton.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include "Qt_SMS_BPF_Editor.hxx"
#include "Qt_SMS_BPF_Editor_Sync.hxx"
#include "Qt_SMS_Morph_Control.hxx"

namespace QtSMS
{
	Qt_SMS_Morph_Control::Qt_SMS_Morph_Control(QWidget* parent)
		: QWidget(parent)
	{
		InitSegmentSMSMorphControl();
	}

	Qt_SMS_Morph_Control::~Qt_SMS_Morph_Control()
	{
		mBPFEditorList.clear();
	}

	void Qt_SMS_Morph_Control::RetrieveGlobalEnvelope(CLAM::BPF& bpf)
	{
		bpf = ((Qt_SMS_BPF_Editor*)mBPFEditorList[GLOBAL_ENVELOPE])->GetBPF();
	}

	void Qt_SMS_Morph_Control::SetGlobalEnvelope(const CLAM::BPF& bpf)
	{
		((Qt_SMS_BPF_Editor*)mBPFEditorList[GLOBAL_ENVELOPE])->Clear();
		((Qt_SMS_BPF_Editor*)mBPFEditorList[GLOBAL_ENVELOPE])->InitPoints(bpf);
	}

	void Qt_SMS_Morph_Control::RetrieveSinAmpEnvelope(CLAM::BPF& bpf)
	{
		bpf = ((Qt_SMS_BPF_Editor_Sync*)mBPFEditorList[SIN_AMP_ENVELOPE])->GetBPF();
	}

	void Qt_SMS_Morph_Control::SetSinAmpEnvelope(const CLAM::BPF& bpf)
	{
		((Qt_SMS_BPF_Editor_Sync*)mBPFEditorList[SIN_AMP_ENVELOPE])->Clear();
		((Qt_SMS_BPF_Editor_Sync*)mBPFEditorList[SIN_AMP_ENVELOPE])->InitPoints(bpf);
	}

	void Qt_SMS_Morph_Control::RetrievePitchHybEnvelope(CLAM::BPF& bpf)
	{
		bpf = ((Qt_SMS_BPF_Editor_Sync*)mBPFEditorList[PITCH_HYB_ENVELOPE])->GetBPF();
	}

	void Qt_SMS_Morph_Control::SetPitchHybEnvelope(const CLAM::BPF& bpf)
	{
		((Qt_SMS_BPF_Editor_Sync*)mBPFEditorList[PITCH_HYB_ENVELOPE])->Clear();
		((Qt_SMS_BPF_Editor_Sync*)mBPFEditorList[PITCH_HYB_ENVELOPE])->InitPoints(bpf);
	}

	void Qt_SMS_Morph_Control::RetrieveSinFreqEnvelope(CLAM::BPF & bpf)
	{
		bpf = ((Qt_SMS_BPF_Editor_Sync*)mBPFEditorList[SIN_FREQ_ENVELOPE])->GetBPF();
	}

	void Qt_SMS_Morph_Control::SetSinFreqEnvelope(const CLAM::BPF& bpf)
	{
		((Qt_SMS_BPF_Editor_Sync*)mBPFEditorList[SIN_FREQ_ENVELOPE])->Clear();
		((Qt_SMS_BPF_Editor_Sync*)mBPFEditorList[SIN_FREQ_ENVELOPE])->InitPoints(bpf);
	}

	void Qt_SMS_Morph_Control::RetrieveResAmpEnvelope(CLAM::BPF& bpf)
	{
		bpf = ((Qt_SMS_BPF_Editor_Sync*)mBPFEditorList[RES_AMP_ENVELOPE])->GetBPF();
	}

	void Qt_SMS_Morph_Control::SetResAmpEnvelope(const CLAM::BPF& bpf)
	{
		((Qt_SMS_BPF_Editor_Sync*)mBPFEditorList[RES_AMP_ENVELOPE])->Clear();
		((Qt_SMS_BPF_Editor_Sync*)mBPFEditorList[RES_AMP_ENVELOPE])->InitPoints(bpf);
	}

	void Qt_SMS_Morph_Control::RetrieveTimeSyncEnvelope(CLAM::BPF& bpf)
	{
		bpf = ((Qt_SMS_BPF_Editor*)mBPFEditorList[TIME_SYNC_ENVELOPE])->GetBPF();
	}

	void Qt_SMS_Morph_Control::SetTimeSyncEnvelope(const CLAM::BPF& bpf)
	{
		((Qt_SMS_BPF_Editor*)mBPFEditorList[TIME_SYNC_ENVELOPE])->Clear();
		((Qt_SMS_BPF_Editor*)mBPFEditorList[TIME_SYNC_ENVELOPE])->InitPoints(bpf);
	}

	void Qt_SMS_Morph_Control::RetrieveSinShapeW1Envelope(CLAM::BPF& bpf)
	{
		bpf = ((Qt_SMS_BPF_Editor*)mBPFEditorList[SIN_SHAPE_W1_ENVELOPE])->GetBPF();
	}

	void Qt_SMS_Morph_Control::SetSinShapeW1Envelope(const CLAM::BPF& bpf)
	{
		((Qt_SMS_BPF_Editor*)mBPFEditorList[SIN_SHAPE_W1_ENVELOPE])->Clear();
		((Qt_SMS_BPF_Editor*)mBPFEditorList[SIN_SHAPE_W1_ENVELOPE])->InitPoints(bpf);
	}

	void Qt_SMS_Morph_Control::RetrieveSinShapeW2Envelope(CLAM::BPF& bpf)
	{
		bpf = ((Qt_SMS_BPF_Editor*)mBPFEditorList[SIN_SHAPE_W2_ENVELOPE])->GetBPF();
	}

	void Qt_SMS_Morph_Control::SetSinShapeW2Envelope(const CLAM::BPF& bpf)
	{
		((Qt_SMS_BPF_Editor*)mBPFEditorList[SIN_SHAPE_W2_ENVELOPE])->Clear();
		((Qt_SMS_BPF_Editor*)mBPFEditorList[SIN_SHAPE_W2_ENVELOPE])->InitPoints(bpf);
	}

	void Qt_SMS_Morph_Control::RetrieveResShapeW1Envelope(CLAM::BPF& bpf)
	{
		bpf = ((Qt_SMS_BPF_Editor*)mBPFEditorList[RES_SHAPE_W1_ENVELOPE])->GetBPF();
	}

	void Qt_SMS_Morph_Control::SetResShapeW1Envelope(const CLAM::BPF& bpf)
	{
		((Qt_SMS_BPF_Editor*)mBPFEditorList[SIN_SHAPE_W2_ENVELOPE])->Clear();
		((Qt_SMS_BPF_Editor*)mBPFEditorList[SIN_SHAPE_W2_ENVELOPE])->InitPoints(bpf);
	}

	void Qt_SMS_Morph_Control::RetrieveResShapeW2Envelope(CLAM::BPF& bpf)
	{
		bpf = ((Qt_SMS_BPF_Editor*)mBPFEditorList[RES_SHAPE_W2_ENVELOPE])->GetBPF();
	}

	void Qt_SMS_Morph_Control::SetResShapeW2Envelope(const CLAM::BPF& bpf)
	{
		((Qt_SMS_BPF_Editor*)mBPFEditorList[RES_SHAPE_W2_ENVELOPE])->Clear();
		((Qt_SMS_BPF_Editor*)mBPFEditorList[RES_SHAPE_W2_ENVELOPE])->InitPoints(bpf);
	}

	void Qt_SMS_Morph_Control::RetrieveSinShapeEnvelope(CLAM::BPF& bpf)
	{
		bpf = ((Qt_SMS_BPF_Editor*)mBPFEditorList[SIN_SHAPE_ENVELOPE])->GetBPF();
	}

	void Qt_SMS_Morph_Control::SetSinShapeEnvelope(const CLAM::BPF& bpf)
	{
		((Qt_SMS_BPF_Editor*)mBPFEditorList[SIN_SHAPE_ENVELOPE])->Clear();
		((Qt_SMS_BPF_Editor*)mBPFEditorList[SIN_SHAPE_ENVELOPE])->InitPoints(bpf);
	}

	void Qt_SMS_Morph_Control::RetrieveResShapeEnvelope(CLAM::BPF& bpf)
	{
		bpf = ((Qt_SMS_BPF_Editor*)mBPFEditorList[RES_SHAPE_ENVELOPE])->GetBPF();
	}

	void Qt_SMS_Morph_Control::SetResShapeEnvelope(const CLAM::BPF& bpf)
	{
		((Qt_SMS_BPF_Editor*)mBPFEditorList[RES_SHAPE_ENVELOPE])->Clear();
		((Qt_SMS_BPF_Editor*)mBPFEditorList[RES_SHAPE_ENVELOPE])->InitPoints(bpf);
	}

	bool Qt_SMS_Morph_Control::FrameInterpolationChecked() const
	{
		return mFrameInterpSelector->isChecked();
	}

	void Qt_SMS_Morph_Control::FrameInterpolationChecked(bool ckecked)
	{
		mFrameInterpSelector->setChecked(ckecked);
	}

	void Qt_SMS_Morph_Control::onEnvelopeSelected(int index)
	{
		for(unsigned i=0; i < mBPFEditorList.size(); i++) mBPFEditorList[i]->hide();
		mBPFEditorList[index]->show();
	}

	void Qt_SMS_Morph_Control::onWhatsThisButtonClicked()
	{
		QWhatsThis::enterWhatsThisMode();
	}
	
	void Qt_SMS_Morph_Control::InitEnvelopes()
	{
		// global envelope
		Qt_SMS_BPF_Editor* global_env = new Qt_SMS_BPF_Editor(this);
		global_env->SetHorizontalRange(0.0, 1.0);
		global_env->SetVerticalRange(0.0, 1.0);
		global_env->SetGridWidth(0.1, 0.1);
		global_env->WhatsThis("This widget allows to control the morph between the two sounds "
							  "being the X axis the time, and the Y axis the interpolation factor "
							  "so a factor of 0.0 means that the resulting sound is exactly the "
							  "source and a factor of 1.0 means that the resulting sound matches "
							  "exactly the target. By default this control is synchronized to Sinusoidal"
							  "Amplitude and Frequency and Residual Amplitude envelopes.");
		global_env->show();
		global_env->InitPoints(0.5);
		mBPFEditorList.push_back(global_env);

		// sin amp envelope
		Qt_SMS_BPF_Editor_Sync* sin_amp_env = new Qt_SMS_BPF_Editor_Sync(this);
		sin_amp_env->SetHorizontalRange(0.0, 1.0);
		sin_amp_env->SetVerticalRange(0.0, 1.0);
		sin_amp_env->SetGridWidth(0.1, 0.1);
		sin_amp_env->WhatsThis("Allows to control an aspect of the timbre blending performed," 
							   "concretely the amplitude of the sinusoidal component, that is the amplitude" 
							   "of the spectral peaks of the resulting sound expressed as a interpolation"
							   " factor between the peaks of the two original sounds. The X axis is time, "
							   "expressed in a parametric form,  and the Y axis is the interpolation factor "
							   "between the two sinusoidal amplitude envelopes. An interpolation factor of "
							   "0.0 means that the resulting sound sinusoidal envelope will match the "
							   "source's one, and a factor of 1.0 means that the resulting sound sinusoidal" 
							   "envelope matches exactly the target's one.");

		sin_amp_env->hide();
		sin_amp_env->InitPoints(0.5);   
		sin_amp_env->SetSyncToMasterChecked(true);
		mBPFEditorList.push_back(sin_amp_env);

		// sin freq envelope
		Qt_SMS_BPF_Editor_Sync* sin_freq_env = new Qt_SMS_BPF_Editor_Sync(this);
		sin_freq_env->SetHorizontalRange(0.0, 1.0);
		sin_freq_env->SetVerticalRange(0.0, 1.0);
		sin_freq_env->SetGridWidth(0.1, 0.1);
		sin_freq_env->WhatsThis("This envelope allows to control an aspect of the timbre "
								"blending performed, concretely the frequency of the sinusoidal component,"
								"that is the frequency of the spectral peaks of the resulting sound "
								"expressed as a interpolation factor between the peaks of the two original "
								"sounds. The X axis is time, expressed in a parametric form,  and the Y "
								"axis is the interpolation factor between the two frequencies of peaks "
								" in the sinusoidal component. An interpolation factor of 0.0 means that "
								" the resulting sound sinusoidal frequencies will match the source's one, "
								"and a factor of 1.0 means that the resulting sound sinusoidal envelope "
								"matches exactly the target's one. Note that this control has rather weak "
								"effect on harmonic sounds analyzed as such. In that case the effect will "
								"be driven by the value in the pitch hybridation factor except in frames "
								"where no pitch has been found, it is then recommended that you synchronize "
								"this factor to pitch.");
		sin_freq_env->hide();
		sin_freq_env->InitPoints(0.5);
		sin_freq_env->SetSyncToMasterChecked(true);
		mBPFEditorList.push_back(sin_freq_env);

		// res amp envelope,
		Qt_SMS_BPF_Editor_Sync* res_amp_env = new Qt_SMS_BPF_Editor_Sync(this);
		res_amp_env->SetHorizontalRange(0.0, 1.0);
		res_amp_env->SetVerticalRange(0.0, 1.0);
		res_amp_env->SetGridWidth(0.1, 0.1);
		res_amp_env->WhatsThis("This widget allows you to control the blending of the two sounds "
							   "residual component amplitude envelope. The X axis represents "
							   "transformation time, in a parametric form, and the Y axis represents "
							   "the interpolation factor between the two sounds hoarseness. So a "
							   "interpolation factor of 0.0 means that the resulting sound noisy "
							   "component will match source's one, and an interpolation factor of 1.0 "
							   "means that the resulting sound residual will  match target's one.");
		res_amp_env->hide();
		res_amp_env->InitPoints(0.5);
		res_amp_env->SetSyncToMasterChecked(true);
		mBPFEditorList.push_back(res_amp_env);
			
		// pitch hyb envelope
		Qt_SMS_BPF_Editor_Sync* pitch_hyb_env = new Qt_SMS_BPF_Editor_Sync(this);
		pitch_hyb_env->SetHorizontalRange(0.0, 1.0);
		pitch_hyb_env->SetVerticalRange(0.0, 1.0);
		pitch_hyb_env->SetGridWidth(0.1, 0.1);
		pitch_hyb_env->WhatsThis("Allows to control sounds' pitches blending. Note that this involves "
								 "not only to blend the fundamental frequency, but also any accompanying "
								 "harmonics detected as tready pointed out in the previous control. The "
								 "X axis is time, expressed in a parametric form, and the Y axis is the "
								 "interpolation factor between the two sounds pitches. An interpolation "
								 "factor of 0.0 means that the resulting sound harmonics will match the "
								 "source's one, and a factor of 1.0 means that the resulting sound "
								 "harmonics will match exactly the target's one in their frequency "
								 "distributions.");
		pitch_hyb_env->hide();
		pitch_hyb_env->InitPoints(0.5);
		pitch_hyb_env->SetSyncToMasterChecked(true);
		mBPFEditorList.push_back(pitch_hyb_env);

		// time sync envelope
		Qt_SMS_BPF_Editor* time_sync_env = new Qt_SMS_BPF_Editor(this);
		time_sync_env->SetHorizontalRange(0.0, 1.0);
		time_sync_env->SetVerticalRange(0.0, 1.0);
		time_sync_env->SetGridWidth(0.1, 0.1);
		time_sync_env->WhatsThis("It defines a temporal mapping between the source and the target sound." 
								 "X axis is the normalized time for the source sound and Y is the "
								 "normalized time for the target. Using this control you can achieve"
								 "morphs on only some parts of the sound, define how you want to stretch "
								 "the shortest sound or even reverse one of the sounds in respect to the "
								 "other.");
		time_sync_env->hide();

		CLAM::BPF tmpBPF0;
		tmpBPF0.Insert(0.0, 0.0);
		tmpBPF0.Insert(1.0, 1.0);

		time_sync_env->InitPoints(tmpBPF0);
		mBPFEditorList.push_back(time_sync_env);
		   
		// sin shape envelope
		Qt_SMS_BPF_Editor* sin_shape_env = new Qt_SMS_BPF_Editor(this);
		sin_shape_env->SetHorizontalRange(0.0, 1.0);
		sin_shape_env->SetVerticalRange(0.0, 1.0);
		sin_shape_env->SetGridWidth(0.1, 0.1);
		sin_shape_env->WhatsThis("SinusoidalShapeW1, SinusoidalShapeW2 and SinusoidalShapeInterpolationFactor "
								 "form a non separable set. The first two define spectral envelopes where, "
								 "and for every frequency, a Y value of 0 means that the amplitude of "
								 "that frequency will be taken from the source spectrum and a value of 1 "
								 "means that the amplitude should be that of the target spectrum. The "
								 "third control is a temporal envelope that defines how much of each of "
								 "the previous spectral shapes should be taken in every instant. An "
								 "instantaneous value of 0 in this temporal envelope means that "
								 "SinusoidalShapeW1 should be used and a value of 1 means that the "
								 "SinusoidalShapeW2 should be used.");
		sin_shape_env->hide();
		
		CLAM::BPF tmpBPF1;
		tmpBPF1.Insert(0.0, 1.0);
		tmpBPF1.Insert(1.0, 1.0);

		sin_shape_env->InitPoints(tmpBPF1);
		mBPFEditorList.push_back(sin_shape_env);

		// sin shape w1 envelope
		Qt_SMS_BPF_Editor* sin_shape_w1_env = new Qt_SMS_BPF_Editor(this);
		sin_shape_w1_env->SetHorizontalRange(0.0, 1.0);
		sin_shape_w1_env->SetVerticalRange(0.0, 1.0);
		sin_shape_w1_env->SetGridWidth(0.1, 0.1);
		sin_shape_w1_env->WhatsThis("SinusoidalShapeW1, SinusoidalShapeW2 and SinusoidalShapeInterpolationFactor "
									"form a non separable set. The first two define spectral envelopes where, "
									"and for every frequency, a Y value of 0 means that the amplitude of "
									"that frequency will be taken from the source spectrum and a value of 1 "
									"means that the amplitude should be that of the target spectrum. The "
									"third control is a temporal envelope that defines how much of each of "
									"the previous spectral shapes should be taken in every instant. An "
									"instantaneous value of 0 in this temporal envelope means that "
									"SinusoidalShapeW1 should be used and a value of 1 means that the "
									"SinusoidalShapeW2 should be used.");
		sin_shape_w1_env->hide();
		
		CLAM::BPF tmpBPF2;
		tmpBPF2.Insert(0.0, 0.5);
		tmpBPF2.Insert(1.0, 0.5);

		sin_shape_w1_env->InitPoints(tmpBPF2);
		mBPFEditorList.push_back(sin_shape_w1_env);
			
		// sin shape w2 envelope
		Qt_SMS_BPF_Editor* sin_shape_w2_env = new Qt_SMS_BPF_Editor(this);
		sin_shape_w2_env->SetHorizontalRange(0.0, 1.0);
		sin_shape_w2_env->SetVerticalRange(0.0, 1.0);
		sin_shape_w2_env->SetGridWidth(0.1, 0.1);
		sin_shape_w2_env->WhatsThis("SinusoidalShapeW1, SinusoidalShapeW2 and SinusoidalShapeInterpolationFactor "
									"form a non separable set. The first two define spectral envelopes where, "
									"and for every frequency, a Y value of 0 means that the amplitude of "
									"that frequency will be taken from the source spectrum and a value of 1 "
									"means that the amplitude should be that of the target spectrum. The "
									"third control is a temporal envelope that defines how much of each of "
									"the previous spectral shapes should be taken in every instant. An "
									"instantaneous value of 0 in this temporal envelope means that "
									"SinusoidalShapeW1 should be used and a value of 1 means that the "
									"SinusoidalShapeW2 should be used.");
		sin_shape_w2_env->hide();
		
		CLAM::BPF tmpBPF3;
		tmpBPF3.Insert(0.0, 0.5);
		tmpBPF3.Insert(1.0, 0.5);

		sin_shape_w2_env->InitPoints(tmpBPF3);
		mBPFEditorList.push_back(sin_shape_w2_env);
  
		// res shape void onWhatsThisButtonClicked();envelope
		Qt_SMS_BPF_Editor* res_shape_env = new Qt_SMS_BPF_Editor(this);
		res_shape_env->SetHorizontalRange(0.0, 1.0);
		res_shape_env->SetVerticalRange(0.0, 1.0);
		res_shape_env->SetGridWidth(0.1, 0.1);
		res_shape_env->WhatsThis("This control is curently disabled");
		res_shape_env->hide();
		
		CLAM::BPF tmpBPF4;
		tmpBPF4.Insert(0.0, 1.0);
		tmpBPF4.Insert(1.0, 1.0);

		res_shape_env->InitPoints(tmpBPF4);
		mBPFEditorList.push_back(res_shape_env);

		// res shape w1 envelope
		Qt_SMS_BPF_Editor* res_shape_w1_env = new Qt_SMS_BPF_Editor(this);
		res_shape_w1_env->SetHorizontalRange(0.0, 1.0);
		res_shape_w1_env->SetVerticalRange(0.0, 1.0);
		res_shape_w1_env->SetGridWidth(0.1, 0.1);
		res_shape_w1_env->WhatsThis("This control is curently disabled");
		res_shape_w1_env->hide();
		
		CLAM::BPF tmpBPF5;
		tmpBPF5.Insert(0.0, 0.5);
		tmpBPF5.Insert(1.0, 0.5);

		res_shape_w1_env->InitPoints(tmpBPF5);  
		mBPFEditorList.push_back(res_shape_w1_env);
	  	  
		// res shape w2 envelope
		Qt_SMS_BPF_Editor* res_shape_w2_env = new Qt_SMS_BPF_Editor(this);
		res_shape_w2_env->SetHorizontalRange(0.0, 1.0);
		res_shape_w2_env->SetVerticalRange(0.0, 1.0);
		res_shape_w2_env->SetGridWidth(0.1, 0.1);
		res_shape_w2_env->WhatsThis("This control is curently disabled");
		res_shape_w2_env->hide();
		
		CLAM::BPF tmpBPF6;
		tmpBPF6.Insert(0.0, 0.5);
		tmpBPF6.Insert(1.0, 0.5);

		res_shape_w2_env->InitPoints(tmpBPF6);
		mBPFEditorList.push_back(res_shape_w2_env);
		
		// connections
		connect(((Qt_SMS_BPF_Editor*)mBPFEditorList[GLOBAL_ENVELOPE]),SIGNAL(pointsChanged()),
				this,SIGNAL(configurationChanged()));
		for(int i=SIN_AMP_ENVELOPE;i <= PITCH_HYB_ENVELOPE; i++)
		{
			connect(((Qt_SMS_BPF_Editor*)mBPFEditorList[GLOBAL_ENVELOPE]),SIGNAL(xValueChanged(int,float)),
					((Qt_SMS_BPF_Editor_Sync*)mBPFEditorList[i]),SLOT(updateXValue(int,float)));
			connect(((Qt_SMS_BPF_Editor*)mBPFEditorList[GLOBAL_ENVELOPE]),SIGNAL(yValueChanged(int,float)),
					((Qt_SMS_BPF_Editor_Sync*)mBPFEditorList[i]),SLOT(updateYValue(int,float)));
			connect(((Qt_SMS_BPF_Editor*)mBPFEditorList[GLOBAL_ENVELOPE]),SIGNAL(elementAdded(int,float,float)),
					((Qt_SMS_BPF_Editor_Sync*)mBPFEditorList[i]),SLOT(addElement(int,float,float)));
			connect(((Qt_SMS_BPF_Editor*)mBPFEditorList[GLOBAL_ENVELOPE]),SIGNAL(elementRemoved(int)),
					((Qt_SMS_BPF_Editor_Sync*)mBPFEditorList[i]),SLOT(removeElement(int)));
			connect(((Qt_SMS_BPF_Editor_Sync*)mBPFEditorList[i]),SIGNAL(pointsChanged()),
					this,SIGNAL(configurationChanged()));
		}
		for(int i=TIME_SYNC_ENVELOPE;i <= RES_SHAPE_W2_ENVELOPE; i++)
		{
			connect(((Qt_SMS_BPF_Editor*)mBPFEditorList[i]),SIGNAL(pointsChanged()),this,SIGNAL(configurationChanged()));
		}
		
	}

	void Qt_SMS_Morph_Control::InitSegmentSMSMorphControl()
	{
		InitEnvelopes();

		mFrameInterpSelector = new QCheckBox(this);
		mEnvelopeSelector = new QComboBox(this);
		mWhatsThisButton = new QPushButton(this);

		mFrameInterpSelector->setChecked(false);
		mFrameInterpSelector->setText("Interpolate intermediate frames");
		QToolTip::add(mFrameInterpSelector,"Enable for allow interpolation of frames " 
					  "while morphing sounds of different lenghts");

		mEnvelopeSelector->insertItem("Global morph factor vs. time",0);
		mEnvelopeSelector->insertItem("Sinusoidal component amplitude hybridization",1);
		mEnvelopeSelector->insertItem("Sinusoidal component frequency hybridization",2);
		mEnvelopeSelector->insertItem("Residual component amplitude hybridization",3);
		mEnvelopeSelector->insertItem("Pitch hybridization",4);
		mEnvelopeSelector->insertItem("Time synchronization",5);
		mEnvelopeSelector->insertItem("Sinusoidal spectral shape interpolation",6);
		mEnvelopeSelector->insertItem("Sinusoidal spectral shape weight A",7);
		mEnvelopeSelector->insertItem("Sinusoidal spectral shape weight B",8);
		mEnvelopeSelector->insertItem("Residual spectral shape interpolation",9);
		mEnvelopeSelector->insertItem("Residual spectral shape weight A",10);
		mEnvelopeSelector->insertItem("Residual spectral shape weight B",11);
		QToolTip::add(mEnvelopeSelector,"Sound Hybridization Controls");

		mWhatsThisButton->setPixmap(QPixmap("rc/whatsthis.xpm"));
		mWhatsThisButton->setFixedSize(25,25);
		QToolTip::add(mWhatsThisButton,"What's this?");

		QBoxLayout* innerLayout0 = new QHBoxLayout;
		innerLayout0->addWidget(mEnvelopeSelector);
		innerLayout0->addStretch();
		innerLayout0->addWidget(mWhatsThisButton);

		QBoxLayout* innerLayout1 = new QHBoxLayout;
		for(unsigned i=0; i < mBPFEditorList.size(); i++)
		{
			innerLayout1->addWidget(mBPFEditorList[i]);
		}

		QBoxLayout* mainLayout = new QVBoxLayout(this);
		mainLayout->addWidget(mFrameInterpSelector);
		mainLayout->addLayout(innerLayout0);
		mainLayout->addLayout(innerLayout1);

		// connections
		connect(mFrameInterpSelector,SIGNAL(clicked()),this,SIGNAL(configurationChanged()));
		connect(mEnvelopeSelector,SIGNAL(activated(int)),SLOT(onEnvelopeSelected(int)));
		connect(mWhatsThisButton,SIGNAL(clicked()),SLOT(onWhatsThisButtonClicked()));
	}
}

// END

