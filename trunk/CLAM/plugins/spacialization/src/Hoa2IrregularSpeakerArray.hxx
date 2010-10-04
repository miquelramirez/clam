#ifndef Hoa2IrregularSpeakerArray_hxx
#define Hoa2IrregularSpeakerArray_hxx
#include <CLAM/AudioInPort.hxx>
#include <CLAM/AudioOutPort.hxx>
#include <CLAM/Processing.hxx>
#include <CLAM/Audio.hxx>
#include <CLAM/InControl.hxx>
#include <CLAM/Filename.hxx>
#include <CLAM/Enum.hxx>
#include "Orientation.hxx"
#include "SpeakerLayout.hxx"
#include <cmath>

/**
 Decodes a Nth order signal for a irregular array of speakers.
 The decoding to use is read from a text file containing a row for each speaker
 and a column to each ambisonics channel with the weight to apply.
 @param Order [Config] The order of ambisonics that it will be fed with.
 @param DecodingFile [Config] A file containing the weights
 @param[out] XX [Port] Ambisonics component were XX is one of W,X,Y,Z,R,S,T,U,V,K,L,M,N,O,P,Q.
 @param[out] AAA [Ports] Audio signals to be emitted by the speaker AAA. \
   Here AAA is the index for the speaker 01, 02...
 @ingroup SpatialAudio
 @see AmbisonicsConventions
*/
class Hoa2IrregularSpeakerArray : public CLAM::Processing
{
public:
	class DecodingMatrix
	{
		unsigned _nInputs;
		unsigned _nOutputs;
		float * _weights;
	public:
		DecodingMatrix()
			: _nInputs(0)
			, _nOutputs(0)
			, _weights(0)
		{
		}
		~DecodingMatrix()
		{
			if (_weights) delete [] _weights;
		}
		bool load(unsigned nInputs, unsigned nOutputs, const std::string & filePath, std::string & errorMsg);
		float weight(unsigned input, unsigned output) const { return _weights[output*_nInputs+output]; }
	};


	class Config : public CLAM::ProcessingConfig
	{
		DYNAMIC_TYPE_USING_INTERFACE( Config, 3, ProcessingConfig );
		DYN_ATTRIBUTE( 0, public, unsigned, Order);
		DYN_ATTRIBUTE( 1, public, unsigned, NSpeakers);
		DYN_ATTRIBUTE( 2, public, CLAM::InFilename, DecodingFile);
	protected:
		void DefaultInit()
		{
			AddAll();
			UpdateData();
			SetOrder(1);
		}
	};
private:
	DecodingMatrix _decodingMatrix;
	typedef std::vector<CLAM::AudioOutPort*> OutPorts;
	typedef std::vector<CLAM::AudioInPort*> InPorts;
	OutPorts _outputs;
	InPorts _inputs;
	Config _config;
	enum {MaxSupportedOrder=3};
	double _decoding[MaxSupportedOrder+1];

public:
	const char* GetClassName() const { return "Hoa2IrregularSpeakerArray"; }
	Hoa2IrregularSpeakerArray(const Config& config = Config()) 
	{
		Configure( config );
	}
	const CLAM::ProcessingConfig & GetConfig() const
	{
		return _config;
	}

	bool ConcreteConfigure(const CLAM::ProcessingConfig& config)
	{
		CopyAsConcreteConfig(_config, config);
		unsigned buffersize = BackendBufferSize();
		unsigned order = _config.GetOrder();
		unsigned nSpeakers = _config.GetNSpeakers();
		ResizePortsToOrder(order, buffersize);
		ResizePortsToLayout(nSpeakers, buffersize);
		std::string errorMessage;
		if (not _decodingMatrix.load(
			(order+1)*(order+1),
			nSpeakers,
			_config.GetDecodingFile(),
			errorMessage))
			return AddConfigErrorMessage(errorMessage);
		return true;
	}
	bool Do()
	{
		// Ambisonics definition assures us one component for order 0 so we can do that:
		const unsigned nSamples = _inputs[0]->GetAudio().GetBuffer().Size();
		const unsigned nComponents = _inputs.size();
		const unsigned nSpeakers = _outputs.size();
		const CLAM::TData* components[nComponents];
		for (unsigned component=0; component<nComponents; component++)
			components[component] = &_inputs[component]->GetAudio().GetBuffer()[0];
		CLAM::TData* speakers[nSpeakers];
		for (unsigned speaker=0; speaker<nSpeakers; speaker++)
		{
			CLAM::Audio & audio = _outputs[speaker]->GetAudio();
			speakers[speaker] = &audio.GetBuffer()[0];
		}

		
		for (unsigned speaker=0; speaker<nSpeakers; speaker++)
		{
			CLAM::TData * speakerBuffer = speakers[speaker];
			for (unsigned sample=0; sample<nSamples; sample++)
			{
				double sampleValue = 0;
				for (unsigned component=0; component<nComponents; component++)
					sampleValue += components[component][sample] * _decodingMatrix.weight(component, speaker);
				speakerBuffer[sample] = sampleValue; //The 2 factor makes the volume comparable to VBAP
			}
		}

		for (unsigned speaker=0; speaker<nSpeakers; speaker++)
			_outputs[speaker]->Produce();
		for (unsigned component=0; component<nComponents; component++)
			_inputs[component]->Consume();
		return true;
	}
	~Hoa2IrregularSpeakerArray()
	{
		for (unsigned speaker=0; speaker<_outputs.size(); speaker++)
			delete _outputs[speaker];
		for (unsigned component=0; component<_inputs.size(); component++)
			delete _inputs[component];
	}
private:
	void ResizePortsToOrder(unsigned order, unsigned buffersize)
	{
		// Set up the inputs according to ambisonics order
		CLAM::SphericalHarmonicsDefinition *sh = CLAM::Orientation::sphericalHarmonics();
		unsigned i=0;
		for (;sh[i].name; i++)
		{
			if (sh[i].order > order) break;
			if (i<_inputs.size())
			{
				_inputs[i]->SetSize( buffersize );
				_inputs[i]->SetHop( buffersize );
				continue;
			}
			CLAM::AudioInPort * port = new CLAM::AudioInPort( sh[i].name, this);
			port->SetSize( buffersize );
			port->SetHop( buffersize );
			_inputs.push_back( port );
		}
		unsigned actualSize=i;
		for (;i<_inputs.size(); i++)
			delete _inputs[i];
		_inputs.resize(actualSize);
	}
	void ResizePortsToLayout(unsigned nSpeakers, unsigned buffersize)
	{
		unsigned previousSize = _outputs.size();
		for (unsigned speaker=nSpeakers; speaker<previousSize; speaker++)
			delete _outputs[speaker];
		_outputs.resize(nSpeakers);
		// adding new speakers
		for (unsigned speaker=previousSize; speaker<nSpeakers; speaker++)
		{
			CLAM::AudioOutPort * port = new CLAM::AudioOutPort( portName(speaker), this);
			_outputs.push_back( port );
		}
		for (unsigned speaker=0; speaker<nSpeakers; speaker++)
		{
			_outputs[speaker]->SetSize( buffersize );
			_outputs[speaker]->SetHop( buffersize );
		}
	}
	std::string portName(unsigned speaker) const
	{
		std::ostringstream os;
		os << std::setw(2) << std::setfill('0') << (speaker+1);
		return os.str();
	}

};
#endif

