#ifndef __QTSMSMORPHCONTROL__
#define __QTSMSMORPHCONTROL__

#include <vector>
#include <qwidget.h>
#include <CLAM/BPF.hxx>

class QCheckBox;
class QComboBox;
class QPushButton;

namespace QtSMS
{
	class Qt_SMS_Morph_Control : public QWidget
	{
		typedef std::vector<QWidget*> BPFEditorList;

		Q_OBJECT
	public:
		Qt_SMS_Morph_Control(QWidget* parent=0);
		~Qt_SMS_Morph_Control();

		void RetrieveGlobalEnvelope(CLAM::BPF& bpf);
		void SetGlobalEnvelope(const CLAM::BPF& bpf);

		void RetrieveSinAmpEnvelope(CLAM::BPF& bpf);
		void SetSinAmpEnvelope(const CLAM::BPF& bpf);

		void RetrieveSinFreqEnvelope(CLAM::BPF & bpf);
		void SetSinFreqEnvelope(const CLAM::BPF& bpf);

		void RetrieveResAmpEnvelope(CLAM::BPF& bpf);
		void SetResAmpEnvelope(const CLAM::BPF& bpf);

		void RetrievePitchHybEnvelope(CLAM::BPF& bpf);
		void SetPitchHybEnvelope(const CLAM::BPF& bpf);

		void RetrieveTimeSyncEnvelope(CLAM::BPF& bpf);
		void SetTimeSyncEnvelope(const CLAM::BPF& bpf);

		void RetrieveSinShapeW1Envelope(CLAM::BPF& bpf);
		void SetSinShapeW1Envelope(const CLAM::BPF& bpf);

		void RetrieveSinShapeW2Envelope(CLAM::BPF& bpf);
		void SetSinShapeW2Envelope(const CLAM::BPF& bpf);

		void RetrieveResShapeW1Envelope(CLAM::BPF& bpf);
		void SetResShapeW1Envelope(const CLAM::BPF& bpf);

		void RetrieveResShapeW2Envelope(CLAM::BPF& bpf);
		void SetResShapeW2Envelope(const CLAM::BPF& bpf);

		void RetrieveSinShapeEnvelope(CLAM::BPF& bpf);
		void SetSinShapeEnvelope(const CLAM::BPF& bpf);

		void RetrieveResShapeEnvelope(CLAM::BPF& bpf);
		void SetResShapeEnvelope(const CLAM::BPF& bpf);

		bool FrameInterpolationChecked() const;
		void FrameInterpolationChecked(bool ckecked);

	signals:
		void configurationChanged();

	private slots:
		void onEnvelopeSelected(int);
		void onWhatsThisButtonClicked();

	private:
		BPFEditorList mBPFEditorList;
		QCheckBox*    mFrameInterpSelector;
		QComboBox*    mEnvelopeSelector;
		QPushButton*  mWhatsThisButton;

		enum 
		{
			GLOBAL_ENVELOPE=0,
			SIN_AMP_ENVELOPE,
			SIN_FREQ_ENVELOPE,
			RES_AMP_ENVELOPE,
			PITCH_HYB_ENVELOPE,
			TIME_SYNC_ENVELOPE,
			SIN_SHAPE_ENVELOPE,
			SIN_SHAPE_W1_ENVELOPE,
			SIN_SHAPE_W2_ENVELOPE,
			RES_SHAPE_ENVELOPE,
			RES_SHAPE_W1_ENVELOPE,
			RES_SHAPE_W2_ENVELOPE
			
		};
		
		void InitEnvelopes();
		void InitSegmentSMSMorphControl();
	};
}

#endif

