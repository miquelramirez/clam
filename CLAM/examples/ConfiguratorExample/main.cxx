#include <iostream>
#include <list>
#include <CLAM/XMLStorage.hxx>
#include <CLAM/ProcessingConfig.hxx>
#include <CLAM/Enum.hxx>
#include <CLAM/Filename.hxx>
#include <CLAM/AudioInFilename.hxx>
#include <CLAM/AudioOutFilename.hxx>
#include <CLAM/BPF.hxx>
#include "Configurator.hxx"

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

namespace CLAMTest
{
	// Firstly we will define the objects that we want to modify.
	// The enumeration must be defined as a class.
	class EDummy : public CLAM::Enum {
	public:
		EDummy() : Enum(EnumValues(), Default()) {}
		EDummy(tValue v) : Enum(EnumValues(), v) {};
		EDummy(std::string s) : Enum(EnumValues(), s) {};
		virtual ~EDummy() {};
		Component * Species() const { return new EDummy;};

		typedef enum
		{
			zero=0,
			dos=2,
			cent=100,
			mil=1000
		} tEnum;
		static const tEnumValue * EnumValues()
		{
			static tEnumValue sEnumValues[] = {
				{zero,"zero"},
				{dos,"dos"},
				{cent,"cent"},
				{mil,"mil"},
				{0,NULL}
			};
			return sEnumValues;
		}
		static tValue Default() 
		{
			return dos;
		}
	};

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
		DYNAMIC_TYPE_USING_INTERFACE (DummyConfig,12,ProcessingConfig);
		DYN_ATTRIBUTE(0,public,CLAM::Text,ThisIsAText);
		DYN_ATTRIBUTE(1,public,std::string,ThisisAString);
		DYN_ATTRIBUTE(2,public,CLAM::TData,ThisIsATData);
		DYN_ATTRIBUTE(3,public,CLAM::TSize,ThisIsATSize);
		DYN_ATTRIBUTE(4,public,EDummy, ThisIsAEDummy);
		DYN_ATTRIBUTE(5,public,NotSupportedType, ThisIsNotSupportedType);
		DYN_ATTRIBUTE(6,public,DummySubConfig, ThisIsASubConfig);
		DYN_ATTRIBUTE(7,public,bool, ThisIsABool);
		DYN_ATTRIBUTE(8,public,DummySubConfig, ThisIsDifferentSubConfig);
		DYN_ATTRIBUTE(9,public,CLAM::BPF, ThisIsABPF);
		DYN_ATTRIBUTE(10,public,CLAM::AudioInFilename, ThisIsAnAudioInFilename);
		DYN_ATTRIBUTE(11,public,CLAM::AudioOutFilename, ThisIsAnAudioOutFilename);


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

#include <QtGui/QApplication>
#include <QtGui/QDialog>
#include "Configurator.hxx"

int main( int argc, char *argv[] )
{
	QApplication app( argc, argv );

	DummyConfig config;
	const char * xmlfilename="config.xml";

	try
	{
		CLAM::XmlStorage::Restore(config, xmlfilename);
	}
	catch (std::exception & e)
	{
		std::cerr << e.what() << std::endl;
		std::cout 
			<< "Could not read the file '" << xmlfilename <<  "'. " 
			<< "Working with a default configuration." << std::endl;
	}
	
	Configurator configurator(config);
	configurator.show();
	int result = app.exec();
	CLAM::XmlStorage::Dump(config,"DummyConfig", xmlfilename);
	CLAM::XmlStorage::Dump(config,"DummyConfig", std::cout);
	return result;
}


