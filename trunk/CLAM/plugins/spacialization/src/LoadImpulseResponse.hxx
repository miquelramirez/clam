/*
 * Copyright (c) 2007 Fundació Barcelona Media Universitat Pompeu Fabra
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#ifndef LoadImpulseResponse_hxx
#define LoadImpulseResponse_hxx

#include "ComplexSpectrum.hxx"
#include <string>
#include <vector>
#include <iostream>

namespace CLAM
{

/**
 An ImpulseResponse object contains the STFT representation of an impulse response.
 It is used to convolve it with a STFT analyzed audio stream.
 @ingroup RealTimeConvolution
 @todo Document ImpulseResponse
*/
typedef std::vector<ComplexSpectrum> ImpulseResponse;

bool computeResponseSpectrums(
		const std::string & wavfile, 
		ImpulseResponse & responseSpectrums, 
		unsigned frameSize, 
		std::string & errorMsg);

bool computeResponseSpectrums(
		const std::vector<double> & buffer,
		ImpulseResponse & responseSpectrums, 
		unsigned frameSize, 
		std::string & errorMsg,
		unsigned samplesOffset=0);

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
	ImpulseResponse & get(unsigned x1, unsigned y1, unsigned z1, unsigned x2, unsigned y2, unsigned z2);
};



}
#endif

