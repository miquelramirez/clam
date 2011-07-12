#ifndef TableParser_hxx
#define TableParser_hxx
#include <sstream>
#include <vector>

class TableParser
{
public:
	class BaseToken
	{
	public:
		BaseToken(TableParser * parser)
		{
			parser->addColumn(this);
		}
		virtual bool read(std::istream & stream) = 0;
	};
	class IntToken : public BaseToken
	{
	public:
		int _value;
		IntToken(TableParser * parser)
			: BaseToken(parser)
		{
		}
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
private:
	std::istream & _stream;
	std::string _errorMessage;
	unsigned _line;
	unsigned _column;
	std::vector<BaseToken*> _columns;
public:
	TableParser(std::istream & stream)
		: _stream(stream)
		, _line(0)
		, _column(0)
	{
	}
	void addColumn(BaseToken * column)
	{
		_columns.push_back(column);
	}
	bool feedLine()
	{
		std::string line;
		while (isJustSpaces(line) or line[0]=='#')
		{
			if (not _stream) return false; // No content read
			_line++;
			std::getline(_stream, line);
		}

		std::istringstream lineStream(line);
		for (_column=1; _column<=_columns.size(); _column++)
		{
			if (not _columns[_column-1]->read(lineStream))
				return addError("Expected an int");
		}

		if (not isJustSpaces(lineStream))
			return addError("Unexpected content at the end of the line");
		return true;
	}
private:
	bool isJustSpaces(std::istream & stream)
	{
		stream >> std::ws;
		return stream.eof();
	}
	bool isJustSpaces(const std::string & string)
	{
		std::istringstream stream(string);
		return isJustSpaces(stream);
	}
public:
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
		os << "Error in line " << _line;
		if (_column<=_columns.size()) os << ", token " << _column;
		os << ": " << message << "\n";
		_errorMessage += os.str();
		return false;
	}
};

#endif//TableParser_hxx

