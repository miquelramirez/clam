#include <cppunit/extensions/HelperMacros.h>
#include "cppUnitHelper.hxx" // necessary for the custom assert

#include "Pool.hxx"
#include "DataTypes.hxx"
#include "Spectrum.hxx"
#include "SpectrumConfig.hxx"
#include "Audio.hxx"
#include "SpectralAnalysis.hxx"
#include "XMLStorage.hxx"
#include "XMLAdapter.hxx"
#include <sstream>


namespace CLAMTest
{

class DescriptionXmlTest;

CPPUNIT_TEST_SUITE_REGISTRATION( DescriptionXmlTest );

class DescriptionXmlTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( DescriptionXmlTest );
	CPPUNIT_TEST(testDumpAttributePool_withSimpleData);
	CPPUNIT_TEST(testDumpAttributePool_withComponentData);
	CPPUNIT_TEST(testRestoreAttributePool_withSimpleData);
	CPPUNIT_TEST(testRestoreAttributePool_withComponentData);
	CPPUNIT_TEST(testRestoreAttributePool_withDifferentNameFails);
	CPPUNIT_TEST(testDumpScopePool_withNoAttributes);
	CPPUNIT_TEST(testDumpScopePool_withAttributesAndZeroSize);
	CPPUNIT_TEST(testDumpScopePool_withAttributes);
	CPPUNIT_TEST(testDumpScopePool_withIntegerAttributes);
	CPPUNIT_TEST(testDumpScopePool_withComponentAttributes);
	CPPUNIT_TEST(testDumpScopePool_withNonInstantiatedAttributes);
	CPPUNIT_TEST(testRestoreScopePool_withSingleAttribute);
	CPPUNIT_TEST(testRestoreScopePool_withSeveralAttributes);
	CPPUNIT_TEST(testRestoreScopePool_withBadScopeName);
	CPPUNIT_TEST(testDumpDescriptionDataPool_withAllKindsOfData);
	CPPUNIT_TEST(testRestoreDescriptionDataPool_withAllKindsOfData);
	CPPUNIT_TEST_SUITE_END();

public:
	/// Common initialization, executed before each test method
	void setUp() { }

	/// Common clean up, executed after each test method
	void tearDown() { }

private:
	std::ostringstream _targetStream;
	class DummyComponent : public CLAM::Component
	{
	public:
		void SetValue(const std::string & newValue)
		{
			_dummyMember=newValue;
		}
		const std::string & GetValue() const
		{
			return _dummyMember;
		}
		const char * GetClassName() const { return "DummyComponent"; }
		void StoreOn(CLAM::Storage & storage) const
		{
			CLAM::XMLAdapter<std::string> adapter(_dummyMember,"DummyMember",false);
			storage.Store(adapter);
		}
		void LoadFrom(CLAM::Storage & storage)
		{
			CLAM::XMLAdapter<std::string> adapter(_dummyMember,"DummyMember",false);
			storage.Load(adapter);
		}
	private:
		std::string _dummyMember;
	};

	void testDumpAttributePool_withSimpleData()
	{
		CLAM::Attribute<int> attribute("MyAttribute");
		CLAM::AttributePool pool;
		pool.SetDefinition(attribute);
		pool.Allocate(3);
		int * data = (int*) pool.GetData();
		for (unsigned int i = 0; i<3; i++) data[i]= -i;

		CLAM::XmlStorage::Dump(pool,"AttributePool",_targetStream);

		CPPUNIT_ASSERT_EQUAL(std::string(
			"<AttributePool name=\"MyAttribute\">"
			"0 -1 -2"
			"</AttributePool>"
			),_targetStream.str());

		pool.Deallocate();
	}
	void testDumpAttributePool_withComponentData()
	{
		CLAM::Attribute<DummyComponent> attribute("MyAttribute");
		CLAM::AttributePool pool;
		pool.SetDefinition(attribute);
		pool.Allocate(3);
		DummyComponent * data = (DummyComponent*) pool.GetData();
		data[0].SetValue("value0");
		data[1].SetValue("value1");
		data[2].SetValue("value2");

		CLAM::XmlStorage::Dump(pool,"AttributePool",_targetStream);

		CPPUNIT_ASSERT_EQUAL(std::string(
			"<AttributePool name=\"MyAttribute\">"
				"<DummyComponent DummyMember=\"value0\"/>"
				"<DummyComponent DummyMember=\"value1\"/>"
				"<DummyComponent DummyMember=\"value2\"/>"
			"</AttributePool>"
			),_targetStream.str());

		pool.Deallocate();
	}

	void testDumpAttributePool_withNonDumpableData()
	{
		//TODO:
	}

	void testDumpAttributePool_withNoData()
	{
		//TODO:
	}

	void testRestoreAttributePool_withSimpleData()
	{
		std::istringstream input(
			"<AttributePool name=\"MyAttribute\">"
			"0 -1 -2"
			"</AttributePool>");

		CLAM::Attribute<int> attribute("MyAttribute");
		CLAM::AttributePool pool;
		pool.SetDefinition(attribute);
		pool.Allocate(3);

		CLAM::XmlStorage::Restore(pool, input);

		const int * data = (int*) pool.GetData();
		for (unsigned int i = 0; i<3; i++)
			CPPUNIT_ASSERT_EQUAL((const int)-i, data[i]);

		pool.Deallocate();
	}
	void testRestoreAttributePool_withComponentData()
	{
		std::istringstream input(
			"<AttributePool name=\"MyAttribute\">"
			"<DummyComponent DummyMember=\"value0\"/>"
			"<DummyComponent DummyMember=\"value1\"/>"
			"<DummyComponent DummyMember=\"value2\"/>"
			"</AttributePool>");

		CLAM::Attribute<DummyComponent> attribute("MyAttribute");
		CLAM::AttributePool pool;
		pool.SetDefinition(attribute);
		pool.Allocate(3);

		CLAM::XmlStorage::Restore(pool, input);

		const DummyComponent * data = (DummyComponent*) pool.GetData();
		const std::string value0("value0"), value1("value1"), value2("value2");
		CPPUNIT_ASSERT_EQUAL(value0, data[0].GetValue());
		CPPUNIT_ASSERT_EQUAL(value1, data[1].GetValue());
		CPPUNIT_ASSERT_EQUAL(value2, data[2].GetValue());

		pool.Deallocate();
	}
	void testRestoreAttributePool_withDifferentNameFails()
	{
		std::istringstream input(
			"<AttributePool name=\"DifferentName\">"
			"0 -1 -2"
			"</AttributePool>");

		CLAM::Attribute<int> attribute("MyAttribute");
		CLAM::AttributePool pool;
		pool.SetDefinition(attribute);
		pool.Allocate(3);

		try
		{
			CLAM::XmlStorage::Restore(pool, input);
			CPPUNIT_FAIL("Should have thrown an exception");
		}
		catch (CLAM::ErrAssertionFailed & err)
		{
			const std::string expected = "Loading a attribute pool for a different attribute";
			CPPUNIT_ASSERT_EQUAL(expected, std::string(err.what()));
		}

		pool.Deallocate();
	}

	void testRestoreAttributePool_withNonDumpableData()
	{
		//TODO:
	}

	void testRestoreAttributePool_withNoData()
	{
		//TODO:
	}

	void testRestoreScopePool_withSingleAttribute()
	{
		std::istringstream input(
			"<ScopePool name=\"TestScope\" size=\"3\">"
			"<AttributePool name=\"MyAttribute\">"
				"value0 value1 value2"
			"</AttributePool>"
			"</ScopePool>");
		CLAM::DescriptionScope scope("TestScope");
		scope.Add<std::string>("MyAttribute");
		CLAM::ScopePool pool(scope);

		CLAM::XmlStorage::Restore(pool, input);

		CPPUNIT_ASSERT_EQUAL(3u, pool.GetSize());

		const std::string * data = pool.GetWritePool<std::string>("MyAttribute");
		const std::string value0("value0"), value1("value1"), value2("value2");
		CPPUNIT_ASSERT_EQUAL(value0, data[0]);
		CPPUNIT_ASSERT_EQUAL(value1, data[1]);
		CPPUNIT_ASSERT_EQUAL(value2, data[2]);
	}

	void testRestoreScopePool_withSeveralAttributes()
	{
		std::istringstream input(
			"<ScopePool name=\"TestScope\" size=\"3\">"
			"<AttributePool name=\"MyAttribute\">"
				"value0 value1 value2"
			"</AttributePool>"
			"<AttributePool name=\"MyOtherAttribute\">"
				"0 -1 -2"
			"</AttributePool>"
			"</ScopePool>");
		CLAM::DescriptionScope scope("TestScope");
		scope.Add<std::string>("MyAttribute");
		scope.Add<int>("MyOtherAttribute");
		CLAM::ScopePool pool(scope);

		CLAM::XmlStorage::Restore(pool, input);

		CPPUNIT_ASSERT_EQUAL(3u, pool.GetSize());
		{
			const std::string * data = pool.GetReadPool<std::string>("MyAttribute");
			const std::string value0("value0"), value1("value1"), value2("value2");
			CPPUNIT_ASSERT_EQUAL(value0, data[0]);
			CPPUNIT_ASSERT_EQUAL(value1, data[1]);
			CPPUNIT_ASSERT_EQUAL(value2, data[2]);
		}
		{
			const int * data = pool.GetReadPool<int>("MyOtherAttribute");
			for (unsigned int i = 0; i<3; i++)
				CPPUNIT_ASSERT_EQUAL((const int)-i, data[i]);
		}

	}

	void testRestoreScopePool_withBadScopeName()
	{
		CLAM::DescriptionScope scope("TestScope");
		scope.Add<std::string>("MyAttribute");
		CLAM::ScopePool pool(scope);

		std::istringstream input(
			"<ScopePool name=\"BadName\" size=\"3\">"
			"<AttributePool name=\"MyAttribute\">"
				"value0 value1 value2"
			"</AttributePool>"
			"</ScopePool>");
		try
		{
			CLAM::XmlStorage::Restore(pool, input);
			CPPUNIT_FAIL("Should have thrown an exception");
		}
		catch (CLAM::ErrAssertionFailed & err)
		{
			const std::string expected = "Loading an scope pool for a different attribute";
			CPPUNIT_ASSERT_EQUAL(expected, std::string(err.what()));
		}
	}

	void testDumpScopePool_withNoAttributes()
	{
		CLAM::DescriptionScope scope("TestScope");
		CLAM::ScopePool pool(scope,20);

		CLAM::XmlStorage::Dump(pool,"ScopePool",_targetStream);
		CPPUNIT_ASSERT_EQUAL(std::string(
			"<ScopePool name=\"TestScope\" size=\"20\"/>"
			),_targetStream.str());
	}
	void testDumpScopePool_withAttributesAndZeroSize()
	{
		CLAM::DescriptionScope scope("TestScope");
		scope.Add<std::string>("MyAttribute");
		CLAM::ScopePool pool(scope,0);

		CLAM::XmlStorage::Dump(pool,"ScopePool",_targetStream);
		CPPUNIT_ASSERT_EQUAL(std::string(
			"<ScopePool name=\"TestScope\" size=\"0\">"
			"<AttributePool name=\"MyAttribute\">"
			"</AttributePool>"
			"</ScopePool>"
			),_targetStream.str());
	}
	void testDumpScopePool_withAttributes()
	{
		CLAM::DescriptionScope scope("TestScope");
		scope.Add<std::string>("MyAttribute");
		CLAM::ScopePool pool(scope,3);
		std::string * values = pool.GetWritePool<std::string>("MyAttribute");
		values[0]="value0";
		values[1]="value1";
		values[2]="value2";

		CLAM::XmlStorage::Dump(pool,"ScopePool",_targetStream);
		CPPUNIT_ASSERT_EQUAL(std::string(
			"<ScopePool name=\"TestScope\" size=\"3\">"
			"<AttributePool name=\"MyAttribute\">"
				"value0 value1 value2"
			"</AttributePool>"
			"</ScopePool>"
			),_targetStream.str());
	}
	void testDumpScopePool_withIntegerAttributes()
	{
		CLAM::DescriptionScope scope("TestScope");
		scope.Add<unsigned>("MyAttribute");
		CLAM::ScopePool pool(scope,3);
		unsigned * values = pool.GetWritePool<unsigned>("MyAttribute");
		values[0]=1;
		values[1]=2;
		values[2]=3;

		CLAM::XmlStorage::Dump(pool,"ScopePool",_targetStream);
		CPPUNIT_ASSERT_EQUAL(std::string(
			"<ScopePool name=\"TestScope\" size=\"3\">"
			"<AttributePool name=\"MyAttribute\">"
				"1 2 3"
			"</AttributePool>"
			"</ScopePool>"
			),_targetStream.str());
	}
	void testDumpScopePool_withComponentAttributes()
	{
		CLAM::DescriptionScope scope("TestScope");
		scope.Add<DummyComponent>("MyAttribute");
		CLAM::ScopePool pool(scope,3);
		DummyComponent * values = pool.GetWritePool<DummyComponent>("MyAttribute");
		values[0].SetValue("value0");
		values[1].SetValue("value1");
		values[2].SetValue("value2");

		CLAM::XmlStorage::Dump(pool,"ScopePool",_targetStream);
		CPPUNIT_ASSERT_EQUAL(std::string(
			"<ScopePool name=\"TestScope\" size=\"3\">"
			"<AttributePool name=\"MyAttribute\">"
				"<DummyComponent DummyMember=\"value0\"/>"
				"<DummyComponent DummyMember=\"value1\"/>"
				"<DummyComponent DummyMember=\"value2\"/>"
			"</AttributePool>"
			"</ScopePool>"
			),_targetStream.str());
	}
	void testDumpScopePool_withNonInstantiatedAttributes()
	{
		CLAM::DescriptionScope scope("TestScope");
		scope.Add<DummyComponent>("MyAttribute");
		CLAM::ScopePool pool(scope,3);

		CLAM::XmlStorage::Dump(pool,"ScopePool",_targetStream);
		CPPUNIT_ASSERT_EQUAL(std::string(
			"<ScopePool name=\"TestScope\" size=\"3\"/>"
			),_targetStream.str());
	}
	void testDumpDescriptionDataPool_withAllKindsOfData()
	{
		CLAM::DescriptionScheme scheme;
		scheme.AddAttribute<DummyComponent>("TestScope1","DummyComponentAttribute");
		scheme.AddAttribute<unsigned>("TestScope2","UnsignedAttribute");
		scheme.AddAttribute<std::string>("TestScope2","StringAttribute");

		CLAM::DescriptionDataPool pool(scheme);
		pool.SetNumberOfContexts("TestScope1",3);
		pool.SetNumberOfContexts("TestScope2",4);
		{
			DummyComponent * values = pool.GetWritePool<DummyComponent>("TestScope1","DummyComponentAttribute");
			values[0].SetValue("value0");
			values[1].SetValue("value1");
			values[2].SetValue("value2");
		}
		{
			std::string * values = pool.GetWritePool<std::string>("TestScope2","StringAttribute");
			values[0]="value0";
			values[1]="value1";
			values[2]="value2";
			values[3]="value3";
		}
		{
			unsigned * values = pool.GetWritePool<unsigned>("TestScope2","UnsignedAttribute");
			values[0]=0;
			values[1]=1;
			values[2]=2;
			values[3]=3;
		}

		CLAM::XmlStorage::Dump(pool,"DescriptionData",_targetStream);
		CPPUNIT_ASSERT_EQUAL(std::string(
		"<DescriptionData>"
			"<ScopePool name=\"TestScope1\" size=\"3\">"
			"<AttributePool name=\"DummyComponentAttribute\">"
				"<DummyComponent DummyMember=\"value0\"/>"
				"<DummyComponent DummyMember=\"value1\"/>"
				"<DummyComponent DummyMember=\"value2\"/>"
			"</AttributePool>"
			"</ScopePool>"
			"<ScopePool name=\"TestScope2\" size=\"4\">"
			"<AttributePool name=\"UnsignedAttribute\">"
				"0 1 2 3"
			"</AttributePool>"
			"<AttributePool name=\"StringAttribute\">"
				"value0 value1 value2 value3"
			"</AttributePool>"
			"</ScopePool>"
		"</DescriptionData>"
			),_targetStream.str());
	}

	void testRestoreDescriptionDataPool_withAllKindsOfData()
	{
		CLAM::DescriptionScheme scheme;
		scheme.AddAttribute<DummyComponent>("TestScope1","DummyComponentAttribute");
		scheme.AddAttribute<unsigned>("TestScope2","UnsignedAttribute");
		scheme.AddAttribute<std::string>("TestScope2","StringAttribute");

		CLAM::DescriptionDataPool pool(scheme);

		std::istringstream input(
		"<DescriptionData>"
			"<ScopePool name=\"TestScope1\" size=\"3\">"
			"<AttributePool name=\"DummyComponentAttribute\">"
				"<DummyComponent DummyMember=\"value0\"/>"
				"<DummyComponent DummyMember=\"value1\"/>"
				"<DummyComponent DummyMember=\"value2\"/>"
			"</AttributePool>"
			"</ScopePool>"
			"<ScopePool name=\"TestScope2\" size=\"4\">"
			"<AttributePool name=\"UnsignedAttribute\">"
				"0 1 2 3"
			"</AttributePool>"
			"<AttributePool name=\"StringAttribute\">"
				"value0 value1 value2 value3"
			"</AttributePool>"
			"</ScopePool>"
		"</DescriptionData>");

		CLAM::XmlStorage::Restore(pool, input);

		CPPUNIT_ASSERT_EQUAL(3u,pool.GetNumberOfContexts("TestScope1"));
		CPPUNIT_ASSERT_EQUAL(4u,pool.GetNumberOfContexts("TestScope2"));
		{
			DummyComponent * data = pool.GetWritePool<DummyComponent>("TestScope1","DummyComponentAttribute");
			const std::string value0("value0"), value1("value1"), value2("value2");
			CPPUNIT_ASSERT_EQUAL(value0, data[0].GetValue());
			CPPUNIT_ASSERT_EQUAL(value1, data[1].GetValue());
			CPPUNIT_ASSERT_EQUAL(value2, data[2].GetValue());
		}
		{
			const std::string * data = pool.GetWritePool<std::string>("TestScope2","StringAttribute");
			const std::string value0("value0"), value1("value1"), value2("value2"), value3("value3");
			CPPUNIT_ASSERT_EQUAL(value0, data[0]);
			CPPUNIT_ASSERT_EQUAL(value1, data[1]);
			CPPUNIT_ASSERT_EQUAL(value2, data[2]);
			CPPUNIT_ASSERT_EQUAL(value3, data[3]);
		}
		{
			const unsigned * data = pool.GetWritePool<unsigned>("TestScope2","UnsignedAttribute");
			for (unsigned int i = 0; i<4; i++)
				CPPUNIT_ASSERT_EQUAL((const unsigned)i, data[i]);
		}

	}

};


} // namespace CLAMTest
