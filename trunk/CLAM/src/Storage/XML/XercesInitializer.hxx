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
#ifndef _XercesInitializer_hxx_
#define _XercesInitializer_hxx_
#include <xercesc/util/PlatformUtils.hpp>
//#include <iostream>
namespace CLAM
{

/**
 * Ensures that Xerces-C library initialization and termination
 * occurs once and only when needed.
 * @ingroup XmlBackends_Xerces
 *
 * At any time you need the xerces library (this is handled by
 * the @link XmlBackends_Xerces Xerces-C XML Backend @endlink,
 * so you won't normally need to), you should call:
 * @code
 * ::CLAM::XercesInitializer::require();
 * @endcode
 * Calling that multiple times is safe and painless.
 *
 * Only if you called the previous static method at any time in
 * your program,
 * the Xerces termination will be automagically performed
 * when the program ends.
 * @author David Garcia
 */
class XercesInitializer
{
	XercesInitializer()
	{
//		std::cout << "Initializing xerces..." << std::endl;
		xercesc::XMLPlatformUtils::Initialize();
	}
public:
	~XercesInitializer()
	{
		xercesc::XMLPlatformUtils::Terminate();
//		std::cout << "Terminating xerces..." << std::endl;
	}
	static const XercesInitializer & require()
	{
		static XercesInitializer initializer;
		return initializer;
	}
};

}

#endif//_XercesInitializer_hxx_

