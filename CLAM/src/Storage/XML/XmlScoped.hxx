/*
 * Copyright (c) 2004 MUSIC TECHNOLOGY GROUP (MTG)
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

#ifndef _XmlScoped_hxx_
#define _XmlScoped_hxx_

#include <iosfwd>
#include "XmlWriteContext.hxx"

namespace CLAM
{
/**
 * @defgroup XmlScoped Scoped XML: A quick way to write XML (write only)
 * @ingroup Xml
 * @brief This module contains classes to write an XML document
 * following a "open on create - close on destroy" idiom.
 *
 * That means that by simply creating an element object
 * you are opening a tag and
 * when the element object goes out of scope the element
 * is automatically closed.
 * The C++ stack frame managing rules assure that the XML
 * will be well balanced.
 *
 * This system only provides writing XML and
 * has no mapping to CLAM data as CLAM::XmlStorage has.
 * See @link XmlDump the XmlStorage way @endlink.
 * 
 * Two  aproaches can be convined to generate XML with
 * scoped objects:
 * -# Using function calls: Define a function call that
 * will write an element and will call other functions
 * to write children elements.
 * -# Using local scopes: Local scopes allows to define
 * an stack frame without calling a new function.
 * So you can write code like that:
 * @code
 * XmlFragment(std::cout);
 * { XmlElement element("Configuration");
 * 	{ XmlElement element("Parameter");
 * 		XmlAttribute attrib("name","SpectralHeight");
 * 		XmlContent content(1276.3);
 * 	}
 * 	{ XmlElement element("Parameter");
 * 		XmlAttribute attrib("name","Filename");
 * 		XmlContent content("Voice.wav");
 * 	}
 * }
 * @endcode
 * 
 * The first scoped object must be a top level one like
 * XmlFragment (XmlDocument will be available soon).
 * That element defines the target ostream where the XML will
 * be written. 
 *
 * @note Use ostringstream if you want to write XML to a string.
 * 
 * @remark You can build as many XmlFragments and XmlDocuments
 * as you want along your program but never having two of them
 * instanciated at the same time, since scoped objects find the
 * current TopLevel scoped object for the current XML writting context.
 * Be also careful on different threads writting XML scopedly.
 * 
 * @warning Be sure to create local variables and not temporary
 * elements, since the following bad-friend code:
 * @code
 * // Wrong code
 * XmlElement("MyTag");
 * {
 * 	XmlElement("Child");
 * 	XmlContent("hello");
 * }
 * @endcode
 * will close the tag inmediatelly writting 
 * 	<tt>&lt;MyTag /&gt;&lt;Child /&gt;hello</tt>
 * instead the probably intended:
 * 	<tt>&lt;MyTag&gt;&lt;Child&gt;hello&lt;/Child&gt;&lt;/MyTag&gt;</tt>
 * the correct code should be:
 * @code
 * XmlElement element("MyTag");
 * {
 * 	XmlElement element("Child");
 * 	XmlContent content("hello");
 * }
 * @endcode
 * 
 * @warning Those classes are useful when are local stack variables.
 * If you create instances with new you loose the autobalancing feature.
 *
 * @bug Special chars are not escaped in attributes and plain content.
 */

/**
 * This is the first scoped object you should create for a XML fragment.
 * It defines and controls how the XML fragment will be generated.
 * While this object exist all the scoped XML elements will be part of
 * this fragment.
 * Don't create two fragments on the same scope or in different 
 * concurrent threads or they will collide.
 * @ingroup XmlScoped
 */

class XmlFragment
{
	public:
		XmlFragment(std::ostream & os) :
			context(os)
		{
		}
	private:
		XmlWriteContext context;
};

/**
 * An scoped XML writer object inserting plain content to the 
 * current XML level.
 * Since the parameter on the constructor is templatized,
 * you can use as content anything with the insertion operator on 
 * ostream defined.
 * @ingroup XmlScoped
 */
class XmlContent
{
	public:
		/**
		 * Inserts on construction plain content to the
		 * current XML writing context.
		 * @param content An ostream insertable object that will
		 * compose the attribute content.
		 * @bug Special chars are not escaped
		 */
		template <typename T>
		XmlContent(const T & content) :
			context(XmlWriteContext::CurrentContext())
		{
			context.InsertContent(content);
		}

	private:

		XmlWriteContext & context;
		
};

/**
 * An scoped XML writer object that opens a new element on 
 * construction and closes it on destruction.
 * @ingroup XmlScoped
 */
class XmlElement
{
	public:
		/**
		 * Opens on construction an XML element on the
		 * current XML writting context.
		 * @pre the name is a valid qualified name
		 * @bug Special chars are not escaped
		 */
		XmlElement(const std::string & name) :
			context(XmlWriteContext::CurrentContext()),
			mName(name)
		{
			context.OpenElement(mName);
		}
		/**
		 * Closes by destruction the element opened by the
		 * constructor.
		 * @pre all elements created inbetween have been
		 * created on the stack so the LIFO order is mantained.
		 */
		~XmlElement()
		{
			context.CloseElement(mName);
		}

	private:
		XmlWriteContext & context;
		const std::string mName;
		
};

#ifdef NEVER_DEFINED
class XmlHeader
{	public:
		/**
		 * Opens on construction an XML element on the
		 * current XML writting context.
		 * @pre the name is a valid qualified name
		 * @bug Special chars are not escaped
		 */
		XmlHeader(const std::string & name) :
			context(XmlWriteContext::CurrentContext())
		{
			context.InsertHeader(name);
		}
	private:
		XmlWriteContext & context;

};
#endif//NEVER_DEFINED

/**
 * An scoped XML writer object that inserts on construction
 * an XML atribute to the last open XML element.
 * Since the second parameter on the constructor is templatized,
 * you can use as content anything with the insertion operator on 
 * ostream defined.
 * @ingroup XmlScoped
 */
class XmlAttribute
{
	public:
		/**
		 * Inserts on construction an attribute to the last
		 * open XML element.
		 * @param name The name of the attribute
		 * @param content An ostream insertable object that will
		 * compose the attribute content.
		 * @pre the name is a valid qualified name
		 * @pre There is an XML element open without any non 
		 * attribute scoped element writen yet.
		 * @bug Special chars are not escaped
		 */
		template <typename T>
		XmlAttribute(const std::string & name, const T & content) :
			context(XmlWriteContext::CurrentContext())
		{
			context.InsertAttribute(name,content);
		}

	private:

		XmlWriteContext & context;
		
};

} // ns CLAM

#endif//_XmlScoped_hxx_

