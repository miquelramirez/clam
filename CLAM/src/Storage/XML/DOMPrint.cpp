/*
 * The Apache Software License, Version 1.1
 *
 * Copyright (c) 1999-2000 The Apache Software Foundation.  All rights
 * reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * 3. The end-user documentation included with the redistribution,
 *    if any, must include the following acknowledgment:
 *       "This product includes software developed by the
 *        Apache Software Foundation (http://www.apache.org/)."
 *    Alternately, this acknowledgment may appear in the software itself,
 *    if and wherever such third-party acknowledgments normally appear.
 *
 * 4. The names "Xerces" and "Apache Software Foundation" must
 *    not be used to endorse or promote products derived from this
 *    software without prior written permission. For written
 *    permission, please contact apache\@apache.org.
 *
 * 5. Products derived from this software may not be called "Apache",
 *    nor may "Apache" appear in their name, without prior written
 *    permission of the Apache Software Foundation.
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESSED OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED.  IN NO EVENT SHALL THE APACHE SOFTWARE FOUNDATION OR
 * ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
 * USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 * ====================================================================
 *
 * This software consists of voluntary contributions made by many
 * individuals on behalf of the Apache Software Foundation, and was
 * originally based on software copyright (c) 1999, International
 * Business Machines, Inc., http://www.ibm.com .  For more information
 * on the Apache Software Foundation, please see
 * <http://www.apache.org/>.
 */


// ---------------------------------------------------------------------------
//  This sample program which invokes the DOMParser to build a DOM tree for
//  the specified input file. It then walks the tree, and prints out the data
//  as an XML file.
//
//   Limitations:
//      1.  The encoding="xxx" clause in the XML header should reflect
//          the system local code page, but does not.
//      2.  Cases where the XML data contains characters that can not
//          be represented in the system local code page are not handled.
//      3.  Enabled namespace processing won't affect the output, since
//          DOM doesn't do namespace yet. But it will confirm that all
//          prefixes are correctly mapped, else you'll get errors.
// ---------------------------------------------------------------------------


// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include "DOMPrint.hpp"
#include <string>

#define TRACEDUMP if (1); else std::cout

// ---------------------------------------------------------------------------
//  Local data
//
//  gEncodingName
//      The encoding we are to output in. If not set on the command line,
//      then it is defaults to the encoding of the input XML file.
//
// ---------------------------------------------------------------------------
static XMLCh*                   gEncodingName          = 0;
static XMLFormatter::UnRepFlags gUnRepFlags            = XMLFormatter::UnRep_CharRef;
static XMLFormatter*            gFormatter             = 0;


// ---------------------------------------------------------------------------
//  Local const data
//
//  Note: This is the 'safe' way to do these strings. If you compiler supports
//        L"" style strings, and portability is not a concern, you can use
//        those types constants directly.
// ---------------------------------------------------------------------------
static const XMLCh  gEndElement[] = { chOpenAngle, chForwardSlash, chNull };
static const XMLCh  gEndPI[] = { chQuestion, chCloseAngle, chNull};
static const XMLCh  gStartPI[] = { chOpenAngle, chQuestion, chNull };
static const XMLCh  gXMLDecl1[] =
{
        chOpenAngle, chQuestion, chLatin_x, chLatin_m, chLatin_l
    ,   chSpace, chLatin_v, chLatin_e, chLatin_r, chLatin_s, chLatin_i
    ,   chLatin_o, chLatin_n, chEqual, chDoubleQuote, chNull
};
static const XMLCh  gXMLDecl2[] =
{
        chDoubleQuote, chSpace, chLatin_e, chLatin_n, chLatin_c
    ,   chLatin_o, chLatin_d, chLatin_i, chLatin_n, chLatin_g, chEqual
    ,   chDoubleQuote, chNull
};
static const XMLCh  gXMLDecl3[] =
{
        chDoubleQuote, chSpace, chLatin_s, chLatin_t, chLatin_a
    ,   chLatin_n, chLatin_d, chLatin_a, chLatin_l, chLatin_o
    ,   chLatin_n, chLatin_e, chEqual, chDoubleQuote, chNull
};
static const XMLCh  gXMLDecl4[] =
{
        chDoubleQuote, chQuestion, chCloseAngle
    ,   chCR, chLF, chNull
};

static const XMLCh  gStartCDATA[] =
{ 
        chOpenAngle, chBang, chOpenSquare, chLatin_C, chLatin_D,
        chLatin_A, chLatin_T, chLatin_A, chOpenSquare, chNull
};

static const XMLCh  gEndCDATA[] =
{
    chCloseSquare, chCloseSquare, chCloseAngle, chNull
};
static const XMLCh  gStartComment[] =
{ 
    chOpenAngle, chBang, chDash, chDash, chNull
};

static const XMLCh  gEndComment[] =
{
    chDash, chDash, chCloseAngle, chNull
};

static const XMLCh  gStartDoctype[] =
{ 
    chOpenAngle, chBang, chLatin_D, chLatin_O, chLatin_C, chLatin_T,
    chLatin_Y, chLatin_P, chLatin_E, chSpace, chNull
};
static const XMLCh  gPublic[] =
{ 
    chLatin_P, chLatin_U, chLatin_B, chLatin_L, chLatin_I,
    chLatin_C, chSpace, chDoubleQuote, chNull
};
static const XMLCh  gSystem[] =
{ 
    chLatin_S, chLatin_Y, chLatin_S, chLatin_T, chLatin_E,
    chLatin_M, chSpace, chDoubleQuote, chNull
};
static const XMLCh  gStartEntity[] =
{ 
    chOpenAngle, chBang, chLatin_E, chLatin_N, chLatin_T, chLatin_I,
    chLatin_T, chLatin_Y, chSpace, chNull
};
static const XMLCh  gNotation[] =
{ 
    chLatin_N, chLatin_D, chLatin_A, chLatin_T, chLatin_A,
    chSpace, chDoubleQuote, chNull
};


// ---------------------------------------------------------------------------
//  ostream << DOM_Node   
//
//  Stream out a DOM node, and, recursively, all of its children. This
//  function is the heart of writing a DOM tree out as XML source. Give it
//  a document node and it will do the whole thing.
// ---------------------------------------------------------------------------

unsigned tabPosition = 0;


std::ostream& PrintDoc(std::ostream& target, DOM_Node& toWrite)
{
  
  DOMString encNameStr("UTF-8");
  DOM_Node aNode = toWrite;
  if (aNode.getNodeType() == DOM_Node::XML_DECL_NODE)
  {
    DOMString aStr = ((DOM_XMLDecl &)aNode).getEncoding();
    if (aStr != "")
    {
      encNameStr = aStr;
    }
  }
  unsigned int lent = encNameStr.length();
  gEncodingName = new XMLCh[lent + 1];
  XMLString::copyNString(gEncodingName, encNameStr.rawBuffer(), lent);
  gEncodingName[lent] = 0;
  DOMPrintFormatTarget* formatTarget = new DOMPrintFormatTarget(target);    
  try
  {
    gFormatter = new XMLFormatter(gEncodingName, formatTarget, 
                                  XMLFormatter::NoEscapes, gUnRepFlags);
    target << toWrite << std::endl;
  }
  catch (XMLException& e)
  {
    std::cerr << "An error occurred during creation of output transcoder. Msg is:"
      << std::endl
      << DOMString(e.getMessage()) << std::endl;
  }
  delete formatTarget;
  delete gFormatter;
  delete gEncodingName;

  return target;
}

std::ostream& operator<<(std::ostream& target, DOM_Node& toWrite)
{
  
    // Get the name and value out for convenience
    DOMString   nodeName = toWrite.getNodeName();
    DOMString   nodeValue = toWrite.getNodeValue();
    unsigned long lent = nodeValue.length();

    switch (toWrite.getNodeType())
    {
        case DOM_Node::TEXT_NODE:
        {
            gFormatter->formatBuf(nodeValue.rawBuffer(), 
                                  lent, XMLFormatter::CharEscapes);
            break;
        }


        case DOM_Node::PROCESSING_INSTRUCTION_NODE :
        {
            *gFormatter << XMLFormatter::NoEscapes << gStartPI  << nodeName;
            if (lent > 0)
            {
                *gFormatter << chSpace << nodeValue;
            }
            *gFormatter << XMLFormatter::NoEscapes << gEndPI;
            break;
        }


        case DOM_Node::DOCUMENT_NODE :
        {

            DOM_Node child = toWrite.getFirstChild();
            while( child != 0)
            {
                target << child << std::endl;
                child = child.getNextSibling();
            }
            break;
        }


        case DOM_Node::ELEMENT_NODE :
        {
		TRACEDUMP << std::string(tabPosition++,'\t') << "Element: " << nodeName << std::endl;
            // The name has to be representable without any escapes
            *gFormatter  << XMLFormatter::NoEscapes
                         << chOpenAngle << nodeName;

            // Output the element start tag.

            // Output any attributes on this element
            DOM_NamedNodeMap attributes = toWrite.getAttributes();
            int attrCount = attributes.getLength();
            for (int i = 0; i < attrCount; i++)
            {
                DOM_Node  attribute = attributes.item(i);

                //
                //  Again the name has to be completely representable. But the
                //  attribute can have refs and requires the attribute style
                //  escaping.
                //
                *gFormatter  << XMLFormatter::NoEscapes
                             << chSpace << attribute.getNodeName()
                             << chEqual << chDoubleQuote
                             << XMLFormatter::AttrEscapes
                             << attribute.getNodeValue()
                             << XMLFormatter::NoEscapes
                             << chDoubleQuote;
            }

            //
            //  Test for the presence of children, which includes both
            //  text content and nested elements.
            //
            DOM_Node child = toWrite.getFirstChild();
            if (child != 0)
            {
                // There are children. Close start-tag, and output children.
                // No escapes are legal here
                *gFormatter << XMLFormatter::NoEscapes << chCloseAngle;

                while( child != 0)
                {
                    target << child;
                    child = child.getNextSibling();
                }

                //
                // Done with children.  Output the end tag.
                //
                *gFormatter << XMLFormatter::NoEscapes << gEndElement
                            << nodeName << chCloseAngle;
            }
            else
            {
                //
                //  There were no children. Output the short form close of
                //  the element start tag, making it an empty-element tag.
                //
                *gFormatter << XMLFormatter::NoEscapes << chForwardSlash << chCloseAngle;
            }
		TRACEDUMP << std::string(--tabPosition,'\t') << "tnemelE: " << nodeName << std::endl;
            break;
        }
        
        
        case DOM_Node::ENTITY_REFERENCE_NODE:
            {
                DOM_Node child;
#if 0
                for (child = toWrite.getFirstChild();
                child != 0;
                child = child.getNextSibling())
                {
                    target << child;
                }
#else
                //
                // Instead of printing the refernece tree 
                // we'd output the actual text as it appeared in the xml file.
                // This would be the case when -e option was chosen
                //
                    *gFormatter << XMLFormatter::NoEscapes << chAmpersand
                        << nodeName << chSemiColon;
#endif
                break;
            }
            
            
        case DOM_Node::CDATA_SECTION_NODE:
            {
            *gFormatter << XMLFormatter::NoEscapes << gStartCDATA
                        << nodeValue << gEndCDATA;
            break;
        }

        
        case DOM_Node::COMMENT_NODE:
        {
            *gFormatter << XMLFormatter::NoEscapes << gStartComment
                        << nodeValue << gEndComment;
            break;
        }
        
	
        case DOM_Node::DOCUMENT_TYPE_NODE:
        {
		TRACEDUMP << "Document: " << nodeName << std::endl;
            DOM_DocumentType doctype = (DOM_DocumentType &)toWrite;;

            *gFormatter << XMLFormatter::NoEscapes  << gStartDoctype
                        << nodeName;
 
            DOMString id = doctype.getPublicId();
            if (id != 0)
            {
                *gFormatter << XMLFormatter::NoEscapes << chSpace << gPublic
                    << id << chDoubleQuote;
                id = doctype.getSystemId();
                if (id != 0)
                {
                    *gFormatter << XMLFormatter::NoEscapes << chSpace 
                       << chDoubleQuote << id << chDoubleQuote;
                }
            }
            else
            {
                id = doctype.getSystemId();
                if (id != 0)
                {
                    *gFormatter << XMLFormatter::NoEscapes << chSpace << gSystem
                        << id << chDoubleQuote;
                }
            }
            
            id = doctype.getInternalSubset(); 
            if (id !=0)
                *gFormatter << XMLFormatter::NoEscapes << chOpenSquare
                            << id << chCloseSquare;

            *gFormatter << XMLFormatter::NoEscapes << chCloseAngle;
		TRACEDUMP << "tnemucoD: " << nodeName << std::endl;
            break;
        }
        
        
        case DOM_Node::ENTITY_NODE:
        {
            *gFormatter << XMLFormatter::NoEscapes << gStartEntity
                        << nodeName;

            DOMString id = ((DOM_Entity &)toWrite).getPublicId();
            if (id != 0)
                *gFormatter << XMLFormatter::NoEscapes << gPublic
                            << id << chDoubleQuote;

            id = ((DOM_Entity &)toWrite).getSystemId();
            if (id != 0)
                *gFormatter << XMLFormatter::NoEscapes << gSystem
                            << id << chDoubleQuote;
            
            id = ((DOM_Entity &)toWrite).getNotationName();
            if (id != 0)
                *gFormatter << XMLFormatter::NoEscapes << gNotation
                            << id << chDoubleQuote;

            *gFormatter << XMLFormatter::NoEscapes << chCloseAngle << chCR << chLF;

            break;
        }


        case DOM_Node::XML_DECL_NODE:
        {
            DOMString  str;

            *gFormatter << gXMLDecl1 << ((DOM_XMLDecl &)toWrite).getVersion();

            *gFormatter << gXMLDecl2 << gEncodingName;
            
            str = ((DOM_XMLDecl &)toWrite).getStandalone();
            if (str != 0)
                *gFormatter << gXMLDecl3 << str;
            
            *gFormatter << gXMLDecl4;

            break;
        }
        
        
        default:
            std::cerr << "Unrecognized node type = "
                 << (long)toWrite.getNodeType() << std::endl;
    }
    return target;
}



// ---------------------------------------------------------------------------
//  ostream << DOMString
//
//  Stream out a DOM string. Doing this requires that we first transcode
//  to char * form in the default code page for the system
// ---------------------------------------------------------------------------
std::ostream& operator<< (std::ostream& target, const XMLCh* s)
{
    char *p = XMLString::transcode(s);
    target << p;
    delete [] p;
    return target;
}

std::ostream& operator<< (std::ostream& target, const DOMString& s)
{
    char *p = s.transcode();
    target << p;
    delete [] p;
    return target;
}

XMLFormatter& operator<< (XMLFormatter& strm, const DOMString& s)
{
    unsigned int lent = s.length();

	if (lent <= 0)
		return strm;

    XMLCh*  buf = new XMLCh[lent + 1];
    XMLString::copyNString(buf, s.rawBuffer(), lent);
    buf[lent] = 0;
    strm << buf;
    delete [] buf;
    return strm;
}

static unsigned int indentation=0;

void Inspect (std::ostream& target, DOM_Node& toWrite)
{  
	for (unsigned int i=indentation++; i--; ) target << ". ";
	// Get the name and value out for convenience
	DOMString   nodeName = toWrite.getNodeName();
	DOMString   nodeValue = toWrite.getNodeValue();
	unsigned long lent = nodeValue.length();

	switch (toWrite.getNodeType()) {
		case DOM_Node::TEXT_NODE:
		{
			target << "TEXT:"<<nodeValue<<std::endl;
//        		gFormatter->formatBuf(nodeValue.rawBuffer(), 
//			              lent, XMLFormatter::CharEscapes);
			break;
		}

	        case DOM_Node::PROCESSING_INSTRUCTION_NODE :
		{
			target << "PROCINSTR"<<std::endl;
//			*gFormatter << XMLFormatter::NoEscapes << gStartPI  << nodeName;
//			if (lent > 0) {
//				*gFormatter << chSpace << nodeValue;
//			}
//			*gFormatter << XMLFormatter::NoEscapes << gEndPI;
			break;
		}


        case DOM_Node::DOCUMENT_NODE :
        {
		target << "ROOT"<<std::endl;
            DOM_Node child = toWrite.getFirstChild();
            while( child != 0)
            {
//                target << child << std::endl;
                Inspect(target,child);
                child = child.getNextSibling();
            }
            break;
        }


        case DOM_Node::ELEMENT_NODE :
        {
		target << "ELEMENT:"<<nodeName<<std::endl;
            // The name has to be representable without any escapes
/*            *gFormatter  << XMLFormatter::NoEscapes
                         << chOpenAngle << nodeName;
*/
            // Output the element start tag.

            // Output any attributes on this element
            DOM_NamedNodeMap attributes = toWrite.getAttributes();
            int attrCount = attributes.getLength();
            for (int i = 0; i < attrCount; i++)
            {
                DOM_Node  attribute = attributes.item(i);
		for (unsigned int i=indentation++; i--; ) target << ". ";
		target << "ATTRIBUTE:"<<attribute.getNodeName()<<"="<<attribute.getNodeValue()<< std::endl;

                //
                //  Again the name has to be completely representable. But the
                //  attribute can have refs and requires the attribute style
                //  escaping.
                //
/*                *gFormatter  << XMLFormatter::NoEscapes
                             << chSpace << attribute.getNodeName()
                             << chEqual << chDoubleQuote
                             << XMLFormatter::AttrEscapes
                             << attribute.getNodeValue()
                             << XMLFormatter::NoEscapes
                             << chDoubleQuote;
			     
  */        indentation--;  
	    }

            //
            //  Test for the presence of children, which includes both
            //  text content and nested elements.
            //
            DOM_Node child = toWrite.getFirstChild();
            if (child != 0)
            {
                // There are children. Close start-tag, and output children.
                // No escapes are legal here
//                *gFormatter << XMLFormatter::NoEscapes << chCloseAngle;

                while( child != 0)
                {
//                    target << child;
                      Inspect(target, child);

                    child = child.getNextSibling();
                }

                //
                // Done with children.  Output the end tag.
                //
//                *gFormatter << XMLFormatter::NoEscapes << gEndElement
//                            << nodeName << chCloseAngle;
            }
            else
            {
                //
                //  There were no children. Output the short form close of
                //  the element start tag, making it an empty-element tag.
                //
//                *gFormatter << XMLFormatter::NoEscapes << chForwardSlash << chCloseAngle;
            }
            break;
        }
        
        
        case DOM_Node::ENTITY_REFERENCE_NODE:
            {
		target << "ENTITY"<<std::endl;
                DOM_Node child;
#if 0
                for (child = toWrite.getFirstChild();
                child != 0;
                child = child.getNextSibling())
                {
                    target << child;
                }
#else
                //
                // Instead of printing the refernece tree 
                // we'd output the actual text as it appeared in the xml file.
                // This would be the case when -e option was chosen
                //
//                    *gFormatter << XMLFormatter::NoEscapes << chAmpersand
//                        << nodeName << chSemiColon;
#endif
                break;
            }
            
            
        case DOM_Node::CDATA_SECTION_NODE:
            {
		    target << "CDATA"<<std::endl;
//            *gFormatter << XMLFormatter::NoEscapes << gStartCDATA
//                        << nodeValue << gEndCDATA;
            break;
        }

        
        case DOM_Node::COMMENT_NODE:
        {
		target << "COMMENT"<<std::endl;
//            *gFormatter << XMLFormatter::NoEscapes << gStartComment
//                        << nodeValue << gEndComment;
            break;
        }
        
	
        case DOM_Node::DOCUMENT_TYPE_NODE:
        {
		target << "DOCUMENT TYPE"<<std::endl;
            DOM_DocumentType doctype = (DOM_DocumentType &)toWrite;;

//            *gFormatter << XMLFormatter::NoEscapes  << gStartDoctype
//                        << nodeName;
 
            DOMString id = doctype.getPublicId();
            if (id != 0)
            {
  //              *gFormatter << XMLFormatter::NoEscapes << chSpace << gPublic
//                    << id << chDoubleQuote;
                id = doctype.getSystemId();
                if (id != 0)
                {
//                    *gFormatter << XMLFormatter::NoEscapes << chSpace 
//                       << chDoubleQuote << id << chDoubleQuote;
                }
            }
            else
            {
                id = doctype.getSystemId();
                if (id != 0)
                {
//                    *gFormatter << XMLFormatter::NoEscapes << chSpace << gSystem
//                        << id << chDoubleQuote;
                }
            }
            
            id = doctype.getInternalSubset(); 
            if (id !=0)
//                *gFormatter << XMLFormatter::NoEscapes << chOpenSquare
//                            << id << chCloseSquare;

//            *gFormatter << XMLFormatter::NoEscapes << chCloseAngle;
            break;
        }
        
        
        case DOM_Node::ENTITY_NODE:
        {
		target << "ENTITYNODE"<<std::endl;
		/*
            *gFormatter << XMLFormatter::NoEscapes << gStartEntity
                        << nodeName;

            DOMString id = ((DOM_Entity &)toWrite).getPublicId();
            if (id != 0)
                *gFormatter << XMLFormatter::NoEscapes << gPublic
                            << id << chDoubleQuote;

            id = ((DOM_Entity &)toWrite).getSystemId();
            if (id != 0)
                *gFormatter << XMLFormatter::NoEscapes << gSystem
                            << id << chDoubleQuote;
            
            id = ((DOM_Entity &)toWrite).getNotationName();
            if (id != 0)
                *gFormatter << XMLFormatter::NoEscapes << gNotation
                            << id << chDoubleQuote;

            *gFormatter << XMLFormatter::NoEscapes << chCloseAngle << chCR << chLF;
*/
            break;
        }


        case DOM_Node::XML_DECL_NODE:
        {
		target << "DECLNODE"<<std::endl;
		/*
            DOMString  str;

            *gFormatter << gXMLDecl1 << ((DOM_XMLDecl &)toWrite).getVersion();

            *gFormatter << gXMLDecl2 << gEncodingName;
            
            str = ((DOM_XMLDecl &)toWrite).getStandalone();
            if (str != 0)
                *gFormatter << gXMLDecl3 << str;
            
            *gFormatter << gXMLDecl4;
*/
            break;
        }
        
        
        default:
            std::cerr << "Unrecognized node type = "
                 << (long)toWrite.getNodeType() << std::endl;
    }
	indentation--;
}




