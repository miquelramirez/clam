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

//////////////////////////////////////////////////////////////////////
// XMLStorage.cxx
// Description: A storage in XML format for CLAM objects
//////////////////////////////////////////////////////////////////////
#include <fstream>
#include "mtgsstream.h" // An alias for <sstream>
#include <iostream>
#include "XMLStorage.hxx"
#include "DOMPrint.hpp"
#include "XMLable.hxx"
#include "Component.hxx"
#include <util/PlatformUtils.hpp>
#include <dom/DOM.hpp>
#include <list>
#include <deque>
#include "Assert.hxx"

#define TRACELOAD if (1) ; else std::cout

using std::ofstream;

using namespace CLAM;

namespace CLAM
{
	class XMLFrame {
		public:
			DOM_NamedNodeMap oldAttributes;
			std::stringstream stream;
			std::stringstream * oldStream;
			DOM_Node oldNode;

	};
	class XMLStoreFrame {
		public:
			DOM_Element oldNode;
	};

	const char * CLAMNamespaceURI = "http://www.iua.upf.es/CLAM03-200204";

	class XMLStorageImplementation {
		public:
			XMLStorageImplementation() {}
			virtual ~XMLStorageImplementation() {}
		// Write methods
		public:
			/**
			 * Starts a new Document Object Model for dumping contents on it
			 * @param rootName The name of the root element
			 */
			virtual void NewDocument(const char * rootName)=0;

			/**
			 * Creates a node and
			 * @return The old writing node
			 */
			virtual void StartWriteElement(XMLStoreFrame &frame, const char * name)=0;

			/**
			 * Indicates that the current element writting has stoped
			 * and restores the previous writting frame.
			 */
			virtual void StopWriteElement(XMLStoreFrame &frame)=0;

			/**
			 * Adds an attribute to the current write element
			 */
			virtual void AddAttribute(const char * name, const char * content)=0;

			/**
			 * Adds plain text content to the current write node
			 */
			virtual void AddPlainContent(const char * content)=0;

			/**
			 * Adds an space separator text content if the last action was adding a content node
			 */
			virtual void AddSeparatorIfNeeded() = 0;

			/**
			 * Writes a dumped DOM on a stream
			 * @param os The recipient stream
			 */
			virtual void WriteDOM(std::ostream & os)=0;

		// Read methods
		public:
			/**
			 * Loads a Document Object model from a named file
			 */
			virtual void Parse(const char * recipientName)=0;

			/**
			 * Places the read frame at the root of the DOM
			 */
			virtual void PrepareReadAtRoot() =0;

			/**
			 * Opens a read frame for a new
			 * @param frame The old read frame
			 * @param name The element to be searched from the DOM
			 * @return Whether the named element exists or not
			 */
			virtual bool StartReadElement(XMLFrame & frame, const char *name)=0;

			virtual void StopReadElement(XMLFrame & f)=0;

			/**
			 *
			 * @return Whether the attribute exists
			 */
			virtual bool ExtractAttribute(const char * name, std::ostream & os) = 0;

			virtual std::istream & ExtractContent() = 0;

		// Internal read implementation (Maybe protected?)

			virtual bool ChangeReadingNode(const char * name)=0;

			/**
			* Prepares available content nodes from the current reading node
			*/
			virtual void FetchContent() = 0;

			/**
			* Fetchs from the first not space on the content stream
			*/
			virtual void EatSpacesFromContent() = 0;

		// Factory methods
			static XMLStorageImplementation * NewDefaultXMLImplementation();
	};

	class XercesXMLStorageImplementation : public XMLStorageImplementation
	{
		public:
		XercesXMLStorageImplementation() {
			if (!sPlatformUtilsInitialized) {
				try
				{
					XMLPlatformUtils::Initialize();
				}
				catch(const XMLException& toCatch)
				{
					std::cerr << "Error during Xerces-c Initialization.\n"
					     << "  Exception message:"
						 << DOMString(toCatch.getMessage()) << std::endl;
					return;
				}
				sPlatformUtilsInitialized = true;
			}
		}
		virtual ~XercesXMLStorageImplementation() {
		/*
			try {
				XMLPlatformUtils::Terminate();
			}
			catch(const XMLException& toCatch)
			{
				cerr << "Error during Xerces-c Termination.\n"
				     << "  Exception message:"
				     << DOMString(toCatch.getMessage()) << endl;
				return;
			}
		*/
		}
		private:
			static bool sPlatformUtilsInitialized;
		// Common variables
		public:
			DOM_Document DOMDoc;
		// Write variables
		public:
			DOM_Element mWrittingNode;
			bool lastWasContent;
		// Read variables
			DOM_Node mReadingNode;
			DOM_NamedNodeMap mAttributes;
			std::stringstream * mTextNodesContent;
			std::stringstream mDummyStream;
		// Write methods
		public:
			void NewDocument(const char * rootName) {
				DOM_DOMImplementation & impl = DOM_DOMImplementation::getImplementation();
				DOMDoc = impl.createDocument(
					"http://www.iua.upf.es/CLAM03-200204", // root element namespace URI.
					rootName,            // root element name
					DOM_DocumentType()  // document type object (DTD).
					);
				mWrittingNode = DOMDoc.getDocumentElement();
				lastWasContent=false;
			}
			virtual void WriteDOM(std::ostream & os) {
				PrintDoc(os,DOMDoc);
			}
			void AddAttribute(const char * name, const char * content) {
				mWrittingNode.setAttribute(name, content);
			}
			void StartWriteElement(XMLStoreFrame &frame, const char * name) {
				frame.oldNode = mWrittingNode;
				mWrittingNode = DOMDoc.createElement(name);
				frame.oldNode.appendChild(mWrittingNode);
				lastWasContent=false;
			}
			void StopWriteElement(XMLStoreFrame &frame) {
				mWrittingNode = frame.oldNode;
				// The inner elements can have set it to true
				lastWasContent=false;
			}
			bool StartReadElement(XMLFrame & frame, const char *name) {
				frame.oldNode = mReadingNode;
				if (!ChangeReadingNode(name)) return false;
				// Change the current attributes
				frame.oldAttributes = PushReadAttributes();
				// Change the current Plain text stream
				frame.oldStream = mTextNodesContent;
				mTextNodesContent = &(frame.stream);
				return true;
			}
			void AddPlainContent(const char * content) {
				DOM_Text domContent = DOMDoc.createTextNode(content);
				mWrittingNode.appendChild(domContent);
				// A menys que el contingut sigui buit
				if (*content) lastWasContent=true;
			}
			virtual void AddSeparatorIfNeeded() {
				if (lastWasContent) {
					DOM_Text content = DOMDoc.createTextNode(" ");
					mWrittingNode.appendChild(content);
				}
			}
		// Read methods
		public:
			void Parse(const char * recipientName) {
				// Instantiate the DOM parser.
				DOMParser parser;
				parser.setValidationScheme(DOMParser::Val_Auto);
				parser.setDoNamespaces(true);
				parser.setDoSchema(false);

				// And create our error handler and install it
			//	DOMCountErrorHandler errorHandler;
			//	parser.setErrorHandler(&errorHandler);

				// Parse the document and catch error exceptions

				try
				{
					parser.parse(recipientName);
				}
				catch (const XMLException& toCatch)
				{
					std::cerr << "\nError during parsing: '" << recipientName << "'\n"
					     << "Exception message is:  \n"
					     << toCatch.getMessage() << "\n" << std::endl;
					throw;
				}
				catch (const DOM_DOMException& toCatch)
				{
					std::cerr << "\nDOM Error during parsing: '" << recipientName << "'\n"
					     << "DOMException code is:  \n"
					     << toCatch.code << "\n" << std::endl;
					throw;
				}
				catch (...)
				{
					std::cerr << "\nUnexpected exception during parsing: '"
						<< recipientName << "'\n";
					throw;
				}
				DOMDoc = parser.getDocument();
			}
			bool ExtractAttribute(const char * name, std::ostream & os) {
				DOM_Node attribute = mAttributes.getNamedItem(name);
				if (attribute.isNull()) return false;
				os << attribute.getNodeValue();
				return true;
			}
			virtual std::istream & ExtractContent() {
				FetchContent();
				return *mTextNodesContent;
			}
			virtual void PrepareReadAtRoot() =0;
			virtual bool ChangeReadingNode(const char * name)=0;
			virtual void StopReadElement(XMLFrame & f)=0;
			/**
			* Prepares available content nodes from the current reading node
			*/
			virtual void FetchContent() = 0;
			/**
			* Fetchs from the first not space on the content stream
			*/
			virtual void EatSpacesFromContent() = 0;

		protected:
			DOM_NamedNodeMap PushReadAttributes() {
				// Change the current attributes
				DOM_NamedNodeMap oldAttributes = mAttributes;
				mAttributes = mReadingNode.getAttributes();
				return oldAttributes;
			}
	};

	class XercesOrderedXMLStorageImplementation : public XercesXMLStorageImplementation {
			std::deque<unsigned int> mReadPositionsStack;
			unsigned int mReadPosition;
		public:
			virtual void PrepareReadAtRoot() {
				mReadingNode = DOMDoc.getDocumentElement();
				mAttributes = mReadingNode.getAttributes();
				mTextNodesContent=&mDummyStream;
				mReadPosition=0;
			}
			bool ChangeReadingNode(const char * name) {
				// Search the node
				DOM_NodeList subNodes=mReadingNode.getChildNodes();
				if (mReadPosition>=subNodes.getLength()) return false;
				// Change the reading node
				DOM_Node candidateNode = subNodes.item(mReadPosition);
				// TODO: Check that any text node is empty
				if (candidateNode.getNodeType()!=DOM_Node::ELEMENT_NODE) return false;
				if (!(candidateNode.getNodeName().equals(name))) return false;
				mReadingNode = candidateNode;
				// Create a new set of nodes to visit
				mReadPositionsStack.push_back(mReadPosition+1);
				mReadPosition=0;
				return true;
			}
			void StopReadElement(XMLFrame & frame) {
				mTextNodesContent = frame.oldStream;
				mReadPosition=mReadPositionsStack.back();
				mReadPositionsStack.pop_back();
				mAttributes = frame.oldAttributes;
				mReadingNode = frame.oldNode;
			}
			/**
			* Prepares available content nodes from the current reading node
			*/
			void FetchContent() {
				DOM_NodeList subNodes=mReadingNode.getChildNodes();
				EatSpacesFromContent();
				const unsigned int maxNodes=subNodes.getLength();
				for (; mReadPosition<maxNodes; mReadPosition++) {
					if (subNodes.item(mReadPosition).getNodeType()!=DOM_Node::TEXT_NODE) {
						break;
					}
					mTextNodesContent->clear();
					// Get the content
					(*mTextNodesContent) << subNodes.item(mReadPosition).getNodeValue() << std::flush;
					// Remove the visited node
					EatSpacesFromContent();
				}
			}
			/**
			* Fetchs from the first not space on the content stream
			*/
			void EatSpacesFromContent() {
				char c;
				do mTextNodesContent->get(c);
				while ((!mTextNodesContent->fail()) && isspace(c));
				if (!mTextNodesContent->fail()) (*mTextNodesContent).putback(c);
			}
	};
	class XercesUnorderedXMLStorageImplementation : public XercesXMLStorageImplementation {
			std::deque<std::list<unsigned int> > mAllNodesLeft;
		public:
			virtual void PrepareReadAtRoot() {
				mReadingNode = DOMDoc.getDocumentElement();
				mAttributes = mReadingNode.getAttributes();
				mTextNodesContent=&mDummyStream;
				mAllNodesLeft.push_back(std::list<unsigned int>());
				for (unsigned int i=mReadingNode.getChildNodes().getLength(); i--; )
					mAllNodesLeft.back().push_back(i);
			}
			virtual bool ChangeReadingNode(const char * name) {
				// Search the node
				std::list<unsigned int> &levelNodesLeft = mAllNodesLeft.back();
				DOM_NodeList subNodes=mReadingNode.getChildNodes();
				std::list<unsigned int>::iterator it;
				for (it = levelNodesLeft.begin();
						it!=levelNodesLeft.end();
						it++) {
					DOM_Node subNode = subNodes.item(*it);
					if (subNode.getNodeType()!=DOM_Node::ELEMENT_NODE) continue;
					if (!(subNode.getNodeName().equals(name))) continue;
					break; // I've found the element
				}
				if (it==levelNodesLeft.end()) return false;
				// Change the reading node
				mReadingNode = subNodes.item(*it);
				// Remove the visited node
				mAllNodesLeft.back().erase(it);
				// Create a new set of nodes to visit
				mAllNodesLeft.push_back(std::list<unsigned int>());
				const unsigned howManyNodes = mReadingNode.getChildNodes().getLength();
				for (unsigned int i=0; howManyNodes>i; i++ )
					mAllNodesLeft.back().push_back(i);
				return true;
			}
			virtual void StopReadElement(XMLFrame & frame) {
				mTextNodesContent = frame.oldStream;
				mAllNodesLeft.pop_back();
				mAttributes = frame.oldAttributes;
				mReadingNode = frame.oldNode;
			}
			/**
			* Prepares available content nodes from the current reading node
			*/
			virtual void FetchContent() {
				std::list<unsigned int> & nodesLeft = mAllNodesLeft.back();
				DOM_NodeList subNodes=mReadingNode.getChildNodes();
				EatSpacesFromContent();
			//	TRACELOAD << "Searching content nodes from " << nodesLeft.size() << std::endl;
				std::list<unsigned int>::iterator it = nodesLeft.begin();
				while ((*mTextNodesContent).eof() && it!=nodesLeft.end()) {
					// Filter non plain text nodes
					if (subNodes.item(*it).getNodeType()!=DOM_Node::TEXT_NODE) {
						it++;
						continue;
					}
					mTextNodesContent->clear();
					// Get the content
					(*mTextNodesContent) << subNodes.item(*it).getNodeValue() << std::flush;
					// Remove the visited node
					it=nodesLeft.erase(it);
					EatSpacesFromContent();
				}
			}
			/**
			* Fetchs from the first not space on the content stream
			*/
			virtual void EatSpacesFromContent() {
				char c;
				do mTextNodesContent->get(c);
				while ((!mTextNodesContent->fail()) && isspace(c));
				if (!mTextNodesContent->fail()) (*mTextNodesContent).putback(c);
			}

	};

//	typedef XercesOrderedXMLStorageImplementation CurrentXMLStorageImplementation;
	typedef XercesUnorderedXMLStorageImplementation CurrentXMLStorageImplementation;

}


//////////////////////////////////////////////////////////////////////
// Constructors
//////////////////////////////////////////////////////////////////////

XMLStorage::XMLStorage() {
	mPM=NewXMLImplementation();
}


//* @deprecated use default constructor
XMLStorage::XMLStorage(const char * rootElementName) {
	CLAM_WARNING(true, "Using XMLStorage deprecated constructor");
	mPM=NewXMLImplementation();
	mPM->NewDocument(rootElementName);
}

XMLStorageImplementation * XMLStorage::NewXMLImplementation() {
	return new CurrentXMLStorageImplementation;
}

XMLStorage::~XMLStorage()
{
	delete mPM;
}

//////////////////////////////////////////////////////////////////////
// Member Functions
//////////////////////////////////////////////////////////////////////

void XMLStorage::Dump(Component & component, const std::string& name, std::ostream & recipient)
{
	mPM->NewDocument(name.c_str());
	component.StoreOn(*this);
	mPM->WriteDOM(recipient);
}

void XMLStorage::Dump(Component & component, const std::string& name, const std::string& recipientName)
{
	std::ofstream recipient(recipientName.c_str());
	Dump(component, name, recipient);
}

void XMLStorage::Restore(Component & component, const std::string& recipientName) {
	mPM->Parse(recipientName.c_str());
	mPM->PrepareReadAtRoot();
	component.LoadFrom(*this);
}

//* @deprecated: use Dump
void XMLStorage::dumpOn(std::ostream & aStream) {
	CLAM_WARNING(true, "Using deprecated method XMLStorage::dumpOn");
	mPM->WriteDOM(aStream);
//	Inspect(aStream,mPM->DOMDoc);
//	aStream << mPM->DOMDoc << endl;
}

//* @deprecated: use Restore
void XMLStorage::_restoreFrom(char * xmlFile) {
	CLAM_WARNING(true, "Using deprecated method XMLStorage::_restoreFrom");
	mPM->Parse(xmlFile);
	mPM->PrepareReadAtRoot();
}

//////////////////////////////////////////////////////////////////////
// Redefined functions
//////////////////////////////////////////////////////////////////////


bool XMLStorage::Load (Storable * object) {
	XMLable * xmlObject = dynamic_cast<XMLable *>(object);

	if (!xmlObject) return false;

	if (xmlObject->IsXMLAttribute()) {
		TRACELOAD << "Attribute --" << std::endl;
		std::stringstream stream;
		if (!mPM->ExtractAttribute(xmlObject->XMLName(), stream))
			return false;
		return xmlObject->XMLContent(stream);
	}
	else if (xmlObject->IsXMLElement()) {
		TRACELOAD << "Element --" << std::endl;
		// Init a new frame on the element if it exists
		XMLFrame frame;
		if (!mPM->StartReadElement(frame,xmlObject->XMLName())) return false;
		// Parsing content nodes that belongs to the element part
		xmlObject->XMLContent(mPM->ExtractContent());
		// When the storable is a component, ask it to store its subitems
		Component * component = dynamic_cast<Component *>(object);
		if (component) component->LoadFrom(*this);
		else TRACELOAD << "Not a component" << std::endl;
		// Restore the old context
		mPM->StopReadElement(frame);
		return true;
	}
	else {
		TRACELOAD << "Content --" << std::endl;
		// Parsing content nodes
		if (!xmlObject->XMLContent(mPM->ExtractContent())) return false;
		// When the storable is a component, ask it to store its subitems
		Component * component = dynamic_cast<Component *>(object);
		if (component) component->LoadFrom(*this);
		else TRACELOAD << "Not a component" << std::endl;
		return true;
	}
	return true;
}

void XMLStorage::Store(Storable * object) {
	XMLable * xmlObject = dynamic_cast<XMLable *>(object);

	if (!xmlObject) return;

	if (xmlObject->IsXMLAttribute()) {
		mPM->AddAttribute(
				xmlObject->XMLName(),
				xmlObject->XMLContent().c_str());
	}
	else if (xmlObject->IsXMLElement()) {
		XMLStoreFrame frame;
		mPM->StartWriteElement(frame, xmlObject->XMLName());
		mPM->AddPlainContent(xmlObject->XMLContent().c_str());
		Component * component = dynamic_cast<Component *>(object);
		if (component) component->StoreOn(*this);
		mPM->StopWriteElement(frame);
	}
	else {
		mPM->AddSeparatorIfNeeded();
		mPM->AddPlainContent(xmlObject->XMLContent().c_str());
		Component * component = dynamic_cast<Component *>(object);
		if (component) component->StoreOn(*this);
	}
}

//////////////////////////////////////////////////////////////////////
// Static variables initialization
//////////////////////////////////////////////////////////////////////

bool XercesXMLStorageImplementation::sPlatformUtilsInitialized = false;



