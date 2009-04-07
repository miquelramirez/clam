#include "CLAMVstPlugin.hpp"
#include <iostream>
int main()
{
	std::cout << "== Testing CLAM vst plugin" << std::endl;
	audioMasterCallback audioMaster=0;
	CLAMTest* effect = new CLAMTest (audioMaster);
	float inbuf[] = {1,1,1,1,1};
	float outbuf[] = {0,0,0,0,0};
	float* firstInBuf = &inbuf[0];
	float* firstOutBuf = &outbuf[0];
	float **ins = &firstInBuf;
	float **outs = &firstOutBuf;
	effect->process(ins, outs, 5);
	delete effect;
	return 0;
}

