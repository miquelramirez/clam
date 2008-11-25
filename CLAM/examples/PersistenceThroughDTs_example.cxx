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

#include <iostream>
#include "DynamicType.hxx"
#include "XMLStorage.hxx"
#include "XMLAdapter.hxx"
#include "XMLIterableAdapter.hxx"
#include "Err.hxx"
#include "List.hxx"
#include "Text.hxx"
#include <string>
#include <list>
#include <fstream>

// Let's suppose you are provided with an XML Schema definition such as the
// follwing one:

//<?xml version="1.0"?>
//<xsd:schema xmlns:xsd="http://www.w3.org/2001/XMLSchema"
//            targetNamespace="http://www.books.org"
//            xmlns="http://www.books.org"
//            elementFormDefault="qualified">
//    <xsd:element name="BookStore">
//        <xsd:complexType>
//            <xsd:sequence>
//                <xsd:element name="Book" maxOccurs="unbounded">
//                    <xsd:complexType>
//                        <xsd:sequence>
//                            <xsd:element name="Title" type="xsd:string"/>
//                            <xsd:element name="Author" type="xsd:string"/>
//                            <xsd:element name="Date" type="xsd:string"/>
//                            <xsd:element name="ISBN" type="xsd:string"/>
//                            <xsd:element name="Publisher" type="xsd:string"/>
//                        </xsd:sequence>
//                    </xsd:complexType>
//                </xsd:element>
//            </xsd:sequence>
//        </xsd:complexType>
//    </xsd:element>
//</xsd:schema>


// and you are told to take these object definitions and implement them using CLAM.
// Obviously, there's also the requirement that the C++ objects can be retrieved from
// existing XML documents and stored into other XML documents. How can you accomplish
// with that in a fast and easy way? The answer is to use Dynamic Type's.

// One of the niftiest features the Dynamic Types offer is that of automatic XML store
// and load services. But first you must code the object descriptions into a DynamicType.
// Let's begin by the "Book" element description:

// Usual DynamicType declaration
class Book : public CLAM::DynamicType
{
public:
	DYNAMIC_TYPE( Book, 5 );
	DYN_ATTRIBUTE( 0, public, CLAM::Text,             Title );
	DYN_ATTRIBUTE( 1, public, CLAM::Text,             Author );
	DYN_ATTRIBUTE( 2, public, CLAM::Text,             Date );
	DYN_ATTRIBUTE( 3, public, CLAM::Text,             ISBN );
	DYN_ATTRIBUTE( 4, public, CLAM::Text,             Publisher );

	// Let's define a DefaultInit() for this dynamic type
	void DefaultInit();
};

void Book::DefaultInit()
{
	// We just add all the dynamic attributes
	AddAll();
	UpdateData();
}

// However, for the container we will have to use a CLAM::Component
class BookStore : public CLAM::Component
{
public:
	// We declare an alias for a STL list of Book 
	typedef std::list<Book> BookList;

	// These are the implementations for the three abstract methods
	// declared in CLAM::Component

	// The first one, the StoreOn will store the object into some sort
	// of storage
	virtual void StoreOn( CLAM::Storage& storer) const;
	// The second one, the LoadFrom, will load the object from some sort
	// of storage
	virtual void LoadFrom( CLAM::Storage& loader );
	// And finally, the GetClassName, so we can get a human-readable 
	// class name. Note that the ClassName will be used as this object
	// element tag name
	virtual const char* GetClassName() const;
	
	// Accessor to the list of books
	BookList& BooksInStore();

protected:

	BookList   mBooksInStore;
};

// Trivial implementations of BookStore::BooksInStore() and BookStore::GetClassName()
// methods
BookStore::BookList& BookStore::BooksInStore() 
{
	return mBooksInStore;
}

const char* BookStore::GetClassName() const
{
	return "BookStore";
}

void BookStore::StoreOn( CLAM::Storage& storer ) const
{
	// To store correctly a 'BookStore' element we should first specify as attributes
	// the XML namespace, XSI value and the XML Schema location

	// We first create the values for these attributes
	std::string xmlnsValue("http://www.books.org");
	std::string xmlns_xsiValue( "http://www.w3.org/2001/XMLSchema-instance" );
	std::string xsi_schemaLocationValue( "http://www.books.org/BookStore.xsd" );

	// We create CLAM::XMLAdapters for these attributes

	CLAM::XMLAdapter< std::string > xmlnsAttr( xmlnsValue, "xmlns" );
	CLAM::XMLAdapter< std::string > xmlns_xsiAttr( xmlns_xsiValue, "xmlns:xsi"); 
	CLAM::XMLAdapter< std::string > xsi_schemaLocationAttr( xsi_schemaLocationValue, "xsi:schemaLocation");

	// And we pass to the storage object the adapter for these attributes
	storer.Store( xmlnsAttr );
	storer.Store( xmlns_xsiAttr );
	storer.Store( xsi_schemaLocationAttr );

	// And now we must store as plain content the sequence of books
	// First we create the adapter for an 'iterable' container - such as the ones
	// offered by STL
	CLAM::XMLIterableAdapter< BookList > listAdapter( mBooksInStore, "Book" );

	// We pass to the storage object the adapter for the object
	storer.Store( listAdapter );
}

void BookStore::LoadFrom( CLAM::Storage& loader )
{
	// To load the Books inside the BookStore element we first create
	// an Iterable adapter 
	CLAM::XMLIterableAdapter< BookList > listAdapter( mBooksInStore, "Book" );

	// and then we tell the storage object to load those sub-elements
	// into the object adapted by listAdapter
	loader.Load( listAdapter );
}

int main( int argc, char** argv )
{
	try
	{
		// We create an instance of BookStore so it becomes the persistent object
		// encoded in XML document
		BookStore store;

		// We retrieve the BookStore object encoded in the XML document
		CLAM::XMLStorage::Restore( store, "BookStore.xml" );
		
		// Now we will create a new entry in the BookStore object
		Book aNewBook;
		aNewBook.SetTitle( "Design Patterns" );
		aNewBook.SetAuthor( "Eric Gamma et al." );
		aNewBook.SetDate( "1994" );
		aNewBook.SetISBN( "5-44302-881-2" );
		aNewBook.SetPublisher( "Prentice Hall" );

		// We add the new book to the store object
		store.BooksInStore().push_back( aNewBook );

		// We create an output file stream to hold the new document
		std::ofstream outputFile( "CLAM-generated-BookStore.xml" );
		
		// We insert the XML docuemnt prolog node
		outputFile << "<?xml version=\"1.0\" ?>" << std::endl;

		// We dump the object onto the newly created document
		CLAM::XMLStorage::Dump( store, "BookStore",  outputFile );
		
		outputFile.close();
	}
	catch( CLAM::Err& e )
	{
		e.Print();
		return -1;
	}
	catch( std::exception& e )
	{
		std::cerr << e.what() << std::endl;
		return -1;
	}

	return 0;
}

