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

#include "ProcessingComposite.hxx"
#include "TopLevelProcessing.hxx"
#include <iostream>
#include "mtgsstream.h"
#include "Processing.hxx"

using CLAM::Err;
using CLAM::Processing;
using CLAM::ProcessingConfig;
using CLAM::ProcessingComposite;
using CLAM::DynamicType;
using CLAM::TopLevelProcessing;
using CLAM::TopLevelProcessingConfig;

using namespace std;

namespace CLAMTest {

	class POCompositeTest {

	public:
		// Test classes.
		class LeafConfig : public ProcessingConfig {
		public:
			DYNAMIC_TYPE_USING_INTERFACE (LeafConfig, 1, ProcessingConfig);
			DYN_ATTRIBUTE (0, public, std::string, Name);
		private:
			void DefaultInit(void);
		};
		class CompositeConfig : public ProcessingConfig {
		public:
			DYNAMIC_TYPE_USING_INTERFACE (CompositeConfig, 2, ProcessingConfig);
			DYN_ATTRIBUTE (0, public, std::string, Name);
			DYN_ATTRIBUTE (1, public, int, NElements);
		public:
			void DefaultInit(void);
		};

		// Unnamed processing object.
		class Leaf1 : public Processing {
			LeafConfig mConfig;
		protected:
			bool ConcreteConfigure(const ProcessingConfig&);
		public:
			Leaf1();
			Leaf1(LeafConfig&);
			const char * GetClassName() const {return "CLAMTest_Leaf1";}
			const ProcessingConfig &GetConfig() const {return mConfig;}
			bool Do(void) {return true;}
		};

		// Named processing object.
		class Leaf2 : public Processing {
			LeafConfig mConfig;
		protected:
			bool ConcreteConfigure(const ProcessingConfig&);
		public:
			Leaf2();
			Leaf2(LeafConfig&);
			const char * GetClassName() const {return "CLAMTest_Leaf2";}
			const ProcessingConfig &GetConfig() const {return mConfig;}
			bool Do(void) {return true;}
		};

		// Named composite object.
		class Composite : public ProcessingComposite {
			CompositeConfig mConfig;
			int elems;
			bool next_is_named;
			bool next_is_leaf1;
			bool next_is_reconfigured;
			std::string NewName();
			Processing* CreateNewLeafChild();
			ProcessingComposite *CreateNewCompositeChild();
		protected:
			bool ConcreteConfigure(const ProcessingConfig&);
		public:
			Composite(CompositeConfig&);
			const char * GetClassName() const {return "CLAMTest_Composite";}
			const ProcessingConfig &GetConfig() const {return mConfig;}
			bool Do(void) {return true;}
		};

		// Test methods.
		int TestComposite();
		int TestTopLevel();
		int TestNames();
		int TestExceptions();
		void PrintPOComposite(ProcessingComposite &);
	public:

		int Do();

	};
}

int main()
{
	int res;
	try {
		CLAMTest::POCompositeTest test;
		res = test.Do();
		if (res)
			cout << "Failed." << endl;
		else
			cout << "Passed." << endl;
		return res;
	}
	catch(Err e) {
		e.Print();
	}
	catch(...) {
		cout << "Failed: Uncaugh exception." << endl;
		return 1;
	}
}


// IMPLEMENTATION
namespace CLAMTest {

	void POCompositeTest::LeafConfig::DefaultInit(void)
	{ 
		AddName(); 
		UpdateData(); 
	}
	void POCompositeTest::CompositeConfig::DefaultInit(void)
	{ 
		AddName(); 
		AddNElements();
		UpdateData(); 
		SetNElements(0);
	}

	POCompositeTest::Leaf1::Leaf1()
	{
		Configure(LeafConfig());
	}
	POCompositeTest::Leaf1::Leaf1(LeafConfig& cfg)
	{
		Configure(cfg);
	}
	bool POCompositeTest::Leaf1::
	ConcreteConfigure(const ProcessingConfig& c)
	{
		CopyAsConcreteConfig(mConfig, c);
		return true;
	}
	POCompositeTest::Leaf2::Leaf2()
	{
		Configure(LeafConfig());
	}
	POCompositeTest::Leaf2::Leaf2(LeafConfig& cfg)
	{
		Configure(cfg);
	}
	bool POCompositeTest::Leaf2::
	ConcreteConfigure(const ProcessingConfig& c)
	{
		CopyAsConcreteConfig(mConfig, c);
		return true;
	}
	bool POCompositeTest::Composite::
	ConcreteConfigure(const ProcessingConfig& c)
	{
		CopyAsConcreteConfig(mConfig, c);
		Processing *child;
		int e = mConfig.GetNElements();

		if (e==0)
			return true;

		while (elems < e-1) {
			child = CreateNewLeafChild();
			child->SetParent(this);
			elems++;
		}
		child = CreateNewCompositeChild();
		child->SetParent(this);
		elems++;
		return true;
	}

	POCompositeTest::Composite::Composite(CompositeConfig &c)
		:elems(0),
		 next_is_named(false),
		 next_is_leaf1(false),
		 next_is_reconfigured(false)
	{
		Configure(c);
	}

	std::string POCompositeTest::Composite::NewName()
	{
		stringstream str;
		str << "Child_" << elems;
		return str.str();
	}

	Processing* POCompositeTest::Composite::CreateNewLeafChild()
	{
		Processing *leaf;
		LeafConfig cfg;

		if (next_is_named)
			cfg.SetName(NewName());

		if (next_is_leaf1)
			if (next_is_reconfigured) {
				leaf = new Leaf1;
				leaf->Configure(cfg);
			}
			else
				leaf = new Leaf1(cfg);
		else {
			if (next_is_reconfigured) {
				leaf = new Leaf2;
				leaf->Configure(cfg);
			}
			else
				leaf = new Leaf2(cfg);
		}

		if (next_is_named)
			next_is_reconfigured = !next_is_reconfigured;
		if (next_is_leaf1)
			next_is_named = !next_is_named;
		next_is_leaf1 = !next_is_leaf1;
		return leaf;
	}

	ProcessingComposite *POCompositeTest::Composite::CreateNewCompositeChild()
	{
		ProcessingComposite *compo;
		CompositeConfig cfg;

		if (next_is_named)
			cfg.SetName(NewName());

		cfg.SetNElements(mConfig.GetNElements()-1);
		
		compo = new Composite(cfg);

		return compo;
	}

	int POCompositeTest::TestComposite()
	{
		CompositeConfig cfg;

		cfg.SetName("TestComposite");

		cfg.SetNElements(20);
		
		Composite compo(cfg);

		PrintPOComposite(compo);

		return 0;
	}

	int POCompositeTest::TestTopLevel()
	{
		TopLevelProcessing &top = TopLevelProcessing::GetInstance();
		{
			Leaf1 leaf1,leaf2,leaf3;
			Leaf2 leaf4,leaf5,leaf6;
			PrintPOComposite(top);
		}
		PrintPOComposite(top);
		{
			Leaf1 leaf1,leaf2,leaf3;
			Leaf2 leaf4,leaf5,leaf6;
			PrintPOComposite(top);
		}
		PrintPOComposite(top);

		return 0;
	}

	int POCompositeTest::TestNames()
	{
		return 0;
	}

	int POCompositeTest::TestExceptions()
	{
		return 0;
	}




	int POCompositeTest::Do()
	{
		int res;

		res = TestTopLevel();
		if (res)
			return res;
		res = TestComposite();
		if (res)
			return res;
		res = TestNames();
		if (res)
			return res;
		res = TestExceptions();
		if (res)
			return res;
		return 0;
	}

	void POCompositeTest::PrintPOComposite(ProcessingComposite &c)
	{
		ProcessingComposite::iterator it;
		ProcessingComposite *compo;
		for (it=c.composite_begin(); it != c.composite_end(); it++) {
			cout << (*it)->GetFullName() << endl;
			compo = dynamic_cast<ProcessingComposite*> (*it);
			if (compo)
				PrintPOComposite(*compo);
		}
	}

}
