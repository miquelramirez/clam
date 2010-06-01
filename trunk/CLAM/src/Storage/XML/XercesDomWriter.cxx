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
#include <xercesc/dom/DOMLSSerializer.hpp>
#include <xercesc/dom/DOMLSOutput.hpp>
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
		xercesc::DOMImplementationLS *impl = 
			(xercesc::DOMImplementationLS*)
			xercesc::DOMImplementationRegistry::getDOMImplementation(U("LS"));

		xercesc::DOMLSSerializer *serializer = impl->createLSSerializer();
		xercesc::DOMLSOutput     *output     = impl->createLSOutput();

		output->setEncoding(U("UTF-8"));

		xercesc::DOMConfiguration* serializerConfig = serializer->getDomConfig();

		if (serializerConfig->canSetParameter(propertyPrettyPrint, mShouldIndent))
			serializerConfig->setParameter(propertyPrettyPrint, mShouldIndent);
		if (serializerConfig->canSetParameter(propertyCanonical, mShouldCanonicalize))
			serializerConfig->setParameter(propertyCanonical, mShouldCanonicalize);

		xercesc::MemBufFormatTarget formatTarget;
		output->setByteStream(&formatTarget);
		serializer->write(node, output);

		const char * buffer = (char *) formatTarget.getRawBuffer();
		const unsigned bufferLen = formatTarget.getLen();
		target << std::string(buffer,bufferLen);
		serializer->release();
		output->release();
	}

}

