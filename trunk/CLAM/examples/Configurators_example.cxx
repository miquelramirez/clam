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

/**
 * @example Configurators_example.cxx
 * This example shows how the diferent values inside a dinamic type
 * can be shown and edited using the Qt and FLTK configurator widgets.
 * The kind of attributes that are shown are:
 * - Boolean
 * - std::string and CLAM::Text (the latter allows spaces)
 * - Integer numbers
 * - Double numbers
 * - Filenames
 * - Another dynamic type
 * - Enumerations (limited set of values)
 */


#include "XMLStorage.hxx"
#include "ProcessingConfig.hxx"
#include "Enum.hxx"
#include "Filename.hxx"
#include "BPF.hxx"


namespace CLAMTest
{

	// Firstly we will define the objects that we want to modify.

	// The enumeration must be defined as a class.
	class EDummy : public CLAM::Enum {
	public:
		static tEnumValue sEnumValues[];
		static tValue sDefault;
		EDummy() : Enum(sEnumValues, sDefault) {}
		EDummy(tValue v) : Enum(sEnumValues, v) {};
		EDummy(std::string s) : Enum(sEnumValues, s) {};
		virtual ~EDummy() {};
		Component * Species() const { return new EDummy;};

		typedef enum {
			zero=0,
			dos=2,
			cent=100,
			mil=1000
		} tEnum;
		static void TestClass ();
	};
	CLAM::Enum::tEnumValue EDummy::sEnumValues[] = {
		{EDummy::zero,"zero"},
		{EDummy::dos,"dos"},
		{EDummy::cent,"cent"},
		{EDummy::mil,"mil"},
		{0,NULL}
	};

	CLAM::Enum::tValue EDummy::sDefault = EDummy::dos;


	// Also the sub configuration, a configuration object
	// inside the main one.

	class DummySubConfig : public CLAM::ProcessingConfig
	{
	public:
		DYNAMIC_TYPE_USING_INTERFACE (DummySubConfig,6,ProcessingConfig);
		DYN_ATTRIBUTE(0,public,std::string,Name);
		DYN_ATTRIBUTE(1,public,std::string,ThatIsAString);
		DYN_ATTRIBUTE(2,public,CLAM::TData,ThatIsATData);
		DYN_ATTRIBUTE(3,public,CLAM::TSize,ThatIsATSize);
		DYN_ATTRIBUTE(4,public,EDummy, ThatIsAEDummy);
		DYN_ATTRIBUTE(5,public,bool, ThatIsABool);
	private:

		void DefaultInit() {
			AddAll();
			UpdateData();
			DefaultValues();
		}
		void DefaultValues() {
		}

	public:
		~DummySubConfig(){};
	};

	// A not suported type, and see what is happening
	class NotSupportedType : public CLAM::Component {
		void StoreOn(CLAM::Storage &) const { }
		void LoadFrom(CLAM::Storage &) { }
		const char * GetClassName() const { return "NotSupportedType"; }
	};

	// And at last, the main configuration class
	// including all the above and more
	class DummyConfig : public CLAM::ProcessingConfig
	{
	public:
		DYNAMIC_TYPE_USING_INTERFACE (DummyConfig,11,ProcessingConfig);
		DYN_ATTRIBUTE(0,public,std::string,Name);
		DYN_ATTRIBUTE(1,public,std::string,ThisisAString);
		DYN_ATTRIBUTE(2,public,CLAM::TData,ThisIsATData);
		DYN_ATTRIBUTE(3,public,CLAM::TSize,ThisIsATSize);
		DYN_ATTRIBUTE(4,public,EDummy, ThisIsAEDummy);
		DYN_ATTRIBUTE(5,public,NotSupportedType, ThisIsNotSupportedType);
		DYN_ATTRIBUTE(6,public,DummySubConfig, ThisIsASubConfig);
		DYN_ATTRIBUTE(7,public,bool, ThisIsABool);
		DYN_ATTRIBUTE(8,public,DummySubConfig, ThisIsDifferentSubConfig);
		DYN_ATTRIBUTE(9,public,CLAM::Filename, ThisIsAFilename);
		DYN_ATTRIBUTE(10,public,CLAM::BPF, ThisIsABPF);


	private:

		void DefaultInit() {
			AddAll();
			UpdateData();
			DefaultValues();
		}
		void DefaultValues() {
		}
	public:
		~DummyConfig(){};
	};


}

using namespace CLAMTest;

#ifdef FLTK_library_removed
#include <FL/Fl.H>
#include "FLTKConfigurator.hxx"
int DisplayFLTKConfigurator(DummyConfig & config)
{
	CLAM::FLTKConfigurator * configurator = new CLAM::FLTKConfigurator;
	configurator->SetConfig(config);
	configurator->show();
	return Fl::run();
}
#endif

#include "QTConfigurator.hxx"
#include <qapplication.h>



int DisplayQTConfigurator(DummyConfig & config, int argc, char**argv)
{
	QApplication a(argc,argv);
	CLAM::QTConfigurator configurator;
	configurator.SetConfig(config);
	configurator.show();
	a.setMainWidget( &configurator );
	return a.exec();
}

int main(int argc, char** argv)
{
	const char * xmlfilename="config.xml";
	DummyConfig config;

	// If it is present, load the xml file
	try
	{
		CLAM::XmlStorage::Restore(config, xmlfilename);
	}
	catch (...)
	{
		std::cout 
			<< "Could not read the file '" << xmlfilename <<  "'. " 
			<< "Working with a default configuration." << std::endl;
	}

	// Use the configurators
	DisplayQTConfigurator(config,argc,argv);
#ifdef FLTK_library_removed
	DisplayFLTKConfigurator(config);
#endif
	// Display and store the results
	CLAM::XmlStorage::Dump(config,"DummyConfig", xmlfilename);
	CLAM::XmlStorage::Dump(config,"DummyConfig", std::cout);
	std::cout << std::endl;

	return 0;	
} 

