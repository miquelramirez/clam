#ifndef _DomPrint
#define _DomPrint


// MRJ: The Xercesc-1.7.0 internal headers have hardwired the 
// path <xerces/whatever/etc> so since I don't feel like 
// hacking the official xerces headers I think more reasonable
// our files that attack directly the xerces headers
#ifndef WIN32
#include <util/PlatformUtils.hpp>
#include <util/XMLString.hpp>
#include <util/XMLUniDefs.hpp>
#include <framework/XMLFormatter.hpp>
#include <util/TranscodingException.hpp>
#include <dom/DOM_DOMException.hpp>
#include <dom/DOM.hpp>
#include <parsers/DOMParser.hpp>
#else
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/XMLUniDefs.hpp>
#include <xercesc/framework/XMLFormatter.hpp>
#include <xercesc/util/TranscodingException.hpp>
#include <xercesc/dom/DOM_DOMException.hpp>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/parsers/DOMParser.hpp>

#endif

#include <iostream>
#include <cstring>
#include <cstdlib>


// ---------------------------------------------------------------------------
//  Local classes
// ---------------------------------------------------------------------------

class DOMPrintFormatTarget : public XMLFormatTarget
{
public:
	DOMPrintFormatTarget()  {};
	DOMPrintFormatTarget(std::ostream& target)  {mpTarget=&target;};//added by XA
	~DOMPrintFormatTarget() {};

	// -----------------------------------------------------------------------
	//  Implementations of the format target interface
	// -----------------------------------------------------------------------

	void writeChars(const   XMLByte* const  toWrite,
	                const   unsigned int    count,
                        XMLFormatter * const formatter)
	{
		// Surprisingly, Solaris was the only platform on which
		// required the char* cast to print out the string correctly.
		// Without the cast, it was printing the pointer value in hex.
		// Quite annoying, considering every other platform printed
		// the string with the explicit cast to char* below.
		mpTarget->write((char *) toWrite, (int) count);
	};

private:
	// -----------------------------------------------------------------------
	//  Unimplemented methods.
	// -----------------------------------------------------------------------
	DOMPrintFormatTarget(const DOMPrintFormatTarget& other);
	void operator=(const DOMPrintFormatTarget& rhs);
	std::ostream* mpTarget;//mpTarget added by XA in order to use other ostream than cout
};

// ---------------------------------------------------------------------------
//  Forward references
// ---------------------------------------------------------------------------
std::ostream& operator<<(std::ostream& target, const DOMString& toWrite);
std::ostream& operator<<(std::ostream& target, const XMLCh* toWrite);
std::ostream& operator<<(std::ostream& target, DOM_Node& toWrite);
XMLFormatter& operator<< (XMLFormatter& strm, const DOMString& s);
std::ostream& PrintDoc(std::ostream& target, DOM_Node& toWrite);

void Inspect (std::ostream& target, DOM_Node& toWrite);

#endif //_DomPrint
