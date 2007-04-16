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

//////////////////////////////////////////////////////////////////////
// XMLStorage.cxx
// Description: A storage in XML format for CLAM objects
//////////////////////////////////////////////////////////////////////

#ifndef _XMLStorage_hxx_
#define _XMLStorage_hxx_

#include "Storage.hxx"
#include "XmlStorageErr.hxx"
#include <iosfwd>
#include <string>


namespace CLAM
{

class XMLable;
class Component;
class DomDocumentHandler;


class XmlStorage : public Storage
{
	DomDocumentHandler * _documentHandler;
	bool _lastWasContent;
	bool _useIndentation;
	std::string _errors;
/**
 * @name Atomic primitives to Dump and Restore
 * To be used only when you need to twickle the default behavior for summary operations
 * (Dump, Restore...).
 */
// @{
public:
	XmlStorage();
	~XmlStorage();

public:
	void Read(std::istream & is); ///< Loads the DOM from the stream and selects the root node
	void Create(const std::string & name); ///< Creates a root node and selects it
	void WriteSelection(std::ostream & os); ///< Dumps the selected node on the stream
	void WriteDocument(std::ostream & os); ///< Dumps the full document on the stream
	void DumpObject(const Component & component); ///< Holds the component DOM to the selected node
	void RestoreObject(Component & component); ///< Restores the component from the selected node
	/**
	 * Moves the node selection to the specified path.
	 * @throws XmlStorageErr whenever a step is not found
	 * The path is a subset of XPath.
	 * Absolute and relative paths can be used but node
	 * names are the only step specifier to be used.
	 *
	 * @code
	 * /Root/Element/SubElement
	 * Element/SubElement     // Relative being in /Root, the default)
	 * /                      // Equivalent to /Root but useful when 
	 *                        // you don't know the root node name.
	 * @endcode
	 */
	void Select(const std::string & path); ///< Sets the selection at the specified path (the default selection is the root node)
	/**
	 * Changes whether to output pretty formated XML or not.
	 * By default, indentation and new lines are not inserted resulting
	 * in compact XML but dificult for humans to read. Use this method,
	 * to enable the indentation.
	 * @todo Not implemented yet
	 */
	void UseIndentation(bool useIndentation);
// @}

/**
 * @name Summary operations to Dump and Restore
 * Those operations are the simplest ones to use to dump and restore
 * your objects in XML. @link XmlDump XmlDump Module @endlink
 */
// @{
public:

	/**
	 * Dumps a Component as XML onto the named file with name as the root element.
	 */
	static void Dump(const Component & obj, const std::string & rootName, std::ostream & os, bool indentate=true)
	{
		XmlStorage storage;
		storage.Create(rootName);
		storage.DumpObject(obj);
		storage.UseIndentation(indentate);
		storage.WriteDocument(os);
	}

	/**
	 * Restore a Component from the given istream.
	 */
	static void Restore(Component & obj, std::istream & is)
	{
		XmlStorage storage;
		storage.Read(is);
		storage.RestoreObject(obj);
	}

	/**
	 * Restore a Component from the xml fragment on the given xpath of the given document.
	 */
	static void RestoreFromFragment(Component & obj, const std::string & path, std::istream & is)
	{
		XmlStorage storage;
		storage.Read(is);
		storage.Select(path);
		storage.RestoreObject(obj);
	}

	/**
	 * Append the xml fragment corresponding to the given component
	 * on the given xpath of an existing file.
	 */
	static void AppendToDocument(const Component & obj, const std::string & xpath, const std::string & filename);

	/**
	 * Dump a Component from the named XML file.
	 */
	static void Dump(const Component & obj, const std::string & rootName, const std::string & filename);

	/**
	 * Restore a Component from the named XML file.
	 */
	static void Restore(Component & obj, const std::string & filename);
// @}

/**
 * @name Interface for Components to load/store their subitems
 * This is the interface to be used by the CLAM::Component::StoreOn 
 * and CLAM::Component::LoadFrom implementers.
 */
// @{
public:
	/**
	 * Components should use that function in their LoadFrom in order to store
	 * their subitems wrapped with XML*Adapters.
	 */
	void Store(const Storable & storable);
	/**
	 * Components should use that function in their LoadFrom in order to load
	 * their subitems wrapped with XML*Adapters.
	 */
	bool Load(Storable & storable);
// @}

// Private helper functions
private:
	bool LoadContentAndChildren(XMLable* xmlable);
	void StoreContentAndChildren(const XMLable * xmlable);
	void StoreChildrenIfComponent(const XMLable * xmlable);
	void AddContentToElement(const std::string & content);

};

/** For maintaining compatibility with deprecated class name  */
typedef XmlStorage XMLStorage;

} // namespace CLAM



/**
 @defgroup XmlDump Dumping and Restoring objects on XML
 @ingroup Xml
 @brief How to dump or restore a CLAM::Component in XML format.

 Any class that derives from Component can be dumped and
 restored in XML (http://www.w3.org/XML) format.
 Components includes Arrays, DynamicType, ProcessingConfig,
 ProcessingData...

 @warning XML storage is a very expensive task when dealing
 	with big amounts of numeric data.

 
 @section XMLStaticInterface Dumping and restoring a component in XML
 
 In its simplest usage, you can use the static functions
 Dump and Restore.
 @code
 MyComponent myComponent;

 // Here you can modify your Component

 CLAM::XmlStorage::Dump(myComponent, "Document", "MyComponent.xml")
 @endcode

 Later you can restore the component:
 @code
 // An unmodified default constructed object!!!
 MyComponent comp;
 try
 {
	CLAM::XmlStorage::Restore(comp, "mycomponent.xml");
 }
 catch (CLAM::XmlStorageErr & err)
 {
 	// Handle the error, a dirty way could be:
 	std::cerr << err.what() << std::endl;
 	exit(-1);
 }
 @endcode

 Dump and Restore are overloaded to accept any C++ stream instead of a filename.
 So, you can also write: 
 @code
 MyComponent myComponent;
 CLAM::XmlStorage::Dump(myComponent, "Document", std::cout)
 @endcode

 You can the AppendToDocument static function to add an object in a given
 path of an existing XML file,
 or using RestoreFromFragment to restore the object taking an XML fragment.

 Catchable exceptions (CLAM::XmlStorageErr) are thrown on the following conditions:
 - The source/target stream could not be open
 - The read source contained illegal XML
 - The XML is valid but it failed to be mapped to the CLAM objects.
 - A step on the path to change the selection couldn't be followed
 - A loaded CLAM object failed its own activation constraints.

 @author David Garcia.
 */

/**
 * @class CLAM::XmlStorage
 * @brief Provides XML format storage for CLAM Component's.
 * @ingroup Xml
 * @ingroup XmlDump
 * @ingroup XmlCustomize
 * Any class that derives from Component can be dumped and
 * restored in XML (http://www.w3.org/XML) format.
 * Components includes Arrays, DynamicType, ProcessingConfig,
 * ProcessingData...
 *
 * @warning XML storage is a very expensive task when dealing
 * 	with big amounts of numeric data.
 *
 * 
 * @section XMLStaticInterface Dumping and restoring a component in XML
 * 
 * In its simplest usage, you can use the static functions
 * Dump and Restore.
 * @code
 * MyComponent myComponent;
 *
 * // Here you can modify your Component
 *
 * CLAM::XmlStorage::Dump(myComponent, "Document", "MyComponent.xml")
 * @endcode
 *
 * Later you can restore the component:
 * @code
 * // An unmodified default constructed object!!!
 * MyComponent comp;
 * try
 * {
 * 	CLAM::XmlStorage::Restore(comp, "mycomponent.xml");
 * }
 * catch (CLAM::XmlStorageErr & err)
 * {
 * 	// Handle the error, a dirty way could be:
 * 	std::cerr << err.what() << std::endl;
 * 	exit(-1);
 * }
 * @endcode
 *
 * Dump and Restore are overloaded to accept any C++ stream instead of a filename.
 *
 * You can the AppendToDocument static function to add an object in a given
 * path of an existing XML file,
 * or using RestoreFromFragment to restore the object taking an XML fragment.
 *
 * 
 * @section XmlStepByStepInterface Step-By-Step Interface
 *
 * The XMLStorage static methods used above provide shortcuts for the widely used funcionalities.
 * The non-static methods that XMLStorage provides are the building
 * blocks for implementing the static ones:
 * - XmlStorage::Create: Creates an Empty DOM document.
 * - XmlStorage::Read: Creates a DOM document from the XML that comps from an istream.
 * - XmlStorage::WriteDocument: Writes on a stream the whole document
 * - XmlStorage::Select: Changes the selected node (by default the root is selected)
 * - XmlStorage::WriteSelection: Writes on a stream the selected target
 * - XmlStorage::DumpObject: Dumps the CLAM object on the selected DOM node
 * - XmlStorage::RestoreObject: Restores the CLAM object from the selected DOM node
 *
 * You can convining them to achieve a diferent behaviour.
 *
 * - storing optimally the same object it onto two different streams,
 *   @code
 *   CLAM::XmlStorage storage;
 *   storage.Create("Document");
 *   storage.DumpObject(component);
 *   // Writting it to a file
 *   std::ofstream os("Document.xml");
 *   storage.WriteDocument(os);
 *   // Writting it to the standard output
 *   storage.WriteDocument(std::cout);
 *     @endcode
 * - updating an existing xml by adding some objects to it,
 * - extracting an object from a part of a document,
 * - writing a document fragment
 * - ...
 *   
 * Static methods are not enough,
 * but you still can instantiate an XMLStorage object
 * and use the non-static methods with it.
 * Non-static methods implements smaller steps than static methods do
 * and you can combine them in order to obtain some concrete behaviour. 
 *    For example, if you want to update an xml document by adding an object on XPath /Doc/element/subElement, you can use the sequence Read-Select-DumpObject-WriteDocument.
 * 
 * @author David Garcia.
 */

/**
 * @defgroup Xml XML
 * @brief This page is a simple road map for XML related documentation in CLAM.
 *
 * - If you are using existing CLAM Objects or your own CLAM::DynamicType
 *   and you simply want to dump or restore them using XML then you should 
 *   check the CLAM::XmlStorage class documentation.
 *
 * - If you want to customize the default XML format that CLAM::DynamicType
 *   offers or you want to provide XML support for a CLAM::Component that
 *   is not a CLAM::DynamicType,
 *   then you should check the @link XmlCustomize XML Customization @endlink module.
 *
 * - If you only need to output XML quickly and you don't want to 
 *   define an object to be dumped, see @link XmlScoped XmlScoped @endlink.
 *   This method works only for writing.
 *
 * - If you want to understand how CLAM choses different XML libraries
 *   backends (Xerces-C, libxml++, no support...),
 *   see @link XmlBackends Xml Implementation Backends @endlink.
 * 
 * @author David Garcia.
 */

/**
 * @defgroup XmlCustomize Customizing the XML format for your object
 * @ingroup Xml
 *
 * @brief This documentation module explains how to customize the XML
 *        format that corresponds to your object.
 *
 * @section XMLDynamicTypeDefaults The defaults for dynamic types
 *
 * CLAM::DynamicTypes have automatic support for XML without
 * any extra effort than defining the DynamicType itself.
 * So, before attempting to generate your own format
 * it is important to know whether you need to.
 * Any DynamicType has a default XML implementation.
 * So any CLAM::ProcessingData and any CLAM::ProcessingConfig will
 * have such a default implementation.
 *
 * By default, every dynamic attribute that is XML aware
 * is dumped in sequence order, within an XML element
 * that takes the attribute name as tag name.
 * Because dynamic attributes may be instantiated or not, 
 * removed attributes are ignored on storing and, 
 * on loading, not present attributes are 'Removed' from the dinamic type.
 *
 * So, which attributes are XML aware?
 *
 * - CLAM::Components, which are stored recursively.
 * - C++ Primitive types, and some CLAM classes, 
 *   which are stored using stream insertion/extraction operators (see below).
 * - STL like components 
 *
 * Basic objects are C primitive types and some others
 * (std::string, CLAM::Complex<TData>, CLAM::Polar<TData>, CLAM::Point<TData>...)
 * they use their extraction (>>) and insertion (<<) operator to generate plain content.
 * You can define your own basic types.
 *
 * @subsection XmlStrings Some words about strings attributes
 *
 * Never use char* as attributes if you want to load it.
 * Use some other alternative:
 * - Does it Represent a file name? Yes -> Use CLAM::Filename
 * - Does it allow spaces within? Yes -> Use CLAM::Text
 * - Has it a limited set of single-word values? Yes -> Use a CLAM::Enum subclass
 * - Is it single-word but free text? Yes -> Use std::string
 *
 * All of those alternatives can be used as 'basic types'.
 *
 * @subsection XmlStlContainers STL like Containers
 *
 * STL compliant containers have XML support if they are declared as DYN_CONTAINER_ATTRIBUTE.
 *
 *
 * When the contained class is a component,
 * then each of the contained objects are stored as elements inside the container element.
 * So:
 * @code
 * DYN_CONTAINER_ATTRIBUTE(1, public, std::list<MyComponent>, ComponentList, AComponent);
 * @endcode
 * will look like
 * @code
 * <ComponentList size='4'>
 *    <AComponent> ... </AComponent>
 *    <AComponent> ... </AComponent>
 *    <AComponent> ... </AComponent>
 *    <AComponent> ... </AComponent>
 * </ComponentList>
 * @endcode
 *
 * When the contained class is a basic type,
 * all the container items will be stored in a single XML element separated by spaces.
 * @code
 * DYN_CONTAINER_ATTRIBUTE(1, public, std::vector<double>, LeafList, Ignored);
 * @endcode
 * will look like
 * @code
 * <LeafList size='256'>342.243 2342.252 .... 0.234 0 0</LeafList>
 * @endcode
 * Note that in this case the last macro parameter is ignored.
 * 
 * This is what you can get by default from a DynamicType.
 * If you don't like it keep reading this documentation module.
 *
 * @section XmlOwnBasicTypes Defining your own basic types
 *
 * You can define any class, for example MyBasicType, to be used in XML as a basic type doing the following: 
 * - defining their extraction (>>) and insertion (<<) operators over std::streams 
 * - using the following macro call at toplevel namespace: 
 *   @code
 *   CLAM_TYPEINFOGROUP(CLAM::BasicCTypeInfo, MyBascType);
 *   @endcode
 * 
 * @section XmlCustomizationBasics Customization Basics
 *
 * Let see a sample Dynamic Type class:
 * @code
 * class ConcreteDT : public CLAM::DynamicType
 * {
 * public:
 * 	DYNAMIC_TYPE(ConcreteDT, 5);
 * 	DYN_ATTRIBUTE      (0, public, DummyComponent, MyComponent);
 * 	DYN_ATTRIBUTE      (1, public, CLAM::Array<Complex>, MyArray);
 * 	DYN_ATTRIBUTE      (2, public, FooDTClass, MyDynType);
 * 	DYN_CONTAINER_ATTRIBUTE(3, public, std::list<int>, MyList);
 * 	DYN_ATTRIBUTE      (4, public, int, MyInt);
 * public:
 * 	virtual ~ConcreteDT() {}
 * protected:
 * 	void DefaultInit()
 * 	{
 * 		AddAll();
 * 		UpdateData();
 * 	}
 * // Some non dynamic attributes
 * private:
 * 	FooComponent mExtraNonDynamicAttribute;
 * };
 * @endcode
 * This Dynamic Type, as is, will generate default XML. In order to customize it we have to redefine two storage related methods:
 * @code
 * void MyDyn::StoreOn(Storage & s);
 * void MyDyn::LoadFrom(Storage & s);
 * @endcode
 * When a MyDyn is stored/loaded on/from a Storage, and the Storage detects that it is a component,
 * it calls those functions in order to store/load all meaningful MyDyn subparts if it has any.
 * So by redefining those functions we will change its XML representation.
 *
 * @section XmlReordering Reordering and skiping attributes
 *
 * Dynamic Types macros expand some useful methods that allow simplifying the customization.
 *
 * For each dynamic attribute named XXX, dynamic type macros expand the methods:
 * @code
 * void ConcreteDT::StoreXXX(Storage & s);
 * void ConcreteDT::LoadXXX(Storage & s);
 * @endcode
 *
 * Using such methods you can easily store/load a concrete dynamic attribute separately.
 * Be careful, LoadXXX requires the attribute XXX to be instantiated before calling it
 * and it will mark it automatically as removed if the attribute is not present in the XML file.
 * It is important to store the attributes in the same order you load them.
 *
 *
 * The following example will store and load its attributes
 * in the inverse order to the default one, and skips the third attribute (MyDynType).
 *
 * @code
	void ConcreteDT::StoreOn(CLAM::Storage & storage)
	{
		StoreMyInt(storage);
		StoreMyList(storage);
		// MyDynType is not stored
		StoreMyArray(storage);
		StoreMyDummyComponent(storage);
	}
	void ConcreteDT::LoadOn(CLAM::Storage & storage)
	{
		// First of all asure that all attributes are instantiated
		AddAll()
		UpdateData();
		// Then load them
		LoadMyInt(storage);
		LoadMyList(storage);
		// MyDynType is not loaded
		LoadMyArray(storage);
		LoadMyDummyComponent(storage);
	}
 * @endcode
 * @section XmlRecallingDefaults Recalling the default implementation
 *
 * StoreAllDynAttributes() and LoadAllDynamicAttributes() are another macro expanded methods.
 * They are called from the default StoreOn and LoadFrom implementation.
 * So, by calling them we can reproduce them and it is easy
 * to add non dynamic subparts before or after them or forcing some attributes to be or not present before them.
 * The first step of LoadAllDynamicAttributes() is to instantiate 
 * all the dynamic attributes that will be marked as erased if they are not in the XML document.
 * 
 * @section XmlNonDynamicAttributes Adding content not from dynamic attributes
 * 
 * If you simply want to add a non dynamic attribute to the XML representation,
 * you may call those expanded functions and then using a suited XML adapter for the attribute and store it.
 * Refer on how to define the XML format for a normal (non DynamicType)
 * Component to know about those adaptators and how they are used.
 *
 * The following example stores two extra items on the XML.
 * An existing member of the class (mExtraNonDynamicAttribute)
 * and a literal string as an XML attribute (the false value).
 * @code
void ConcreteDT::StoreOn(CLAM::Storage & storage)s
{
	// Store a temporary object in the first place
	CLAM::XMLAdapter<char*> adapter1("Addedcontent", "Added", false);
	storage.Store(&adapter1);

	// Call the default implementation
	StoreAllDynAttributes();

	// Store a non dynamic attribute member
	CLAM::XMLComponentAdapter adapter2(mExtraNonDynamicAttribute,
		"ExtraNonDynamic", true);
	storage.Store(&adapter2);
}

void ConcreteDT::LoadOn(CLAM::Storage & storage)
{
	// std::string is not vulnerable to buffer overflows on loading
	std::string foo; // A temp
	CLAM::XMLAdapter<std::string> adapter1(foo, "Added", false);
	storage.Load(&adapter1);

	LoadAllDynAttributes();

	CLAM::XMLComponentAdapter adapter2(mExtraNonDynamicAttribute,
		, "ExtraNonDynamic", true);
	storage.Load(&adapter2);
}
 * @endcode
 * @section XmlChangeTypeXmlOrTagName Storing not as XML elements or changing the tag name
 *
 *
 * Of course, we can also use Adapters with the dynamic attributes
 * instead of using StoreXXX and LoadXXX.
 * This is useful to store a dynamic attribute as XML attribute
 * or XML plain content or to change the name from the one the attribute has.
 * Again, refer to the XML developer guide.
 *
 *
 * When using adapters with dynamic attributes you must take care of some dynamic attributes tasks:
 *
 * When storing a dynamic attribute XXX you must check
 * that it is instantiated using the function HasXXX.
 * When loading you must check that the Storage::Load returns true.
 * When it returns false it is advisable to mark it as removed.
 * @code
void ConcreteDT::StoreOn(CLAM::Storage & storage)
{
	StoreMyDummyComponent(storage);
	StoreMyArray(storage);
	StoreMyDynType(storage);
	StoreMyList(storage);
	// MyInt is stored as an attribute (the default is element
	// and with a different name ('Size').
	if (HasMyInt())
	{
		CLAM::XMLAdapter<int> adapter(GetMyInt(), "Size", false);
		storage.Store(&adapter);
	}
}
void ConcreteDT::LoadOn(CLAM::Storage & storage)
{
	// First of all asure that all attributes are instantiated
	AddAll()
	UpdateData();
	// Then load them
	LoadMyDummyComponent(storage);
	LoadMyArray(storage);
	LoadMyDynType(storage);
	LoadMyList(storage);
	// MyInt is loaded as an attribute (the default is element
	// and with a different name ('Size').
	CLAM::XMLAdapter<int> adapter(GetMyInt(), "Size", false);
	if (!storage.Load(&adapter))
	{
		RemoveMyInt();
	}
}
 * @endcode
 *
 * @section XmlAlternativeFormats Keeping several alternative XML formats
 *
 * Normally you will define the storage customization on the same concrete dynamic type class.
 * But sometimes, you want to keep the default implementation o several customized implementations.
 * A good way of doing this is by subclassing the concrete Dynamic Type
 * and redefining the storage related methods as above but in the subclasses.
 *
 * @author David Garcia.
 */

/**
 * @defgroup XmlAdapters Using XML Adapters to implement StoreOn and LoadFrom
 * @author David Garcia.
 * @ingroup XmlCustomize
 * @brief How to use XmlAdapters to implement Load and Store methods for a CLAM::Component.
 *
 * When you are implementing StoreOn and LoadFrom for a given
 * Component, you should choose one of those adapters to wrap
 * the subitems to be stored on, or loaded from, the XmlStorage.
 *
 * When you create an adapter, the adaptee is specified by the 
 * the first constructor parameters, so they are adapter dependant.
 * The later two constructor parameters are always to indicate in which
 * XML form the subitem will be stored or loaded.
 * @code
 * 	MyAdapterClass( // Here goes the adaptee info 
 * 		, char * name = NULL, bool beElement = false );
 * @endcode
 * - As plain content (name = null, beElement = false)
 * - As XML attribute (name = "yourAttributeName", beElement = false)
 * - As XML element   (name = "yourElementName", beElement = true)
 *
 * You can use them in the following form:
 * @code
 * void StoreOn(CLAM::Storage & storage) const
 * {
 * 	SelectedAdapter adapter(subitem, "ElementName", true);
 * 	storage.Store(adapter);
 * }
 * void LoadFrom(CLAM::Storage & storage)
 * {
 * 	SelectedAdapter adapter(subitem, "ElementName", true);
 * 	if (!storage.Load(adapter))
 * 	{
 * 		// error handling
 * 	}
 * }
 * @endcode
 *
 * @warning The adapter only copies the pointer to
 * the the null-terminated string, not a copy of it.
 * So it is dangerous to delete or modify this string 
 * until the adapter has been stored on the Storage.
 *
 * For more information see the documentation for any
 * concrete adapter class.
 *
 * @author David Garcia.
 */

#endif//_XMLStorage_hxx_

