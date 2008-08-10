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
	_data.clear();
	
	_firstData.clear();
	_firstData.reserve(_config.GetFrames());
	std::cout << "reserve first " << _config.GetFrames() << " frames, " << _firstData.capacity() << " capacity, " << _firstData.size() << " size " << std::endl;
	
	_secondData.clear();
	_secondData.reserve(_config.GetFrames());
	std::cout << "reserve second " << _config.GetFrames() << " frames, " << _secondData.capacity() << " capacity, " << _secondData.size() << " size " << std::endl;
	return true;
}

bool FloatPairVectorStorage::Do()
{
	const FloatPairVector & input = _in.GetData();
	_data.push_back(input);
//	for (std::vector<const std::pair<CLAM::TData, CLAM::TData> >::iterator i = input.begin(); i != input.end(); i++)
	std::cout << "in size " << input.size() << std::endl;
	for (FloatPairVector::const_iterator i = input.begin(); i != input.begin()+1; i++)
	{
		//std::cout << "+ " << i->first << ", " << i->second << std::endl;
		_firstData.push_back(i->first);
		_secondData.push_back(i->second);
	}
	_in.Consume();
	return true;
}

const std::list<std::vector<std::pair<CLAM::TData, CLAM::TData> > >& FloatPairVectorStorage::Data() const
{
	return _data;
}

std::vector<CLAM::TData> FloatPairVectorStorage::FirstData()
{
	return _firstData;
}

std::vector<CLAM::TData> FloatPairVectorStorage::SecondData()
{
	return _secondData;
}
