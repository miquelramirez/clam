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

#include "FLTKConfigurator.hxx"
//#include "QTConfigurator.hxx"
#include "HeapDbg.hxx"
#include "DynamicType.hxx"

#include "mtgsstream.h" // An alias for <sstream>
#include <iostream>
#include <string>
#include <vector>
#include <exception>

#include "Component.hxx"
#include "TypeInfo.hxx"
#include "TypeInfoStd.hxx"

#include "Filename.hxx"
#include "Enum.hxx"
#include "Processing.hxx"

using namespace CLAM;

namespace CLAMTest {

// Dummy Enum

class EDummy : public Enum {
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
Enum::tEnumValue EDummy::sEnumValues[] = {
	{EDummy::zero,"zero"},
	{EDummy::dos,"dos"},
	{EDummy::cent,"cent"},
	{EDummy::mil,"mil"},
	{0,NULL}
};

Enum::tValue EDummy::sDefault = EDummy::dos;


// Dummy Config

class DummySubConfig : public ProcessingConfig
{
public:
	DYNAMIC_TYPE_USING_INTERFACE (DummySubConfig,6,ProcessingConfig);
	DYN_ATTRIBUTE(0,public,std::string,Name);
	DYN_ATTRIBUTE(1,public,std::string,ThatIsAString);
	DYN_ATTRIBUTE(2,public,TData,ThatIsATData);
	DYN_ATTRIBUTE(3,public,TSize,ThatIsATSize);
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

class NotSupportedType : public Component {
	void StoreOn(Storage &) const { }
	void LoadFrom(Storage &) { }
	const char * GetClassName() const { return "NotSupportedType"; }
};

class DummyConfig : public ProcessingConfig
{
public:
	DYNAMIC_TYPE_USING_INTERFACE (DummyConfig,10,ProcessingConfig);
	DYN_ATTRIBUTE(0,public,std::string,Name);
	DYN_ATTRIBUTE(1,public,std::string,ThisisAString);
	DYN_ATTRIBUTE(2,public,TData,ThisIsATData);
	DYN_ATTRIBUTE(3,public,TSize,ThisIsATSize);
	DYN_ATTRIBUTE(4,public,EDummy, ThisIsAEDummy);
	DYN_ATTRIBUTE(5,public,NotSupportedType, ThisIsNotSupportedType);
	DYN_ATTRIBUTE(6,public,DummySubConfig, ThisIsASubConfig);
	DYN_ATTRIBUTE(7,public,bool, ThisIsABool);
	DYN_ATTRIBUTE(8,public,DummySubConfig, ThisIsDifferentSubConfig);
	DYN_ATTRIBUTE(9,public,Filename, ThisIsAFilename);

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
//using namespace CLAM;

#include <FL/Fl.H>
//#include <qapplication.h>


using namespace CLAMTest;

int TryQTConfigurator(DummyConfig & config, int argc, char**argv)
{/*
	QApplication a(argc,argv);
	CLAM::QTConfigurator configurator;
	configurator.SetConfig(config);
	configurator.show();
	a.setMainWidget( &configurator );
	return a.exec();*/
	return 0;
}

int TryFLTKConfigurator(DummyConfig & config)
{
	CLAM::FLTKConfigurator * configurator = new CLAM::FLTKConfigurator;
	configurator->SetConfig(config);
	configurator->show();
	return Fl::run();
}

void TryDisplayXML(DummyConfig & config) 
{
	XMLStorage storage;
	storage.Dump(config,"DummyConfig", std::cout);
}

void TryStoreXML(DummyConfig & config, char * xmlfilename) 
{
	XMLStorage storage;
	storage.Dump(config,"DummyConfig", xmlfilename);
}

void TryLoadXML(DummyConfig & config, char * xmlfilename) 
{
	try {
		XMLStorage storage;
		storage.Restore(config, xmlfilename);
	} catch (...) {
		std::cout 
			<< "Could not read the file '" << xmlfilename <<  "'. " 
			<< "Working with a default configuration." << std::endl;
	}
}

int main(int argc, char** argv)
{
	int ret=0;
	try {
		DummyConfig config;
		TryLoadXML(config,"configout.xml");
		TryQTConfigurator(config,argc,argv);
		TryFLTKConfigurator(config);
		TryStoreXML(config,"configout.xml");
	} catch (CLAM::Err e)
	{
		e.Print();
		std::cout << "Failed!" << std::endl;
		return 1;
	} catch (std::exception e)
	{
		std::cout << e.what() << std::endl;
	} catch (...)
	{
		std::cout << "catch (...)" << std::endl;
	}

	return ret;	
} 
