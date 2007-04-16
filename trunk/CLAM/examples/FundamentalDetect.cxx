#if 1

#include <stdio.h>

int main(int argc,char** argv)
{
	printf("THIS EXAMPLE NEEDS FIXING\n");
}

#else

/*
 * Copyright (c) 2001-2004 MUSIC TECHNOLOGY GROUP (MTG)
 *                         UNIVERSITAT POMPEU FABRA
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

#include<iostream>
#include<fstream>
#include<string>
#include"FFT_rfftw.hxx"
#include "AudioFileIn.hxx"
#include "PeakDetect.hxx"

#include"FundFreqDetect.hxx"
#include"XMLStorage.hxx"
#include"DataTypes.hxx"
#include"WindowGenerator.hxx"

#include<cmath>

using namespace CLAM;
using namespace  std;

int main(int argc, char* argv[])
{
	try {

		int i,Size=1024;
		float SampleRate=44100.0;

		XMLStorage test_file("MPEG_example_output");



// Audio Data creation
		// Audio config

		Audio audioData;
		audioData.SetSize(Size);
		for (i=0;i<Size;i++)
			audioData.GetBuffer()[i]=
			    0.3*sin(2.0*PI*200.0*(((float)i)/SampleRate)) + 
			    0.2*sin(2.0*PI*400.0*(((float)i)/SampleRate)) + 
			    0.1*sin(2.0*PI*600.0*(((float)i)/SampleRate)) + 
			    0.1*sin(2.0*PI*800.0*(((float)i)/SampleRate)) + 
			    0.1*sin((2.0*PI*1000.0*((float)i)/SampleRate));

// Spectrum Data creation
		// Spectrum attribute selection and config


		SpecTypeFlags sflags;
		sflags.bMagPhase=false;
		sflags.bMagPhaseBPF=true;

		SpectrumConfig sconfig;
		sconfig.SetType(sflags);
		sconfig.SetSize(Size/2+1);

		Spectrum  spectrumData(sconfig);
		spectrumData.GetMagBPF().SetIntpType(EInterpolation::ePolynomial2);
		Spectrum   spectrumDataOut(sconfig);

		
		// Spectral Peaks

		SpectralPeakArray   peaks;
		peaks.SetnMaxPeaks(5);

		// Fundamental 

		Fundamental        fund;
		fund.SetnMaxCandidates(5);
		fund.AddCandidatesFreq();
		fund.AddCandidatesErr();
		fund.UpdateData();
		fund.GetCandidatesErr().Resize(10);
		fund.GetCandidatesErr().SetSize(10);


		// Processing object creation
		// Processing object configuration

		FFTConfig fconfig;
		fconfig.SetAudioSize(Size);
		FFT_rfftw fft(fconfig);

		// PeakDetection

		PeakDetectConfig  spconfig;
		spconfig.SetNumBands(Size/2+1);
		PeakDetect peakproc(spconfig);

		FundFreqDetectConfig fundprocconfig;
		FundFreqDetect       fundproc(fundprocconfig);


		AudioFileConfig  afconfig;
		afconfig.SetFilename("test.wav");
		afconfig.SetFiletype(EAudioFileType::eWave);
		AudioFileIn  audioIn(afconfig);

		WindowGeneratorConfig  wingenconf;
		WindowGenerator  wingen(wingenconf);

		DataArray data;
		data.Resize(1028);
		data.SetSize(1028);

		// Processing object execution

		cerr << "Starting Object ... " << endl;

		wingen.Start();		
		audioIn.Start();
		fft.Start();
		peakproc.Start();
		fundproc.Start();

		cerr << "Running Object ... " << endl;

		// for now this only produces an  Array, which is not used
		wingen.Do(data);

		//		audioIn.Do(audioData);
		fft.Do(audioData,spectrumData);
		peakproc.Do(spectrumData,peaks);
		fundproc.Do(peaks,fund);

		cerr << "done " << endl;

		if (fund.GetnCandidates())
		  cerr << "Fundamental Value = " << fund.GetFreq() << endl;
		exit(0);

		// Result storage  Spectrum Data only for now...

		spectrumData.StoreOn(test_file);
		ofstream output("MPEG_example.xml");
		test_file.dumpOn(output);
		

	}
	catch(Err err) {
		err.Print();
	}

	return 0;

}

#endif

