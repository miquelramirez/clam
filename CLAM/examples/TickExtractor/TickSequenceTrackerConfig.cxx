#include "TickSequenceTrackerConfig.hxx"

namespace CLAM
{
	namespace RhythmDescription
	{

		void TickSequenceTrackerConfig::DefaultInit()
		{
			AddAll();
			UpdateData();
			//default values
			SetSampleRate(44100.0);
			SetComputeBeats(true);
			SetNTrans(500);
			SetTransHop(450);
			SetThreshold_IOIHistPeaks(0.0);
			SetTempoLimInf(50);
			SetTempoLimSup(200);
			SetTickLimInf(0.6);
			SetTickLimSup(0.1);
			SetDeviationPenalty(1);
			SetOverSubdivisionPenalty(8);
			SetGaussianWindowSize(0.25);
			SetScope(0.10);
			SetTickAdjustForSwing(false);
			SetAdjustWithOnsets(true);
		}

	}
}
