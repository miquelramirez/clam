/*
 * Copyright (c) 2001-2006 MUSIC TECHNOLOGY GROUP (MTG)
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

/* 
David Garcia Garzon
Universitat Pompeu Fabra

Katy Noland
Queen Mary, University of London

Adapted from MATLAB code by Chris Harte at Queen Mary
*/
#include <iostream>
#include <fstream>
#include <cmath>
#include "ChordExtractor.hxx"
#include "MonoAudioFileReader.hxx"
#include "AudioInPort.hxx"
#include "AudioOutPort.hxx"
#include "XMLStorage.hxx"
#include "Pool.hxx"
#include "Array.hxx"
#include "Enumerated.hxx"
#include "FrameDivision.hxx"
#include <CLAM/Assert.hxx>

const char * copyright =
	"Chord extraction v1.0.\n"
	"Copyright 2006 Queen Mary University of London\n"
	"Copyright 2006 Universitat Pompeu Fabra\n"
	"Original algorithm by Chris Harte.\n"
	"Ported to C++ by David Garcia Garzon and Katy Noland.\n"
	"\n"
	;
const char * usage =
	"Usage: ChordExtractor [-s out.sc] [-f <suffix>] <wavefile1> <wavefile2> ...\n"
	"\nOptions:\n"
	" -h            shows this help\n"
	" -s            dump the schema to the standard output\n"
	" -f <suffix>   append <suffix> to the generated descriptors file (default: '.pool')\n"
	"\nUsage examples:\n"
	" ChordExtractor -s schema.sc\n"
	" ChordExtractor -f .beats song1.wav song2.mp3 song3.ogg\n"
	;
const char * schemaContent = 
"<?xml version='1.0' encoding='UTF-8' standalone='no' ?>\n"
"<DescriptionScheme>\n"
"\n"
" <Uri>descriptionScheme:www.iua.upf.edu:clam:ChordExtraction</Uri>\n"
"\n"
" <Attributes>\n"
"   <Attribute name='Chords_Harte' scope='Song' type='Segmentation'>\n"
"     <ChildScope>ExtractedChord</ChildScope>\n"
"     <SegmentationPolicy>Discontinuous</SegmentationPolicy>\n"
"   </Attribute>\n"
"    <Attribute name='DebugFrameSegments' scope='Song' type='Segmentation'>\n"
"	  <Documentation>\n"
"	  	This segmentation is just to check that the segments and the frames are properly aligned.\n"
"	  </Documentation>\n"
"      <ChildScope></ChildScope>\n"
"      <SegmentationPolicy>Continuous</SegmentationPolicy>\n"
"    </Attribute>\n"
"    <Attribute name='Frames' scope='Song' type='FrameDivision'>\n"
"      <ChildScope>Frame</ChildScope>\n"
"    </Attribute>\n"
"    <Attribute name='TunningPosition' scope='Frame' type='Float'/>\n"
"    <Attribute name='TunningStrength' scope='Frame' type='Float'/>\n"
"    <Attribute name='FirstChordCandidateRelevance' scope='Frame' type='Float'/>\n"
"    <Attribute name='SecondChordCandidateRelevance' scope='Frame' type='Float'/>\n"
"    <Attribute name='FirstChordIndex' scope='Frame' type='Float'/>\n"
"    <Attribute name='SecondChordIndex' scope='Frame' type='Float'/>\n"
"    <Attribute name='Energy' scope='Frame' type='Float'/>\n"
"	<!--\n"
"    <Attribute name='PrimaryRoot' scope='Frame'type='Enumerated'>\n"
"      <EnumerationValues>A A# B B# C C# D D# E F F# G G#</EnumerationValues>\n"
"    </Attribute>\n"
"    <Attribute name='PrimaryMode' scope='Frame' type='Enumerated'\n"
"      <EnumerationValues>Major Minor Major7 Minor7 Dominant7 MajorMinor7 Diminished Augmented Fifth</EnumerationValues>\n"
"    </Attribute>\n"
"    <Attribute name='SecondaryRoot' scope='Frame' type='Enumerated'>\n"
"      <EnumerationValues>A A# B B# C C# D D# E F F# G G#</EnumerationValues>\n"
"    </Attribute>\n"
"    <Attribute name='SecondaryMode'scope='Frame' type='Enumerated'>\n"
"      <EnumerationValues>Major Minor Major7 Minor7 Dominant7 MajorMinor7 Diminished Augmented Fifth</EnumerationValues>\n"
"    </Attribute>\n"
"	-->\n"
"    <Attribute name='HartePcp' scope='Frame' type='FloatArray'>\n"
"      <BinLabels>G G# A Bb B C C# D Eb E F F#</BinLabels>\n"
"    </Attribute>\n"
"    <Attribute name='HarteChordCorrelation' scope='Frame' type='FloatArray'>\n"
"      <BinLabels>"
"G G#/Ab A A#/Bb B C C#/Db D D#/Eb E F F#/Gb "
"g g#/ab a a#/bb b c c#/db d d#/eb e f f#/gb "
		"</BinLabels>\n"
"    </Attribute>\n"
"    <Attribute name='Root' scope='ExtractedChord' type='Enumerated'>\n"
"      <EnumerationValues>G G# A A# B C C# D D# E F F#</EnumerationValues>\n"
"    </Attribute>\n"
"    <Attribute name='Mode' scope='ExtractedChord' type='Enumerated'>\n"
"      <EnumerationValues>\n"
"         Major Minor Major7 Minor7 Dominant7 MinorMajor7\n"
//"		 Suspended2 Suspended4 Major6 Minor6 6/9\n"
"		 Diminished Diminished7 Augmented Fifth\n"
"      </EnumerationValues>\n"
"    </Attribute>\n"
"  </Attributes>\n"
"\n"
"</DescriptionScheme>\n"
;

class ChordExtractorSegmentation
{
	const Simac::ChordExtractor & extractor;
	CLAM::DescriptionDataPool * _pool;
	unsigned & _lastChord;
	CLAM::DataArray * _chordSegmentation;
	CLAM::TData _currentTime;
public:
	ChordExtractorSegmentation(CLAM::DescriptionDataPool * pool, unsigned & lastChord, CLAM::DataArray * chordSegmentation, CLAM::TData & currentTime, const Simac::ChordExtractor & extractor) 
		: extractor(extractor)
		, _pool(pool)
		, _lastChord(lastChord)
		, _chordSegmentation(chordSegmentation)
		, _currentTime(currentTime)				      
	{
	}
	~ChordExtractorSegmentation() {};

	void doIt()
	{
		const std::vector<double> & correlation = extractor.chordCorrelation(); //pointer to correlation data

		CLAM::TData firstCandidateWeight = correlation[extractor.firstCandidate()];
		CLAM::TData secondCandidateWeight = correlation[extractor.secondCandidate()];
		CLAM::TData noCandidateWeigth = correlation[0];
		
		unsigned currentChord = firstCandidateWeight*0.6<=noCandidateWeigth || noCandidateWeigth<0.001 ?
				0 : extractor.firstCandidate();
		
		if (currentChord!=_lastChord)
		{
			if (_lastChord != 0)
				_chordSegmentation[0].AddElem(_currentTime);
			if (currentChord != 0)
			{
				unsigned newSegment = _pool->GetNumberOfContexts("ExtractedChord");
				_chordSegmentation[0].AddElem(_currentTime);
				_pool->Insert("ExtractedChord", newSegment);
				_pool->GetWritePool<Simac::Enumerated>("ExtractedChord","Root")[newSegment]= extractor.root(extractor.firstCandidate());
				_pool->GetWritePool<Simac::Enumerated>("ExtractedChord","Mode")[newSegment]= extractor.mode(extractor.firstCandidate());
			}
			_lastChord = currentChord;
		}
	}
};

class ChordExtractorDescriptionDumper
{
	const Simac::ChordExtractor & extractor;
	std::ofstream outputPool;

	CLAM::DescriptionScheme _schema;
	CLAM::DescriptionDataPool * _pool;
	unsigned _currentFrame;
	CLAM::TData * _tunningPositions;
	CLAM::TData * _tunningStrength;
	CLAM::TData * _firstChordRelevance;
	CLAM::TData * _secondChordRelevance;
	CLAM::TData * _firstChordIndex;
	CLAM::TData * _secondChordIndex;
	CLAM::TData * _energies;
	unsigned _lastChord;
	CLAM::DataArray * _pcps;
	CLAM::DataArray * _chordChorrelation;
	CLAM::DataArray * _chordSegmentation;
	unsigned _hop;
	unsigned _firstFrameOffset;
	CLAM::TData _samplingRate;
public:
	ChordExtractorDescriptionDumper(const std::string & filenameBase, const std::string & suffix, unsigned frames, unsigned hop, unsigned framesize, CLAM::TData samplingRate, const Simac::ChordExtractor & extractor)
		: extractor(extractor)
		, outputPool((filenameBase+suffix).c_str())
		, _currentFrame(0)
		, _lastChord(0)
		, _hop(hop)
//		, _firstFrameOffset((framesize-hop)/2)
		, _firstFrameOffset(0)
		, _samplingRate(samplingRate)
	{
		_schema.AddAttribute<CLAM::DataArray>("Song", "Chords_Harte");
		_schema.AddAttribute<CLAM::DataArray>("Song", "DebugFrameSegments");
		_schema.AddAttribute<Simac::FrameDivision>("Song", "Frames");
		_schema.AddAttribute<CLAM::TData>("Frame", "TunningPosition");
		_schema.AddAttribute<CLAM::TData>("Frame", "TunningStrength");
		_schema.AddAttribute<CLAM::TData>("Frame", "FirstChordCandidateRelevance");
		_schema.AddAttribute<CLAM::TData>("Frame", "SecondChordCandidateRelevance");
		_schema.AddAttribute<CLAM::TData>("Frame", "FirstChordIndex");
		_schema.AddAttribute<CLAM::TData>("Frame", "SecondChordIndex");
		_schema.AddAttribute<CLAM::TData>("Frame", "Energy");
		_schema.AddAttribute<Simac::Enumerated>("ExtractedChord", "Root");
		_schema.AddAttribute<Simac::Enumerated>("ExtractedChord", "Mode");
		_schema.AddAttribute<CLAM::DataArray>("Frame", "HartePcp");
		_schema.AddAttribute<CLAM::DataArray>("Frame", "HarteChordCorrelation");
		_pool = new CLAM::DescriptionDataPool(_schema);
		_pool->SetNumberOfContexts("Song", 1);
		_pool->SetNumberOfContexts("Frame", frames);
		_pool->SetNumberOfContexts("ExtractedChord", 0);
		Simac::FrameDivision & frameDivision = _pool->GetWritePool<Simac::FrameDivision>("Song","Frames")[0];
		frameDivision.SetFirstCenter(0);
	//	frameDivision.SetFirstCenter(framesize/2);
		frameDivision.SetInterCenterGap(hop);
		_tunningPositions = _pool->GetWritePool<CLAM::TData>("Frame","TunningPosition");
		_tunningStrength = _pool->GetWritePool<CLAM::TData>("Frame","TunningStrength");
		_firstChordRelevance = _pool->GetWritePool<CLAM::TData>("Frame","FirstChordCandidateRelevance");
		_secondChordRelevance = _pool->GetWritePool<CLAM::TData>("Frame","SecondChordCandidateRelevance");
		_firstChordIndex = _pool->GetWritePool<CLAM::TData>("Frame","FirstChordIndex"); // TODO: Kludge!!
		_secondChordIndex = _pool->GetWritePool<CLAM::TData>("Frame","SecondChordIndex"); // TODO: Kludge!!
		_chordSegmentation = _pool->GetWritePool<CLAM::DataArray>("Song","Chords_Harte");
		_pcps = _pool->GetWritePool<CLAM::DataArray>("Frame","HartePcp");
		_chordChorrelation = _pool->GetWritePool<CLAM::DataArray>("Frame","HarteChordCorrelation");
		_energies = _pool->GetWritePool<CLAM::TData>("Frame","Energy");
	}
	~ChordExtractorDescriptionDumper()
	{
		for (unsigned frame=_currentFrame; frame<_pool->GetNumberOfContexts("Frame"); frame++)
		{
			_pcps[frame].Resize(12);
			_pcps[frame].SetSize(12);
			_chordChorrelation[frame].Resize(24);
			_chordChorrelation[frame].SetSize(24);
		}
		CLAM::TData currentTime = (_currentFrame*_hop+_firstFrameOffset)/_samplingRate;
		

		if (_lastChord != 0)
			_chordSegmentation[0].AddElem(currentTime);


//		CLAM::XMLStorage::Dump(*_pool, "Description", std::cout);
		CLAM::XMLStorage::Dump(*_pool, "Description", outputPool);
		std::cout << "Frames " << _currentFrame << " of " <<  _pool->GetNumberOfContexts("Frame") << std::endl;
		delete _pool;
	}

	void doIt()
	{
		const std::vector<double> & pcp = extractor.pcp(); //pointer to chromagram data
		const std::vector<double> & chromagram = extractor.chromagram(); //pointer to chromagram data
		const Simac::CircularPeakPicking::PeakList & peaks = extractor.peaks(); //pointer to chromagram data
		const std::vector<double> & correlation = extractor.chordCorrelation(); //pointer to chromagram data
		std::string estimation = extractor.chordEstimation();

		CLAM_ASSERT(pcp.size()==12, "Unexpected pcp size" );
		_pcps[_currentFrame].Resize(pcp.size());
		_pcps[_currentFrame].SetSize(pcp.size());
		for (unsigned i =0; i<pcp.size(); i++)
			_pcps[_currentFrame][i]=pcp[i];


		unsigned correlationSize = 24;
		_chordChorrelation[_currentFrame].Resize(correlationSize);
		_chordChorrelation[_currentFrame].SetSize(correlationSize);
		for (unsigned i =0; i<correlationSize; i++)
			_chordChorrelation[_currentFrame][i]=correlation[i+1];

		_tunningPositions[_currentFrame] = extractor.tunning();
		_tunningStrength[_currentFrame] = extractor.tunningStrength();
		CLAM::TData firstCandidateWeight = correlation[extractor.firstCandidate()];
		CLAM::TData secondCandidateWeight = correlation[extractor.secondCandidate()];
		CLAM::TData noCandidateWeigth = correlation[0];
		_firstChordRelevance[_currentFrame] = firstCandidateWeight/noCandidateWeigth;
		_secondChordRelevance[_currentFrame] = secondCandidateWeight/noCandidateWeigth;
		_firstChordIndex[_currentFrame] = extractor.firstCandidate();
		_secondChordIndex[_currentFrame] = extractor.secondCandidate();
		_energies[_currentFrame] = extractor.energy();
		unsigned currentChord = firstCandidateWeight*0.6<=noCandidateWeigth || noCandidateWeigth<0.001 ?
				0 : extractor.firstCandidate();

		CLAM::TData currentTime = (_currentFrame*_hop+_firstFrameOffset)/_samplingRate;
//		_debugFrameSegmentation[0].AddElem(currentTime);
		
		ChordExtractorSegmentation segmentation(_pool, _lastChord, _chordSegmentation, currentTime, extractor);
		segmentation.doIt();

		_currentFrame++;
	}
};

class ChordExtractorSerializer
{
	const Simac::ChordExtractor & extractor;
	std::ofstream outputChromogram;
	std::ofstream outputPcp;
	std::ofstream outputPeaks;
	std::ofstream outputCorrelation;
	std::ofstream outputTuning;
	std::ofstream outputEstimation;
public:
	ChordExtractorSerializer(const std::string & filenameBase, unsigned frames, unsigned hop, unsigned framesize, const Simac::ChordExtractor & extractor)
		: extractor(extractor)
		, outputChromogram((filenameBase+".chromogram.txt").c_str())
		, outputPcp((filenameBase+".pcp.txt").c_str())
		, outputPeaks((filenameBase+".peaks.txt").c_str())
		, outputCorrelation((filenameBase+".correlation.txt").c_str())
		, outputTuning((filenameBase+".tuning.txt").c_str())
		, outputEstimation((filenameBase+".estimation.txt").c_str())
	{
	}
	~ChordExtractorSerializer()
	{
	}

	void doIt()
	{
		std::cout << extractor.chordEstimation() << std::endl;

		const std::vector<double> & pcp = extractor.pcp(); //pointer to chromagram data
		for (unsigned i=0; i<pcp.size(); i++)
			outputPcp << pcp[i] << " ";
		outputPcp << std::endl;

		const std::vector<double> & chromagram = extractor.chromagram(); //pointer to chromagram data
		for (unsigned i=0; i<chromagram.size(); i++)
			outputChromogram << chromagram[i] << " ";
		outputChromogram << std::endl;

		const Simac::CircularPeakPicking::PeakList & peaks = extractor.peaks(); //pointer to chromagram data
		for (unsigned i=0; i<peaks.size(); i++)
			outputPeaks << peaks[i].first << " ";
		outputPeaks << std::endl;

		const std::vector<double> & correlation = extractor.chordCorrelation(); //pointer to chromagram data
		for (unsigned i=0; i<correlation.size(); i++)
			outputCorrelation << correlation[i] << " ";
		outputCorrelation << std::endl;
		outputTuning
			<< extractor.tunning() 
			<< "\t" 
			<< extractor.tunningStrength()
			<< std::endl;
		outputEstimation
			<< extractor.chordEstimation()
			<< std::endl;

	}
};


int main(int argc, char* argv[])			// access command line arguments
{
	std::cout << copyright << std::endl;
	std::list<std::string> songs;
	std::string suffix = ".pool";
	std::string schemaLocation = "";
	if (argc==1) 
	{
		std::cerr << usage << std::endl;
		return -1;
	}

	bool hasSchemaOption = false;
	bool isSchema = false;
	bool isSuffix = false;
	for (unsigned i = 1; i<argc; i++)
	{
		std::string parameter = argv[i];
		if (parameter=="-h")
		{
			std::cerr << usage << std::endl;
			return -1;
		}
		else if (isSchema)
		{
			schemaLocation = parameter;
			isSchema=false;
		}
		else if (isSuffix)
		{
			suffix = parameter;
			isSuffix=false;
		}
		else if (parameter=="-f") isSuffix = true;
		else if (parameter=="-s") isSchema = hasSchemaOption = true;
		else songs.push_back(parameter);
	}

	if (schemaLocation!="")
	{
		std::ofstream schemaFile(schemaLocation.c_str());
		schemaFile << schemaContent;
	}

	for (std::list<std::string>::iterator it = songs.begin();
			it!= songs.end();
			it++)
	{
		const std::string & waveFile = *it;
		CLAM::MonoAudioFileReaderConfig cfg;
		cfg.SetSourceFile( waveFile );
		CLAM::MonoAudioFileReader reader;
		if (!reader.Configure(cfg))
		{
			std::cerr << "Error reading file '" << waveFile << "'" << std::endl;
			std::cerr << reader.GetConfigErrorMessage() << std::endl;
			return -1;
		}
		CLAM::TData samplingRate = reader.GetHeader().GetSampleRate();
		unsigned long nsamples = reader.GetHeader().GetLength()*samplingRate/1000.0;

		CLAM::XMLStorage::Dump(reader.GetHeader(), "Header", std::cout);
		CLAM::XMLStorage::Dump(reader.GetTextDescriptors(), "TextDescriptors", std::cout);
		
		int factor=1;							// downsampling factor
		float minf = 98;						// (MIDI note G1)
		unsigned bpo = 36;			// bins per octave

		Simac::ChordExtractor chordExtractor(samplingRate/factor,minf,bpo);
		unsigned framesize = chordExtractor.frameSize();
		unsigned hop = chordExtractor.hop();
		unsigned long nFrames = floor((float)(nsamples-framesize+hop)/(float)hop);	// no. of time windows
	//	unsigned long nFrames = (nsamples-framesize)/hop;	// no. of time windows
	//	ChordExtractorSerializer serializer(waveFile, nFrames, hop, framesize, chordExtractor);
		ChordExtractorDescriptionDumper dumper(waveFile, suffix, nFrames, hop, framesize, samplingRate, chordExtractor);

		std::cout << "Frame size: " << framesize << std::endl;
		std::cout << "Hop size: " << hop << std::endl;

		CLAM::AudioInPort inport;
		reader.GetOutPorts().GetByNumber(0).ConnectToIn(inport);
		inport.SetSize( framesize );
		inport.SetHop( hop );

		reader.Start();
	//	clock_t start = clock();
		std::vector<float> floatBuffer(framesize);
		while (reader.Do())
		{
			if (!inport.CanConsume()) continue; // Not enough audio, try again
			std::cout << "." << std::flush;
			CLAM::TData * segpointer = &(inport.GetAudio().GetBuffer()[0]);
			for (unsigned i = 0; i < framesize; i++)
				floatBuffer[i] = segpointer[i];
			chordExtractor.doIt(&floatBuffer[0]);
			inport.Consume();
	//		serializer.doIt();
			dumper.doIt();
		}
	//	clock_t end = clock();
		reader.Stop();
	}

//	std::cout << "\nProcessing time: " << (end-start)/CLOCKS_PER_SEC << " seconds\n";

	return 0;
}


