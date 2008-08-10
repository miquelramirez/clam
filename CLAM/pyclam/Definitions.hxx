#ifndef __clam_definitions_hpp__
#define __clam_definitions_hpp__

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

#include <vector>
#include <string>
#include <iostream>

#include <CLAM/Network.hxx>
#include <CLAM/PANetworkPlayer.hxx>
#include <CLAM/MonoAudioFileReader.hxx>
#include <CLAM/ProcessingConfig.hxx>

#include <CLAM/Flags.hxx>
#include <CLAM/SpecTypeFlags.hxx>

#include <CLAM/Array.hxx>
#include <CLAM/FFT_ooura.hxx>

#include <CLAM/Spectrum.hxx>
#include <CLAM/SpectrumConfig.hxx>
#include <CLAM/FFT.hxx>
#include <CLAM/XMLStorage.hxx>
#include <CLAM/DataTypes.hxx>
#include <CLAM/Err.hxx>

namespace Bindings {
using boost::shared_ptr;
using boost::weak_ptr;
using CLAM::TSize;
using CLAM::TData;

class Spectrum: public CLAM::Spectrum { //extra-wrap
	shared_ptr<CLAM::Spectrum> _spectrum;
public:
	Spectrum() { _spectrum = shared_ptr<CLAM::Spectrum>( new CLAM::Spectrum() ); }
	void SetType(const CLAM::SpecTypeFlags& newFlags) { _spectrum->SetType(newFlags); }
	void SetSize(int s) { _spectrum->SetSize(s); }

	CLAM::Spectrum& real() const { return *_spectrum.get(); } //Note: breaks orginal interface
	CLAM::ProcessingData& getBase() const { return dynamic_cast<CLAM::ProcessingData&>(*_spectrum.get()); } //Note: breaks orginal interface
};

//TODO: make generic as a template. template <class T> class Array and typedef Bindings::Array<TData> Bindings::DataArray;
class DataArray { //extra-wrap

	//Note: avoid shared_ptr here, will try to delete the CLAM::Array object managed internally by the CLAM library (no 'new' here)
	CLAM::DataArray* _dataArray;
public:
	DataArray(CLAM::DataArray& array) { _dataArray=&array; }

	TData& operator [](const int& i) { return (*_dataArray)[i]; }

	//FIXME: Workaround since [] operator no works with assigment (Note: bindings to std::vector work ok)
	void set(const int& i, const TData& data) { (*_dataArray)[i]=data; } //Note: breaks orginal interface

//TODO: DataArray& operator = (const DataArray& src)
//TODO: inline bool operator == (const DataArray& a, const DataArray& b)
};

class Audio: public CLAM::Audio { //extra-wrap
	shared_ptr<CLAM::Audio> _audio;
	shared_ptr<Bindings::DataArray> _dataArray;
public:
	Audio() {
		_audio = shared_ptr<CLAM::Audio>( new CLAM::Audio() );
		_dataArray = shared_ptr<Bindings::DataArray>( new Bindings::DataArray( _audio->GetBuffer() ) );
	}
	void SetSize(TSize s) { _audio->SetSize(s); }
	TSize GetSize() { _audio->GetSize(); }
	void SetSampleRate(TData s) { _audio->SetSampleRate(s); }

	shared_ptr<Bindings::DataArray> GetBuffer() { return _dataArray; }
// 	CLAM::DataArray& GetBuffer() { return _audio->GetBuffer(); } //no CLAM::DataArray class available in python

	CLAM::Audio& real() const { return *_audio.get(); } //Note: breaks orginal interface
	CLAM::ProcessingData& getBase() const { return dynamic_cast<CLAM::ProcessingData&>(*_audio.get()); } //Note: breaks orginal interface
};

// class Processing { //extra-wrap
// protected:
// 	CLAM::Processing* _proc;
// 	bool _createdHere;
// public:
// 	Processing() { _proc=0; _createdHere=true; }
// 	~Processing() { if(_proc!=0&&_createdHere) delete _proc; }
// 	Processing(const Bindings::Processing& Proc) { _proc=Proc.real(); _createdHere=false;}
// 	Processing(CLAM::Processing& proc) { _proc = &proc; _createdHere=false; }
// 
// 	CLAM::Processing* real() const { return _proc; } //Note: breaks orginal interface
// };
// class MonoAudioFileReader: public Processing { //extra-wrap
// public:
// 	MonoAudioFileReader() { _proc = dynamic_cast<CLAM::Processing*>( new CLAM::MonoAudioFileReader() ); }
// 
// 	/**	This allows 'downcasts'
// 	*	
// 	*	For example things like:
// 	*	p = clam.MonoAudioFileReader( network.GetProcessing(reader) )
// 	*	p.GetLength()
// 	*/
// 	MonoAudioFileReader(CLAM::Processing& Proc): Processing(Proc) {}
// 
// 	//Note: breaks orginal interface
// 	int GetLength() const {
// 		return (int)dynamic_cast<CLAM::MonoAudioFileReader*>(_proc)->GetHeader().GetLength()/1000;
// 	}
// };

//KLUDGE: using same alias for the clase name.
// WARNING: CLAM::ProcessingConfig [class]
// > warning W1047: There are two or more classes that use same alias("ProcessingConfig"). Duplicated aliases causes few problems,
// > but the main one is that some of the classes will not be exposed to Python.Other classes : Bindings::ProcessingConfig
// class ProcessingConfig { //extra-wrap
// protected: shared_ptr<CLAM::ProcessingConfig> _cfg;
// public:
// 	CLAM::ProcessingConfig& real() const { return *_cfg.get(); } //Note: breaks orginal interface
// };
// class MonoAudioFileReaderConfig: public ProcessingConfig { //extra-wrap
// public:
// 	MonoAudioFileReaderConfig() { _cfg = shared_ptr<CLAM::ProcessingConfig>( new CLAM::MonoAudioFileReaderConfig() ); }
// 	void SetSourceFile(char* name) { dynamic_cast<CLAM::MonoAudioFileReaderConfig*>(_cfg.get())->SetSourceFile(name); }
// };

class BPNetworkPlayer { //extra-wrap
protected: shared_ptr<CLAM::NetworkPlayer> _player;
public:
	BPNetworkPlayer() {};
	BPNetworkPlayer(const Bindings::BPNetworkPlayer& Net) { _player=Net.sharedPointer(); }

	shared_ptr<CLAM::NetworkPlayer> sharedPointer() const { return _player; } //Note: breaks orginal interface
	CLAM::NetworkPlayer& real() const { return *(_player.get()); } //Note: breaks orginal interface
};
class PANetworkPlayer: public BPNetworkPlayer {
public:
	PANetworkPlayer() { _player = shared_ptr<CLAM::NetworkPlayer>( new CLAM::PANetworkPlayer() ); }
};

//FIXME: Temporary hacks to allow some issues
namespace PyHacks {

//Note: Expected automatic upcasting seems no work in some methods called through python (no meet some methods signatures)
static inline CLAM::Component& toComponent(CLAM::ProcessingData& pd) { return dynamic_cast<CLAM::Component&>(pd); }
static inline CLAM::Component& toComponent(CLAM::Network& n) { return dynamic_cast<CLAM::Component&>(n); }
static inline CLAM::Component& toComponent(Bindings::Spectrum& s) { return toComponent(s.getBase()); }

static inline CLAM::ProcessingConfig& toProcessingConfig(CLAM::FFTConfig& fft_c) { return dynamic_cast<CLAM::ProcessingConfig&>(fft_c); }


} //namespace PyHacks


} //namespace Bindings

namespace CLAM {

//FIXME: Workaround to get the symbol for the inline function in line 107 at CLAM/Flags.hxx file
inline bool FlagsBase::CheckInvariant() {
	// Test that the names array is not a NULL pointer (it still could be invalid)
	if (!mFlagValues) {
		std::cerr << "Name definitions are a NULL pointer" << std::endl;
		return false;
	}
	// Test that a NULL name is present at the end of the names array
	unsigned int top;
	for (top=0; top<=GetNFlags() && mFlagValues[top].name; top++) {
		if (top==GetNFlags() && mFlagValues[top].name) {
			std::cerr << "There are more names than flags or there is no NULL name "
				"at the end of the name array list" << std::endl;
		return false;
		}
	}
	// Test that the names array has no values replications and no names replications
	for (unsigned int i=0; i<top; i++) {
		for (unsigned int j=i+1; j<top; j++) {
			if (std::string(mFlagValues[i].name)==std::string(mFlagValues[j].name)) {
				std::cerr << "Warning: flag names '"<< mFlagValues[i].name 
					<< "' and '" << mFlagValues[j].name 
					<< "' are replicated" << std::endl;
			}
			if (mFlagValues[i].value==mFlagValues[j].value) {
				std::cerr << "Warning: flag values '"<< mFlagValues[i].value 
					<< "' and '" << mFlagValues[j].value 
					<< "' are replicated" << std::endl;
			}
		}
	}
	return true;
}

} //namespace CLAM

#endif//__clam_definitions_hpp__
