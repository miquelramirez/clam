#ifndef TableParser_hxx
#define TableParser_hxx
#include <sstream>
#include <fstream>
#include <vector>


/**
	A TableParser parses table based files having a set of lines
	containing each line the same set of fields of different types.
	For example, a file containing:
	@code
		# list of items in my store
		13 10.3 orange
		24 20.6 apples
	@endcode
	could be parsed by a parser defined as:
	@code
	class StockParser : public TableParser
	{
	public:
		Token<unsigned> quantity;
		Token<float> price;
		Token<string> item;
		StockParser(std::istream & stream)
			: TableParser(stream)
			, quantity(this)
			, price(this)
			, item(this)
		{}
		StockParser(const std::string & filename)
			: TableParser(filename)
			, quantity(this)
			, price(this)
			, item(this)
		{}
	}
	@endcode

	After that, the parser can be used as follows:
	@code
	StockParser parser("myStock.db");
	while (parser.feedLine())
	{
		std::cout 
			<< "There are " << parser.quantity()
			<< " items left of product " << parser.item()
			<< " at the price of " << parser.price()
			<< std::endl;
	}
	if (parser.hasError())
		std::cerr << parser.errorMessage() << std::endl;
	@endcode

	For reading tokens, the Token template uses the 
	extraction operator '>>'. 
	If you want a custom field parser you can specialize
	the Token<Type>::read method or you can even create
	your own BaseToken subclass.

*/
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

	template <typename TokenType>
	class Token : public BaseToken
	{
	public:
		TokenType _value;
		Token(TableParser * parser)
			: BaseToken(parser)
		{
		}
		bool read(std::istream & stream)
		{
			stream >> _value;
			return stream;
		}
		const TokenType & operator() () const
		{
			return _value;
		}
	};

private:
	std::ifstream _fstream;
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
	TableParser(const std::string & filename)
		: _fstream(filename.c_str())
		, _stream(_fstream)
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

