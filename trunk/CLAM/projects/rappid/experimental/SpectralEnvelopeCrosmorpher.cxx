#include "SpectralEnvelopeCrosmorpher.hxx"




void SpectralEnvelopeCrosmorpherConfig::DefaultInit()
{

}



bool SpectralEnvelopeCrosmorpher::ConcreteConfigure(const ProcessingConfig&cfg)
{
	mConfig = dynamic_cast<const SpectralEnvelopeCrosmorpherConfig&>(cfg);
}

SpectralEnvelopeCrosmorpher::SpectralEnvelopeCrosmorpher()
{
}

SpectralEnvelopeCrosmorpher::SpectralEnvelopeCrosmorpher(const SpectralEnvelopeCrosmorpherConfig &c)
{
}

bool SpectralEnvelopeCrosmorpher::Do(void)
{
	bool res = Do(Input1.GetData(),Input2.GetData(),Output.GetData());
	Input1.LeaveData();
	Input2.LeaveData();
	Output.LeaveData();
	return res;
}

bool SpectralEnvelopeCrosmorpher::Do(Spectrum& in1, Spectrum& in2, Spectrum& out)
{
	CheckData(in1,in2,out);

	ExtractEnvelope(in1,mEnvelope1);

	ExtractEnvelope(in2,mEnvelope2);

	MorphEnvelopes(mEnvelope1,mEnvelope2,mMorphedEnvelope1);

	MorphEnvelopes(mEnvelope2,mEnvelope1,mMorphedEnvelope2);

	MultiplyMag(mIn1,mMorphedEnvelope1,mOut1);

	MultiplyMag(mIn2,mMorphedEnvelope2,mOut2);

	Add(mOut1,mOut2,out);
}

