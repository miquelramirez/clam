#include "NonSupervisedSystem.hxx"

using FlowControlExample::System;

int main()
{
	System s(
		"snd.wav" /* file in */,
		"curtout.wav" /* file out */, 
		512 /* frame size */, 
		5000 /* frames to compute*/ );

	s.ProcessAllIterations();
//	s.DoProcessings( &System::OscillatorToFileOut );

	return 0;
}
