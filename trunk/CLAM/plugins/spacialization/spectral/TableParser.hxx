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
			return stream;
		}
		int operator() ()
		{
			return _value;
		}
	};

	std::istream & _stream;
	std::string _errorMessage;
	unsigned _line;
public:
	IntToken intColumn;
	TableParser(std::istream & stream)
		: _stream(stream)
		, _line(0)
	{
	}
	bool feedLine()
	{
		_line++;
		std::string line;
		std::getline(_stream, line);
//		std::cout << "::" << _line << ":" << line<< std::endl;
		if (line=="" and not _stream) return false;
		if (line=="") return feedLine();
		if (line[0]=='#') return feedLine();

		std::istringstream lineStream(line);

		if (not intColumn.read(lineStream))
			return addError("Expected an int");
		return true;
	}
	const std::string & errorMessage() const
	{
		return _errorMessage;
	}
	bool hasError() const
	{
		return _errorMessage!="";
	}
	bool addError(const std::string & message)
	{
		std::ostringstream os;
		os << "Error in line " << _line << ", token 1: " << message << "\n";
		_errorMessage += os.str();
		return false;
	}
};

#endif//TableParser_hxx

