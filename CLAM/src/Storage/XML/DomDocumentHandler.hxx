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

#ifdef CLAM_USE_XML
/**
 * @class CLAM::DomDocumentHandler
 * This class subclasses from the proper DomDocumentHandler depending
 * on the configuration.
 * If you have defined CLAM_USE_XML in your settings.cfg, then a
 * XercesC++ based implementation will be used.
 * If not, then a null implementation will be used, that it assert
 * false whenever you try to use the DOM based XML support in CLAM.
 * @see XercesDomDocumentHandler
 * @see NullDomDocumentHandler
 */

#include "XercesDomDocumentHandler.hxx"
namespace CLAM 
{
	class DomDocumentHandler : public XercesDomDocumentHandler {};
}

#else

#include "NullDomDocumentHandler.hxx"
namespace CLAM 
{
	class DomDocumentHandler : public NullDomDocumentHandler {};
}


#endif//CLAM_USE_XML

#endif//_XercesDomDocumentHandler_hxx_

