
#include "SupervisedSystemWithoutTrueFlowControl.hxx"

int main()
{
	FlowControlExample::SupervisedSystemWithoutTrueFlowControl s(
		"testin.wav" /* file in */,
		"testout.wav" /* file out */, 
		512 /* frame size */, 
		500 /* frames to compute*/,
		false /* audio output*/);

	s.ProcessAllNetworkTopologies();

	return 0;
}
