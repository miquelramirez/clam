#include "FloatVectorStorage.hxx"

#include <iostream> // debug

void FloatVectorStorageConfig::DefaultInit(void)
{
	AddAll();

	UpdateData();

	SetBins(12);
	SetFrames(0);
}

FloatVectorStorage::FloatVectorStorage(const Config& cfg)
	: _in("Data Input", this)
{
	Configure(cfg);
}

FloatVectorStorage::~FloatVectorStorage()
{
}

// TODO: move it to the header
const char* FloatVectorStorage::GetClassName() const
{
	return "FloatVectorStorage";
}

bool FloatVectorStorage::ConcreteConfigure(const CLAM::ProcessingConfig& cfgObject)
{
	CopyAsConcreteConfig(_config, cfgObject);
	return true;
}

bool FloatVectorStorage::ConcreteStart()
{
	_data.clear();
	_data.reserve(_config.GetBins() * _config.GetFrames());
	std::cout << "reserve " << _config.GetBins() << " bins, " << _config.GetFrames() << " frames, " << _data.capacity() << " capacity, " << _data.size() << " size " << std::endl;
	return true;
}

bool FloatVectorStorage::Do()
{
	const std::vector<CLAM::TData> & input = _in.GetData();
	_data.insert(_data.end(), input.begin(), input.end());
	_in.Consume();
	return true;
}

std::vector<CLAM::TData> FloatVectorStorage::Data()
{
	return _data;
}
