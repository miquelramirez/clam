#ifndef LoadImpulseResponse_hxx
#define LoadImpulseResponse_hxx

#include "ComplexSpectrum.hxx"
#include <string>
#include <vector>
#include <iostream>

namespace CLAM
{


typedef std::vector<ComplexSpectrum> ImpulseResponse;

bool computeResponseSpectrums(
		const std::string & wavfile, 
		ImpulseResponse & responseSpectrums, 
		unsigned frameSize, 
		std::string & errorMsg);

class ImpulseResponseDatabase
{
	typedef std::vector<ImpulseResponse> ImpulseResponses;
	ImpulseResponses _storage;
public:
	unsigned NXEmitter;
	unsigned NYEmitter;
	unsigned NZEmitter;
	unsigned NXReceiver;
	unsigned NYReceiver;
	unsigned NZReceiver;
	bool loadImpulseResponseDatabase( 
			const std::string & filePrefix,
			unsigned frameSize,
			std::string & errorMsg );
	ImpulseResponse & get(int x1, int y1, int z1, int x2, int y2, int z2);
};



}
#endif

