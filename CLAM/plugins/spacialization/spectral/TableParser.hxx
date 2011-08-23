#ifndef spectral_TableParser_hxx
#define spectral_TableParser_hxx
#include <sstream>
#include <fstream>
#include <vector>
#ifdef __GNUC__
#include <cxxabi.h>
#endif//__GNUC__
#include <cstdlib>

namespace spectral { class TableParser; }


/**
A TableParser parses table based data files having a set of lines
with the same structure of fields.

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
};
@endcode

Such parser can be used as follows:
@code
StockParser parser("mystore.db");
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

If you want to ignore error lines and parse it until the end.
@code
StockParser parser("mystore.db");
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

For reading tokens, the Token template class uses the 
extraction operator '>>'. 
If you want a custom field parser you can specialize
the Token<Type>::read method or you can even create
your own BaseToken subclass.

Some already provided special tokens tokens
- Token<std::string>, redefines read so that you can 
  use ' or " to enclose multiword fields.
- LabelToken, loads a string until a special caracter
  is found (':', by default)
- RemainingToken, loads the remainig line as string,
  striping spaces at both sides.
- QuotedToken, is equivalent to Token<std::string> as
  it already supports quoted strings.

*/

class spectral::TableParser
{
public:
	class BaseToken
	{
		TableParser * _parser;
	public:
		BaseToken(TableParser * parser)
			: _parser(parser)
		{
			parser->addColumn(this);
		}
		/// Tries to read the token from stream.
		/// @return false if an error happens
		virtual bool read(std::istream & stream) = 0;
	protected:
		bool error(const std::string & error)
		{
			_parser->addError(error);
			return false;
		}
	};
	friend class BaseToken;

	template <typename TokenType>
	class Token : public BaseToken
	{
	protected:
		TokenType _value;
	public:
		Token(TableParser * parser)
			: BaseToken(parser)
		{
		}
		inline bool read(std::istream & stream)
		{
			stream >> _value;
			if (stream) return true;
			return error("Expected field of type "+
				demangle(typeid(TokenType).name()));
		}
		const TokenType & operator() () const
		{
			return _value;
		}
	};

	/// Label tokens parse a string until a stop token ':' is found
	class LabelToken : public Token<std::string>
	{
		char _stopChar;
	public:
		LabelToken(TableParser * parser, char stopChar=':')
			: Token<std::string>(parser)
			, _stopChar(stopChar)
		{
		}
		bool read(std::istream & stream)
		{
			_value="";
			stream >> std::ws;
			while (stream)
			{
				char c = stream.get();
				if (c==_stopChar) return true;
				_value.push_back(c);
			}
			return error("Label should end with a colon");
		}
	};

	/// Quoted tokens parse a single word string or multiple words enclosed by single or double quotes
	class QuotedToken : public Token<std::string>
	{
	public:
		QuotedToken(TableParser * parser)
			: Token<std::string>(parser)
		{
		}
		bool read(std::istream & stream)
		{
			_value="";
			stream >> std::ws;
			char firstChar=stream.get();
			if (not stream) return error("Expected quoted string");
			if (firstChar!='\'' and firstChar!='"')
			{
				_value.push_back(firstChar);
				firstChar=0;
			}
				
			while (stream)
			{
				char c = stream.get();
				if (not stream and firstChar)
					return error("Unclosed quote");
				if (not firstChar and not stream) return true;
				if (firstChar and c==firstChar) return true;
				if (not firstChar and isspace(c)) return true;
				_value.push_back(c);
			}
			return false;
		}
	};

	/// A token that takes the remaining text in a line (multiple words stripping spaces at the end)
	class RemainingToken : public Token<std::string>
	{
	public:
		RemainingToken(TableParser * parser)
			: Token<std::string>(parser)
		{}
		bool read(std::istream & stream)
		{
			stream >> std::ws;
			std::getline(stream, _value);
			return true;
		}
	};

protected:
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
		if (not _fstream)
			_errorMessage+="Could not open file '"+filename+"'\n";
	}
	/**
		Feeds a data line ignoring any empty or comment line.
		Returns true if such data was found, and false if there is no more lines
		or a line was malformed. With a malformed line, the parsing could continue
		for the next line. If you need to, use hasError to discriminate such a case.
	*/
	bool feedLine()
	{
		std::string readLine;
		while (readLine=="")
		{
			if (not _stream) return false; // EOF and no content read
			_line++;
			std::getline(_stream, readLine);
			readLine=parseableContent(readLine);
		}

		std::istringstream lineStream(readLine);
		for (_column=1; _column<=_columns.size(); _column++)
		{
			if (not _columns[_column-1]->read(lineStream))
				return false;
		}

		std::string remaining;
		std::getline(lineStream, remaining);
		if (remaining!="")
			return addError("Unexpected content at the end of the line");
		return true;
	}
	/**
		Returns the error messages.
	*/
	const std::string & errorMessage() const
	{
		return _errorMessage;
	}
	/**
		Returns true when some parsing error happened
	*/
	bool hasError() const
	{
		return _errorMessage!="";
	}
protected:
	/// Returns the meaningful content of a line, by removing trailing spaces and comments
	std::string parseableContent(const std::string & string)
	{
		size_t firstNotSpace = string.find_first_not_of(" \t\r\n");
		if (firstNotSpace==std::string::npos) return ""; // all spaces
		size_t firstHash = string.find_first_of("#");
		if (firstHash==0) return "";
		if (firstHash!=std::string::npos) firstHash--;
		size_t lastNotSpace = string.find_last_not_of(" \t\r\n", firstHash);
		if (lastNotSpace==std::string::npos) return "";
		size_t size = lastNotSpace-firstNotSpace+1;

		return string.substr(firstNotSpace, size);
	}
	/// Helper to be called from BaseToken constructor	
	void addColumn(BaseToken * column)
	{
		_columns.push_back(column);
	}
	/// Appends an error message
	bool addError(const std::string & message)
	{
		std::ostringstream os;
		os << "Error in line " << _line;
		if (_column<=_columns.size()) os << ", field " << _column;
		os << ": " << message << "\n";
		_errorMessage += os.str();
		return false;
	}
	/// Demangles a typeid name (just works with gcc)
	static std::string demangle(const std::string & mangledName)
	{
		std::string result = mangledName;
		#ifdef __GNUC__
		int demangleError = 0;
		char * demangled = abi::__cxa_demangle(mangledName.c_str(),0,0,&demangleError);
		if (!demangleError && demangled)
			result = demangled;
		if (demangled) free(demangled);
		#endif//__GNUC__
		return result;
	}

};

namespace spectral
{
template <>
inline bool TableParser::Token<std::string>::read(std::istream & stream)
{
	_value="";
	stream >> std::ws;
	char firstChar=stream.get();
	if (not stream) return error("Expected field of type std::string");
	if (firstChar!='\'' and firstChar!='"')
	{
		_value.push_back(firstChar);
		firstChar=0;
	}
		
	while (stream)
	{
		char c = stream.get();
		if (not stream and firstChar)
			return error("Unclosed quote");
		if (not firstChar and not stream) return true;
		if (firstChar and c==firstChar) return true;
		if (not firstChar and isspace(c)) return true;
		_value.push_back(c);
	}
	return false;
}
}

#endif//spectral_TableParser_hxx

