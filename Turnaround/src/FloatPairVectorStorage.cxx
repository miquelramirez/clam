#include "FloatPairVectorStorage.hxx"

#include <iostream> // debug

void FloatPairVectorStorageConfig::DefaultInit(void)
{
	AddAll();

	UpdateData();

	SetFrames(0);
}

FloatPairVectorStorage::FloatPairVectorStorage(const Config& cfg)
	: _in("Data Input", this)
{
	Configure(cfg);
}

FloatPairVectorStorage::~FloatPairVectorStorage()
{
}

// TODO: move it to the header
const char* FloatPairVectorStorage::GetClassName() const
{
	return "FloatPairVectorStorage";
}

bool FloatPairVectorStorage::ConcreteConfigure(const CLAM::ProcessingConfig& cfgObject)
{
	CopyAsConcreteConfig(_config, cfgObject);
	return true;
}

bool FloatPairVectorStorage::ConcreteStart()
{
	_positionStorage.clear();
	_magnitudeStorage.clear();
	return true;
}

bool FloatPairVectorStorage::Do()
{
	const FloatPairVector & input = _in.GetData();
	_positionStorage.push_back(std::vector<CLAM::TData> ());
	_magnitudeStorage.push_back(std::vector<CLAM::TData> ());
	std::vector<CLAM::TData> & positions = _positionStorage.back();
	std::vector<CLAM::TData> & magnitudes = _magnitudeStorage.back();
	
	for (FloatPairVector::const_iterator i = input.begin(); i != input.end(); i++)
	{
		positions.push_back(i->first);
		magnitudes.push_back(i->second);
	}
	_in.Consume();
	return true;
}

const std::list<std::vector<CLAM::TData> >& FloatPairVectorStorage::PositionStorage() const
{
	return _positionStorage;
}

const std::list<std::vector<CLAM::TData> >& FloatPairVectorStorage::MagnitudeStorage() const
{
	return _magnitudeStorage;
}
