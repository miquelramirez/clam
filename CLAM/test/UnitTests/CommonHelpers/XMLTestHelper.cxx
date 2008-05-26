 #include "XMLTestHelper.hxx"

#include <cppunit/SourceLine.h>
#include <cppunit/TestAssert.h>
#include <string>


namespace CLAMTest
{


/*! Uniformise an XML string.
* Strips spaces between attribut in Element.
* \warning Attribute values must be double-quoted (att="value").
* No support for embedded DTD declaration
*/

class XmlUniformiser
{
public:
	XmlUniformiser( const std::string &xml );
	std::string stripped();

private:
	void skipSpaces();
	bool isValidIndex();
	void skipNext( int count =1 );
	void copyNext( int count =1 );
	void skipProcessed();
	void skipComment();
	void copyElement();
	void copyElementContent();
	bool isSpace( char c );
	bool isSpace();
	bool startsWith( const std::string & expected );
	void copyElementName();
	void copyElementAttributes();
	void copyAttributeName();
	bool isEndOfAttributeName();
	void copyAttributeValue();
	void copyUntilDoubleQuote();

private:
	unsigned m_index;
	std::string m_xml;
	std::string m_stripped;
};


int notEqualIndex( std::string expectedXml, std::string actualXml )
{
	unsigned index = 0;
	while ( index < actualXml.length()  &&
			index < expectedXml.length()  &&
			actualXml[index] == expectedXml[index] )
		++index;

	return index;
}


/// Asserts that two XML string are equivalent.
void checkXmlEqual(
				const std::string & expectedXml,
				const std::string & actualXml,
				CppUnit::SourceLine sourceLine )
{
	std::string expected = XmlUniformiser( expectedXml ).stripped();
	std::string actual = XmlUniformiser( actualXml ).stripped();

	if ( expected == actual )
		return;

	int index = notEqualIndex( expected, actual );
	CppUnit::OStringStream message;
	message  <<  "differ at index: "  <<  index  << "\n"
		<<  "expected: "  <<  expected.substr(0,index)
			<< "\033[1;32m" <<  expected.substr(index) << "\033[0m\n"
		<<  "but was: "  <<  actual.substr(0,index)
			<< "\033[1;31m" <<  actual.substr(index) << "\033[0m\n";
	::CppUnit::Asserter::fail(
		message.str(), sourceLine );
}



XmlUniformiser::XmlUniformiser( const std::string &xml ) :
	m_index( 0 ),
	m_xml( xml )
{
}


std::string XmlUniformiser::stripped()
{
	while ( isValidIndex() )
	{
		skipSpaces();
		if ( startsWith( "<?" ) )
			skipProcessed();
		else if ( startsWith( "<!--" ) )
			skipComment();
		else if ( startsWith( "<" ) )
			copyElement();
		else
			copyElementContent();
	}
	return m_stripped;
}


void
XmlUniformiser::skipSpaces()
{
	while ( isSpace() )
		skipNext();
}


bool
XmlUniformiser::isSpace( char c )
{
	return c < 33;
}


bool
XmlUniformiser::isSpace()
{
	return isValidIndex()  &&  isSpace( m_xml[m_index] );
}


bool
XmlUniformiser::isValidIndex()
{
	return m_index < m_xml.length();
}


void
XmlUniformiser::skipNext( int count )
{
	while ( count-- > 0 )
		++m_index;
}


void
XmlUniformiser::copyNext( int count )
{
	while ( count-- > 0  &&  isValidIndex() )
		m_stripped += m_xml[ m_index++ ];
}


bool
XmlUniformiser::startsWith( const std::string & expected )
{
	std::string actual = m_xml.substr( m_index, expected.length() );
	return actual == expected;
}


void
XmlUniformiser::skipProcessed()
{
	while ( isValidIndex()  &&  !startsWith( "?>" ) )
		skipNext();
	if ( isValidIndex() )
		skipNext( 2 );
}


void
XmlUniformiser::skipComment()
{
	while ( isValidIndex()  &&  !startsWith( "-->" ) )
		skipNext();
	if ( isValidIndex() )
		skipNext( 3 );
}


void
XmlUniformiser::copyElement()
{
	copyElementName();
	copyElementAttributes();
}


void
XmlUniformiser::copyElementName()
{
	while ( isValidIndex()  &&
			!( isSpace()  ||  startsWith( ">" ) ) )
		copyNext();
}


void
XmlUniformiser::copyElementAttributes()
{
	do
	{
		skipSpaces();
		if ( startsWith( ">" ) )
			break;
		m_stripped += ' ';

		copyAttributeName();
		skipSpaces();
		if ( startsWith( "=" ) )
		{
			copyNext();
			copyAttributeValue();
		}
		else    // attribute should always be valued, ne ?
			m_stripped += ' ';
	}
	while ( isValidIndex() );
	copyNext();
}


void
XmlUniformiser::copyAttributeName()
{
	while ( isValidIndex()  &&  !isEndOfAttributeName() )
		copyNext();
}


bool
XmlUniformiser::isEndOfAttributeName()
{
	return isSpace()  ||  startsWith( ">" )  ||  startsWith( "=" );
}


void
XmlUniformiser::copyAttributeValue()
{
	skipSpaces();
	copyUntilDoubleQuote();
	copyUntilDoubleQuote();
}


void
XmlUniformiser::copyUntilDoubleQuote()
{
	while ( isValidIndex()  &&  !startsWith("\"") )
		copyNext();
	copyNext();   // '"'
}


void
XmlUniformiser::copyElementContent()
{
	while ( isValidIndex()  &&  !startsWith( "<" ) )
		copyNext();
}


}  // namespace CppUnitTest

