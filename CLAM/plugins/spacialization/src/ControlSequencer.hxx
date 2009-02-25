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
 Sends the control values stored in a file in tab separated columns.
 @todo: move ControlSequencer into CLAM as it is too general to be in the spacialization plugin.
*/

class ControlSequencer : public CLAM::Processing
{
	class Config : public CLAM::ProcessingConfig
	{ 
		DYNAMIC_TYPE_USING_INTERFACE( Config, 5, ProcessingConfig );
		DYN_ATTRIBUTE( 0, public, InFilename, Filename);
		DYN_ATTRIBUTE( 1, public, unsigned, FrameSize);
		DYN_ATTRIBUTE( 2, public, unsigned, SampleRate);
		DYN_ATTRIBUTE( 3, public, unsigned, NumberOfColumns);
		DYN_ATTRIBUTE( 4, public, unsigned, ControlsPerSecond);
	protected:
		void DefaultInit()
		{
			AddAll();
			UpdateData();
			SetFilename("");
			SetFrameSize(512);
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
			//std::cout << ", ctl "<<_sequenceIndex << "/" <<_controlSequence.size() << std::flush;
			//TODO check that _indexOut1,2,3 < _controlSequence[_sequenceIndex].size()
			for (unsigned i=0;i<_config.GetNumberOfColumns();i++)
			{
				_outControls[i]->SendControl ( _controlSequence[_sequenceIndex][i] );
			}
			_sampleCount -= _samplesPerControl;
			_sequenceIndex++;
			if (_sequenceIndex >= _controlSequence.size())
			{
				_sequenceIndex=0;
				std::cout << std::endl << "\n End of control sequence. Starting new iteration\n"<<std::endl;;
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
		RemoveOldControls();
	}
protected:

	void RemoveOldControls()
	{
		std::vector<FloatOutControl*>::iterator it;
		for (it=_outControls.begin();it!=_outControls.end();it++)
		{
			delete *it;
		}
		_outControls.clear();
	}

	void CreateControls()
	{
		for (unsigned i=0;i<_config.GetNumberOfColumns();i++)
		{
			FloatOutControl * outControl=new FloatOutControl ("Output "+i,this);
			_outControls.push_back(outControl);
		}
	}

	bool ConcreteConfigure(const CLAM::ProcessingConfig & config)
	{
		CopyAsConcreteConfig(_config, config);
		_syncIn.SetSize( _config.GetFrameSize() );
		_syncIn.SetHop( _config.GetFrameSize() );
		_samplesPerControl = _config.GetSampleRate()/_config.GetControlsPerSecond();
		_sampleCount=0;
		_frameSize=_config.GetFrameSize();
		_sequenceIndex=0;
		_controlSequence.clear();

		//Load the sequence
		if (_config.GetFilename()=="") 
		{
			AddConfigErrorMessage("Not filename defined");
			return false;
		}
		std::cout << "ControlSequencer: reading file: "<< _config.GetFilename() << std::endl;
		// Load table from file
		std::ifstream file( _config.GetFilename().c_str() );
		while (file)
		{
			std::string line;
			std::getline(file, line);
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
			if ( row.size() != _config.GetNumberOfColumns() )
			{
				AddConfigErrorMessage("Columns number in file differs to configuration.");
				return false;
			}
			_controlSequence.push_back(row);
		}

		RemoveOldControls();
		CreateControls();
		return true;
	}
};

} //namespace

#endif
