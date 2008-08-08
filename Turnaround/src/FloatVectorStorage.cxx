#include "FloatVectorStorage.hxx"

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

bool FloatVectorStorage::ConcreteStart()
{
	_data.clear();
	return true;
}

bool FloatVectorStorage::Do()
{
	for (FloatVector::iterator i = _in.GetData().begin(); i != _in.GetData().end(); i++)
		_data.push_back(*i);
	_in.Consume();
	return true;
}

std::vector<CLAM::TData> FloatVectorStorage::Data()
{
	return _data;
}
