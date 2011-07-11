#ifndef TableParser_hxx
#define TableParser_hxx
#include <iosfwd>

class TableParser
{
	class IntToken
	{
	public:
		int _value;
		bool read(std::istream & stream)
		{
			stream >> _value;
			return false; // not tested
		}
		int operator() ()
		{
			return _value;
		}
	};

	std::istream & _stream;
public:
	IntToken intColumn;
	TableParser(std::istream & stream)
		: _stream(stream)
	{
	}
	bool feedLine()
	{
		intColumn.read(_stream);
		return _stream;
	}
	const std::string & errorMessage() const
	{
		static std::string message =
			"boo";
	}
	bool hasError() const
	{
		return false;
	}
};

#endif//TableParser_hxx

