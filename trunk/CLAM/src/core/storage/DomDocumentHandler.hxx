/*
 * Copyright (c) 2001-2004 MUSIC TECHNOLOGY GROUP (MTG)
 *                         UNIVERSITAT POMPEU FABRA
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */
#ifndef _DomDocumentHandler_hxx_
#define _DomDocumentHandler_hxx_

/**
 * @class CLAM::DomDocumentHandler
 * @ingroup XmlBackends
 * @brief This class subclasses from the proper DomDocumentHandler depending
 * on the build time configuration 'xmlbackend' option.
 *
 * An XML implementation is choosen depending on the USE_XXXXXX
 * directives concerning XML libraries.
 * The current USE_XXXXXX variables that affect this class in priority order are:
 * - USE_XERCES: Chooses XercesDomDocumentHandler (Apache's Xerces-C++)
 * - USE_XMLPP:  Chooses LibXmlDomDocumentHandler (Gnome's libxml++)
 *
 * If you choose 'none' as backend, then a Null implementation will 
 * be used that fails an assertion whenever you try to use the XML
 * serialization.
 *
 * @see XercesDomDocumentHandler
 * @see LibXmlDomDocumentHandler
 * @see NullDomDocumentHandler
 * @author David Garcia.
 */

#if defined USE_XERCES
#include "XercesDomDocumentHandler.hxx"
namespace CLAM 
{
	class DomDocumentHandler : public XercesDomDocumentHandler {};
}
#elif defined USE_XMLPP
#include "LibXmlDomDocumentHandler.hxx"
namespace CLAM 
{
	class DomDocumentHandler : public LibXmlDomDocumentHandler {};
}

#elif !defined CLAM_USE_XML
#include "NullDomDocumentHandler.hxx"
namespace CLAM 
{
	class DomDocumentHandler : public NullDomDocumentHandler {};
}
#else
#error CLAM_USE_XML active but no backend available (libxml++, xerces...)
#endif//USE_XMLPP

/**
 * @defgroup XmlBackends XML Implementation Backends
 * @ingroup Xml
 * @brief This documentation module explains the XML implementation backends.
 *
 * CLAM supports different XML libraries as backend for the XML support.
 * This can be done by implementing the DomDocumentHandler class and
 * its nested classes, DomDocumentWritingContext and DomDocumentReadingContext.
 *
 * A DocumentHandler provides the abstraction that CLAM::XmlStorage
 * uses to perform its tasks:
 * DOM construction and exploration, and file reading and writing.
 *
 * Two inner classes (or typedefs) named DomDocumentReadingContext and
 * DomDocumentWritingContext, must be defined.
 * Those inner classes keeps the context while mapping for reading and writting
 * CLAM::Objects to a given XML tree element.
 * 
 * Usually a XXXReader and a XXXWriter are implemented in order to build 
 * a DOM tree from an XML file and to write it given the DOM Document.
 * The Document Handler also keeps the selected node in order to do partial operations
 * like adding object on a given tree position or dumping a document fragment.
 * 
 * Currently, the following implementations are available:
 * - CLAM::NullDomDocumentHandler: 
 *   A dummy one, that just assert false when it is instanciated,
 *   just in case XML is used and it is disabled.
 * - CLAM::XercesDomDocumentHandler:
 *   Based on the Xerces-C++ library
 * - CLAM::LibXmlDomDocumentHandler:
 *   Based on the libxml++ library (Experimental)
 * @author David Garcia.
 */


#endif//_XercesDomDocumentHandler_hxx_

