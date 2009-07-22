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

#include "LoadImpulseResponse.hxx"
#include <CLAM/MonoAudioFileReader.hxx>
#include <CLAM/AudioSource.hxx>
#include <CLAM/AudioWindowing.hxx>
#include <fstream>
#include "MyFFT.hxx"
#include <CLAM/ProcessingDataPlugin.hxx>

namespace CLAM
{
namespace Hidden
{
	static ProcessingDataPlugin::Registrator<ImpulseResponse*> registrator("green","CLAM::ImpulseResponse");
}

bool computeResponseSpectrums(Processing & source, std::vector<ComplexSpectrum> & responseSpectrums, unsigned framesize, std::string & errorMsg, unsigned sampleRate);

unsigned neededFramesForNSamples(unsigned nsamples, unsigned frameSize)
{
	return (nsamples+frameSize-1)/frameSize;
}


bool computeResponseSpectrums(const std::string & wavfile, std::vector<ComplexSpectrum> & responseSpectrums, unsigned framesize, std::string & errorMsg, unsigned selectedChannel, unsigned sampleRate)
{
	MonoAudioFileReaderConfig readerConfig;
	readerConfig.SetSourceFile(wavfile);
	readerConfig.SetSelectedChannel(selectedChannel);
	MonoAudioFileReader reader(readerConfig);
	if (!reader.IsConfigured())
	{
		errorMsg += "Configuring the inner AudioFileReader:\n";
		errorMsg += reader.GetConfigErrorMessage();
		return false;
	}
	const unsigned nSamples = neededFramesForNSamples( reader.GetHeader().GetSamples(), framesize ) * framesize;
//	std::cout << "ComputeResponseSpectrums: NSamples: " << nSamples << std::endl;

	reader.GetOutPort(0).SetSize(nSamples);
	reader.GetOutPort(0).SetHop(nSamples);

	return computeResponseSpectrums(reader, responseSpectrums, framesize, errorMsg, sampleRate);
}

bool computeResponseSpectrums(const std::vector<double> & buffer, std::vector<ComplexSpectrum> & responseSpectrums, unsigned framesize, std::string & errorMsg, unsigned sampleRate, unsigned samplesOffset )
{
	// TODO: case where offset > size
	const unsigned validSamples = buffer.size() - samplesOffset;
	AudioSource source;
	int portIndex = 0;
	source.SetExternalBuffer(&buffer[0]+samplesOffset, validSamples, portIndex);
	const unsigned nSamples = neededFramesForNSamples( validSamples, framesize) * framesize;

	// this works by the grace of audiocource being constructed locally,
	// by default always having one port with index '0'
	source.GetOutPort(portIndex).SetSize(nSamples);
	source.GetOutPort(portIndex).SetHop(nSamples);
	
	return computeResponseSpectrums(source, responseSpectrums, framesize, errorMsg, sampleRate);
}

bool computeResponseSpectrums(Processing & source, std::vector<ComplexSpectrum> & responseSpectrums, unsigned framesize, std::string & errorMsg, unsigned sampleRate)
{
	AudioWindowingConfig windowerConfig;
	windowerConfig.SetSamplingRate(sampleRate); // TODO: Take it from the file
	windowerConfig.SetHopSize(framesize);
	windowerConfig.SetWindowSize(framesize+1);
	windowerConfig.SetFFTSize(framesize*2);
	windowerConfig.SetDoHalfWindowShift(false);
	windowerConfig.SetWindowType(EWindowType::eNone);
	AudioWindowing windower(windowerConfig);
	if (!windower.IsConfigured())
	{
		errorMsg += "Configuring the inner AudioWindowing:\n";
		errorMsg += windower.GetConfigErrorMessage();
		return false;
	}
	MyFFT::Config fftConfig; 
	fftConfig.SetAudioSize(framesize*2);
	MyFFT fft(fftConfig);

	ConnectPorts(source,0,windower,0);
	ConnectPorts(windower,0,fft,0);
	InPort<ComplexSpectrum> fetcher;
	fft.GetOutPort(0).ConnectToIn(fetcher);

	responseSpectrums.clear();
	source.Start();
	windower.Start();
	fft.Start();

	source.Do(); //read all the file and put it into the out port buffer
	while (windower.CanConsumeAndProduce())
	{
		windower.Do();
		fft.Do();
		responseSpectrums.push_back(fetcher.GetData());
		fetcher.Consume();
	}
	CLAM_DEBUG_ASSERT(responseSpectrums.size()>0, "LoadImpulseResponse: Expected an sliced IR with more than one slice.");
	source.Stop();
	windower.Stop();
	fft.Stop();
	return true;
}
std::string formatFile(
		const std::string & filePrefix,
		unsigned x1, unsigned y1, unsigned z1, unsigned x2, unsigned y2, unsigned z2 )
{
		std::ostringstream os;
		os << filePrefix 
			<< "_emissor_"<< x1 << "-" << y1 << "-" << z1
			<< "_receptor_" << x2 << "-" << y2 << "-" << z2
			<< ".wav" << std::flush;
		return os.str();
}
unsigned searchCoordinateUpperLimit(
		const std::string & filePrefix,
		unsigned coordinate)
{
	std::string file;
	for (unsigned value=0; true; value++)
	{
		unsigned coords[6]={};
		coords[coordinate] = value;
		std::ostringstream os;
		file = formatFile(filePrefix, coords[0],coords[1],coords[2],coords[3],coords[4],coords[5]);
		if (std::fstream(file.c_str()).fail())
			return value;
	}
	throw "should not reach this line";
}

bool ImpulseResponseDatabase::loadImpulseResponseDatabase( 
		const std::string & filePrefix,
		unsigned frameSize,
		std::string & errorMsg,
		unsigned sampleRate )
{
	std::string file;
	file = formatFile(filePrefix, 0,0,0,0,0,0);
	if (std::fstream(file.c_str()).fail())
	{
		errorMsg += "ImpulseResponseDatabase::loadImpulseResponseDatabase:\nCan not open '"+file+"'";
		return false;
	}

	NXEmitter=searchCoordinateUpperLimit(filePrefix, 0);
	NYEmitter=searchCoordinateUpperLimit(filePrefix, 1);
	NZEmitter=searchCoordinateUpperLimit(filePrefix, 2);
	NXReceiver=searchCoordinateUpperLimit(filePrefix, 3);
	NYReceiver=searchCoordinateUpperLimit(filePrefix, 4);
	NZReceiver=searchCoordinateUpperLimit(filePrefix, 5);
	long unsigned totalFiles = NXEmitter*NYEmitter*NZEmitter*NXReceiver*NYReceiver*NZReceiver;
	_storage.resize( totalFiles);
	int percentDone=-1;
	std::cout << "Loading impulse response files: " << std::flush;
	const unsigned nChannel=0;
	unsigned i=0;
	for (unsigned xEmitter=0; xEmitter<NXEmitter; xEmitter++)
	for (unsigned yEmitter=0; yEmitter<NYEmitter; yEmitter++)
	for (unsigned zEmitter=0; zEmitter<NZEmitter; zEmitter++)
	for (unsigned xReceiver=0; xReceiver<NXReceiver; xReceiver++)
	for (unsigned yReceiver=0; yReceiver<NYReceiver; yReceiver++)
	for (unsigned zReceiver=0; zReceiver<NZReceiver; zReceiver++)
	{
		std::string file = formatFile(filePrefix, xEmitter,yEmitter,zEmitter,xReceiver,yReceiver,zReceiver);
		if (!computeResponseSpectrums( file, _storage[i], frameSize, errorMsg, nChannel, sampleRate )) return false;
		i++;
		if (int(i/float(totalFiles)*10)>percentDone)
		{
			percentDone++;
			std::cout << percentDone*10 <<"% "<< std::flush;
		}
	}
	std::cout << std::endl;
	return true;

}

ImpulseResponse &  ImpulseResponseDatabase::get(unsigned x1, unsigned y1, unsigned z1, unsigned x2, unsigned y2, unsigned z2 )
{
	long unsigned index =
		z2 + NZReceiver * (
		y2 + NYReceiver * (
		x2 + NXReceiver * (
		z1 + NZEmitter * (
		y1 + NYEmitter * (
		x1
		)))));
//		std::cout << " x1 "<<x1 <<" y1 "<<y1<<" z1 "<<z1<<std::endl <<" x2 "<<x2<<" y2 "<<y2<<" z2 "<<z2 << std::endl<< "index "<<index <<std::endl;
	return _storage[index];
}

} //namespace CLAM

