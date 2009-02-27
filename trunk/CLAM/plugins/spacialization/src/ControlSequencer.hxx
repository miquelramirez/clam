#ifndef ControlSequencer_hxx
#define ControlSequencer_hxx

#include <CLAM/Processing.hxx>
#include <CLAM/OutControl.hxx>
#include <CLAM/AudioInPort.hxx>
#include <CLAM/Filename.hxx>
#include <string>
#include <cmath>
#include <fstream>

namespace CLAM
{

/**
 Sends the control values stored in a file in space separated columns.
 @todo: move ControlSequencer into CLAM as it is too general to be in the spacialization plugin.
*/

class ControlSequencer : public CLAM::Processing
{
	class Config : public CLAM::ProcessingConfig
	{ 
		DYNAMIC_TYPE_USING_INTERFACE( Config, 4, ProcessingConfig );
		DYN_ATTRIBUTE( 0, public, InFilename, Filename);
		DYN_ATTRIBUTE( 1, public, unsigned, SampleRate);
		DYN_ATTRIBUTE( 2, public, unsigned, NumberOfColumns);
		DYN_ATTRIBUTE( 3, public, unsigned, ControlsPerSecond);
	protected:
		void DefaultInit()
		{
			AddAll();
			UpdateData();
			SetFilename("");
			SetSampleRate(48000);
			SetNumberOfColumns(1);
			SetControlsPerSecond(24);
		};
	};

	Config _config;
	std::vector<FloatOutControl*> _outControls;
	AudioInPort _syncIn;

	unsigned _samplesPerControl;
	unsigned _sampleCount;
	unsigned _frameSize;
	unsigned _sequenceIndex;

	typedef std::vector<float> Row;
	std::vector<Row> _controlSequence;

public:
	ControlSequencer(const Config& config = Config()) 
		: _syncIn("sync", this)
	{
		Configure( config );
	}
	const CLAM::ProcessingConfig & GetConfig() const
	{
		return _config;
	}
	bool Do()
	{
		CLAM_DEBUG_ASSERT(_sampleCount<2*_samplesPerControl,"_sampleCount too large" );
		_sampleCount += _frameSize;
		if (_sampleCount>=_samplesPerControl)
		{
			for (unsigned i=0;i<_config.GetNumberOfColumns();i++)
			{
				_outControls[i]->SendControl ( _controlSequence[_sequenceIndex][i] );
			}
			_sampleCount -= _samplesPerControl;
			_sequenceIndex++;
			if (_sequenceIndex >= _controlSequence.size())
			{
				_sequenceIndex=0;
//				std::cout << std::endl << "\n End of control sequence. Starting new iteration\n"<<std::endl;
			}
		}
		_syncIn.Consume();
		return true;
	}
	const char* GetClassName() const
	{
		return "ControlSequencer";
	}

	~ControlSequencer()
	{
		ResizeControls(0);
	}
protected:

	void ResizeControls(unsigned newSize)
	{
		for (unsigned i=_outControls.size(); i<newSize; i++)
		{
			std::ostringstream name;
			name << "Output " <<i;
			FloatOutControl * outControl=new FloatOutControl (name.str(),this);
			_outControls.push_back(outControl);
		}
		for (unsigned i=newSize; i<_outControls.size(); i++)
			delete _outControls[i];
		_outControls.resize(newSize);
	}

	bool ConcreteConfigure(const CLAM::ProcessingConfig & config)
	{
		CopyAsConcreteConfig(_config, config);
		_frameSize=BackendBufferSize();
		_syncIn.SetSize( _frameSize );
		_syncIn.SetHop( _frameSize );
		_samplesPerControl = _config.GetSampleRate()/_config.GetControlsPerSecond();
		_sampleCount=0;
		_sequenceIndex=0;
		_controlSequence.clear();

		//Load the sequence
		if (_config.GetFilename()=="") 
			return AddConfigErrorMessage("Not filename defined");

		// Load table from file
		std::ifstream file( _config.GetFilename().c_str() );
		if (not file)
			return AddConfigErrorMessage("Unable to open the file "+_config.GetFilename());
		unsigned lineCount = 0;
		while (file)
		{
			lineCount++;
			std::string line;
			std::getline(file, line);
			line.erase(0,line.find_first_not_of("\t\r\n "));
			if (line=="" or line[0]=='#') continue;
			std::istringstream is(line);
			Row row;
			while (is and not is.eof())
			{
				float data;
				is >> data;
				if (not is) break;
				row.push_back(data);
			}
			if (not is.eof())
			{
				std::ostringstream os;
				os
					<< "Bad sequence file, at line " << lineCount 
					<< ", not a float number at columns " << row.size() << ".";
				return AddConfigErrorMessage(os.str());
			}
			if ( row.size() != _config.GetNumberOfColumns() )
			{
				std::ostringstream os;
				os
					<< "Bad sequence file, at line " << lineCount 
					<< ", expected "
					<< _config.GetNumberOfColumns() << " columns, but got " << row.size() << ".";
				return AddConfigErrorMessage(os.str());
			}
			_controlSequence.push_back(row);
		}

		ResizeControls(_config.GetNumberOfColumns());
		return true;
	}
};

} //namespace

#endif
