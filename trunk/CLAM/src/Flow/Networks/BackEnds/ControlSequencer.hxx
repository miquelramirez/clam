#ifndef ControlSequencer_hxx
#define ControlSequencer_hxx

#include "Network.hxx"
#include "ControlSource.hxx"

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>

namespace CLAM 
{

class ControlSequencer
{
	std::string _coreoFile;
	double _controlRate;
	int _frameSize;
	int _sampleRate;
	CLAM::Network& _net;

	typedef std::vector<double> Values;

	struct CoreoEntry
	{
		std::string mtc;
		std::string name;
		Values values;
	};

	typedef std::vector<CoreoEntry> CoreoEntries;
	CoreoEntries _coreoEntries;
	CoreoEntries::iterator _coreoEvent; //const

	double SmpteToMs(const std::string& mtc)
	{
		std::vector<std::string> elems;
		std::stringstream ss(mtc);
		std::string item;
		while(std::getline(ss, item, ':')) 
				elems.push_back(item);

		CLAM_ASSERT(elems.size() == 4, 
			std::string("Invalid MTC found in sequencefile (not like nn:nn:nn:nn)"
				" Offending MTC is: \'" + mtc + "\'. Please correct!").c_str());

		int hours = atoi(elems[0].c_str());
		int minutes = atoi(elems[1].c_str());
		int seconds = atoi(elems[2].c_str());
		int frames = atoi(elems[3].c_str());
		double frame_length = 1000. / _controlRate;

		double mtcInMillisec = 
			(double)hours * 3600000. + (double)minutes * 60000. 
				+ (double)seconds * 1000. + (double)frames * frame_length;

		return mtcInMillisec;
	}
	
	std::string MsToSmpte(int milliseconds)
	{
		std::stringstream hours;
		hours.width(2);
		hours.fill('0');
		hours << int(milliseconds / 3600000);
		std::stringstream minutes;
		minutes.width(2);
		minutes.fill('0');
		minutes << int((milliseconds % 3600000)/60000);
		std::stringstream seconds;
		seconds.width(2);
		seconds.fill('0');
		seconds << int((milliseconds % 3600000)%60000/1000);
		std::stringstream frames;
		frames.width(2);
		frames.fill('0');
		double frame_length = 1000. / _controlRate;
		frames << int(int(((milliseconds % 3600000)%60000)%1000)/frame_length);

		std::string smpte = 
			hours.str() + ":" + minutes.str() + ":" + seconds.str() + ":" +frames.str();

		return smpte;
	}

	// Reads lines like '00:00:00:00 /gaincontrol 1.0 2.0 3.0'
	void PreprocessCoreoFile()
	{
		std::ifstream file(_coreoFile.c_str(), std::ifstream::in);

		std::string mtc;
		std::string name;
		std::string values;
		
		std::string line;
		while(std::getline(file, line))
		{
			std::stringstream is(line);
			is >> mtc >> name;

			Values values;
			while (is && !is.eof())
			{
				float value;
				is >> value;
				values.push_back(value);
			}

			// dont store wrong input
			if (mtc == "" || name == "" || values.empty())
				continue;

			if (name.find("FPS") != std::string::npos)
				_controlRate = values[0];

			// dont store setup
			if (mtc == "setup")
				continue;
			
			#if 0
			std::cout << "mtc=" << mtc << " name=" << name << " values=";
			std::copy(values.begin(), values.end(), std::ostream_iterator<double>(std::cout, " "));
			std::cout << std::endl; 
			#endif

			CoreoEntry ce = { mtc, name, values };
			_coreoEntries.push_back(ce); 
		}
		//std::cout << "framerate=" << _controlRate << std::endl;
		_coreoEvent = _coreoEntries.begin();
	}


public:
	ControlSequencer(std::string const& coreo_file,	int frameSize, int sampleRate, CLAM::Network& net)
	: _coreoFile(coreo_file)
	, _controlRate(25.000)
	, _frameSize(frameSize)
	, _sampleRate(sampleRate)
	, _net(net)
	{
		PreprocessCoreoFile();
	}

	~ControlSequencer()
	{
	}
 
	void UpdateControls(int buffernr)
	{
		double currentTimeInMillisec = 
			((buffernr * (double)_frameSize) / (double)_sampleRate) * 1000.;
		for (; _coreoEvent != _coreoEntries.end() &&
					 SmpteToMs(_coreoEvent->mtc) <= currentTimeInMillisec;
					 ++_coreoEvent) 
		{
			std::string const& controlName = _coreoEvent->name;
			//std::cout << "buffer=" << buffernr
			//					<< " smpte=" << MsToSmpte(currentTimeInMillisec)
			//					<< " controlname=" << controlName
			//					<< " coreo event in samples=" << SmpteToMs(_coreoEvent->mtc) * 48
			//					<< " sample=" << buffernr * _framesize;
		 
			// check for wrong input
			if (!_net.HasProcessing(controlName))
			{
				std::cout 
					<< "Error! ControlSource named \'" << controlName 
					<< "\' not found in Network!" << std::endl;
				continue;
			}

			size_t size = _coreoEvent->values.size();
			//std::cout << " coreo_values_size=" << size;

			// check for wrong input
			if (size == 0)
			{
				std::cout
					<< "Error!\tNo value found for timecode \'" 
					<< MsToSmpte(currentTimeInMillisec) << "\'"
					<< " with MTC path \'" << _coreoEvent->name << "\'"
					<< std::endl;
				continue;
			}

			CLAM::Processing& control = _net.GetProcessing(controlName);
			CLAM::ControlSource* controlSource = dynamic_cast<CLAM::ControlSource*>(&control);

			if (!controlSource )
			{
				std::cout
					<< "Error\tControlSequencer: '" 
					<< controlName << "' is not a ControlSource"
					<< std::endl;
				continue;
			}

			const double& value = _coreoEvent->values[0];

			// TODO: support sources with multiple out controls
			controlSource->Do(value);
		}
		//std::cout << "current_buffer=" << buffernr
		//					<< " currentTimeInMillisec=" << currentTimeInMillisec
		//					<< " sample=" << buffernr * framesize
		//					<< std::endl;
	}
};

}
#endif

