#include "NonSupervisedSystem.hxx"
#include "AudioManager.hxx"

int main()
{
	CLAM::AudioManager audioManager(44100, 512);
	FlowControlExample::System s(
		"testin.wav" /* file in */,
		"testout.wav" /* file out */, 
		512 /* frame size */, 
		500 /* frames to compute*/,
		true /* flag for audio out*/);

	s.ProcessAllIterations();

	return 0;
}
