/*
 * Copyright (c) 2001-2002 MUSIC TECHNOLOGY GROUP (MTG)
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
#ifndef _XERCES_DOM_WRITER_HXX_
#define _XERCES_DOM_WRITER_HXX_

#include "XercesEncodings.hxx"
#include "XercesInitializer.hxx"

#include <xercesc/dom/DOMImplementation.hpp>
#include <xercesc/dom/DOMImplementationLS.hpp>
#include <xercesc/dom/DOMImplementationRegistry.hpp>
#include <xercesc/dom/DOMWriter.hpp>
#include <xercesc/framework/MemBufFormatTarget.hpp>
#include <string>


namespace xercesc = XERCES_CPP_NAMESPACE;

namespace CLAM
{

class XercesDomWriter
{
	public:
		XercesDomWriter()
		{
			mShouldIndent=false;
			mShouldCanonicalize=false;
		}
		void write(std::ostream & target, xercesc::DOMNode * node)
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
		void DoCanonicalFormat(bool shouldDo = true)
		{
			mShouldCanonicalize=shouldDo;
		}
		void DoIndentedFormat(bool shouldDo = true)
		{
			mShouldIndent=shouldDo;
		}
	private:
		bool mShouldIndent;
		bool mShouldCanonicalize;
};



}
#endif//_XERCES_DOM_WRITER_HXX_

