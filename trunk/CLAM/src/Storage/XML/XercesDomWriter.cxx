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

#include "XercesDomWriter.hxx"
#include "XercesEncodings.hxx"
#include "XercesInitializer.hxx"

#include <xercesc/dom/DOMImplementation.hpp>
#include <xercesc/dom/DOMImplementationLS.hpp>
#include <xercesc/dom/DOMImplementationRegistry.hpp>
#include <xercesc/dom/DOMWriter.hpp>
#include <xercesc/framework/MemBufFormatTarget.hpp>
#include <string>
#include <sstream>

namespace CLAM
{
	void XercesDomWriter::write(std::ostream & target, xercesc::DOMNode * node)
	{
		XercesInitializer::require();
		const XMLCh * propertyCanonical = xercesc::XMLUni::fgDOMWRTCanonicalForm;
		const XMLCh * propertyPrettyPrint = xercesc::XMLUni::fgDOMWRTFormatPrettyPrint;
		xercesc::DOMImplementation *impl = 
			xercesc::DOMImplementationRegistry::getDOMImplementation(X("LS"));
		xercesc::DOMWriter *xercesWriter = 
			((xercesc::DOMImplementationLS*)impl)->createDOMWriter();

		if (xercesWriter->canSetFeature(propertyPrettyPrint, mShouldIndent))
			xercesWriter->setFeature(propertyPrettyPrint, mShouldIndent);
		if (xercesWriter->canSetFeature(propertyCanonical, mShouldCanonicalize))
			xercesWriter->setFeature(propertyCanonical, mShouldCanonicalize);

		xercesc::MemBufFormatTarget * xercesTarget = new xercesc::MemBufFormatTarget();
		xercesWriter->writeNode(xercesTarget, *node);
		const char * buffer = (char *) xercesTarget->getRawBuffer();
		const unsigned bufferLen = xercesTarget->getLen();
		target << std::string(buffer,bufferLen);
		delete xercesWriter;
		delete xercesTarget;
	}

}

