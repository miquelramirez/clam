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

#include "List.hxx"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <time.h>
#include "Assert.hxx"
#include "XMLTestHelper.hxx"


#define NITERATIONS 2000

namespace CLAMTest {

using namespace CLAM;


template <class T> bool CorrectSequentialValues(const List<T>& myList)
{
	CLAM_ASSERT(myList.FulfillsInvariant(),"List does not Fulfill Invariant");
	if((!myList.IsEmpty())&&(myList.CurrentIndex()!=myList.Current()||myList.First()!=0||
	   myList.Last()!=myList.Size()-1)) 
		return false;
	else return true;

}

}

using CLAM::Err;

int main()
{

try{
	CLAM::List<int> testList;
	int testElem;

/*Sequential additions*/
	if(testList.IsEmpty())
		std::cout<<"List is empty, OK!";
	else
		std::cout<<"List is not empty, Error!";

	std::cout<<"Sequential Additions\n";
	for(int i=0;i<NITERATIONS;i++)
	{
		testList.AddElem(i);
		std::cout<<"Current Element is "<<testList.CurrentIndex()<<" has a value of "<<testList.Current()<<"\n";
		CLAM_ASSERT(testList.FulfillsInvariant(), "List does not fullfil invariant");
		CLAM_ASSERT(CLAMTest::CorrectSequentialValues(testList),"List does not have correct Values");
	}

	std::cout<<"Sequential Additions Finished Successfully, starting Sequential Extractions \n";
	/*std::cout<<"Press enter when ready\n";
	std::cin>>c;*/
/*Sequential Extractions*/
	for(int i=0;i<NITERATIONS;i++)
	{
		testList.Extract(testElem);
		std::cout<<"Element in position "<<NITERATIONS-i-1<<" has a value of "<<testElem<<"\n";
		CLAM_ASSERT(testList.FulfillsInvariant(), "List does not fullfil invariant");
		CLAM_ASSERT(CLAMTest::CorrectSequentialValues(testList),"List does not have correct Values");
		if(testList.CurrentIndex()>=0)
			std::cout<<"Current Element is "<<testList.CurrentIndex()<<" has a value of "<<testList.Current()<<"\n";
		else{
			if(i==NITERATIONS-1)
				std::cout<<"List is empty, OK!\n";
			else
				throw Err("List is not empty, ERROR!");}

	}
	if(testList.IsEmpty())
		std::cout<<"List is empty, OK!\n";
	else
		std::cout<<"List is not empty, Error!\n";

	if(testList.IsEmpty())
		std::cout<<"List is empty, OK!\n";
	else
		throw Err("List is not empty, ERROR!");;
	srand( (unsigned)time( NULL ) );
	//srand(999);
	std::cout<<"Sequential Extractions Finished Successfully, Filling list again\n";
	/*std::cout<<"Press enter when ready\n";
	std::cin>>c;
	std::cout<<"Random Value Additions Finished Successfully, starting Random Isertions\n";
	std::cout<<"Press enter when ready\n";
	std::cin>>c;*/

/*Additions*/
	for(int i=0;i<NITERATIONS;i++)
	{
		testList.AddElem(i);
		std::cout<<"Current Element is "<<testList.CurrentIndex()<<" has a value of "<<testList.Current()<<"\n";
		CLAM_ASSERT(testList.FulfillsInvariant(), "List does not fullfil invariant");
		CLAM_ASSERT(CLAMTest::CorrectSequentialValues(testList),"List does not have correct Values");
	}

	std::cout<<"Additions Finished Successfully, starting Random Insertions\n";

/*Random Accesses*/
	int current;
	for(int i=0;i<NITERATIONS;i++)
	{
		current=int((double(rand())/RAND_MAX)*(2*NITERATIONS));
		std::cout<<"Element at position "<<current<<" has a value of "<<testList[current]<<"\n";;
		std::cout.flush();
		CLAM_ASSERT(testList.FulfillsInvariant(), "List does not fullfil invariant");
		CLAM_ASSERT(CLAMTest::CorrectSequentialValues(testList),"List does not have correct Values");
		if(testList.CurrentIndex()>=0)
			std::cout<<"Current Element is "<<testList.CurrentIndex()<<" has a value of "<<testList.Current()<<"\n";
		else throw Err("List is not empty, ERROR!");

	}
	CLAM_ASSERT(testList.FulfillsInvariant(), "List does not fullfil invariant");
	std::cout<<"Random Accesses Finished Successfully, starting Random Extractions\n";
/*Random Insertions*/
	for(int i=0;i<NITERATIONS;i++)
	{
		current=int((double(rand())/RAND_MAX)*(NITERATIONS));
		CLAM_ASSERT(testList.FulfillsInvariant(), "List does not fullfil invariant");
		testList.InsertElem(current,current);
		CLAM_ASSERT(testList.FulfillsInvariant(), "List does not fullfil invariant");
		std::cout<<"Current Element is "<<testList.CurrentIndex()<<" has a value of "<<testList.Current()<<"\n";
		std::cout.flush();


	}
	CLAM_ASSERT(testList.FulfillsInvariant(), "List does not fullfil invariant");
	std::cout<<"Random Insertions Finished Successfully, starting Random Accesses\n";
//	std::cout<<"Press enter when ready\n";
//	std::cin>>c;

/*Random Accesses*/
	for(int i=0;i<NITERATIONS;i++)
	{
		current=int((double(rand())/RAND_MAX)*(2*NITERATIONS));
		std::cout<<"Element at position "<<current<<" has a value of "<<testList[current]<<"\n";;
		std::cout.flush();
		CLAM_ASSERT(testList.FulfillsInvariant(), "List does not fullfil invariant");
		if(testList.CurrentIndex()>=0)
			std::cout<<"Current Element is "<<testList.CurrentIndex()<<" has a value of "<<testList.Current()<<"\n";
		else throw Err("List is not empty, ERROR!");

	}
	CLAM_ASSERT(testList.FulfillsInvariant(), "List does not fullfil invariant");
	std::cout<<"Random Accesses Finished Successfully, starting Random Extractions\n";
	/*std::cout<<"Press enter when ready\n";
	std::cin>>c;*/

/* Copy */
	List<int> copyList, *copyList2;
	copyList=testList;
	CLAM_ASSERT(copyList.FulfillsInvariant(), "List does not fullfil invariant");
	copyList2=new List<int>(testList);
	CLAM_ASSERT(copyList2->FulfillsInvariant(), "List does not fullfil invariant");
	delete copyList2;

	// Storing result to XML document and check if loading matches
	CLAM_ASSERT(CLAMTest::XMLInputOutputMatches(testList, "ListTest.xml"), 
		"Original resulting list and XML reloaded one don't match");

/*Random Extractions*/
	for(int i=0;i<2*NITERATIONS;i++)
	{
		current=int((double(rand())/RAND_MAX)*(2*NITERATIONS-i-1));
		testList.Extract(testElem,current);
		std::cout<<"Element in position "<<current<<" has a value of "<<testElem<<"\n";
		std::cout.flush();
		CLAM_ASSERT(testList.FulfillsInvariant(), "List does not fullfil invariant");
		if(testList.CurrentIndex()>=0)
			std::cout<<"Current Element is "<<testList.CurrentIndex()<<" has a value of "<<testList.Current()<<"\n";
		else{
			if(i==2*NITERATIONS-1)
				std::cout<<"List is empty, OK!\n";
			else
				throw Err("List is not empty, ERROR!");}

	}
	CLAM_ASSERT(testList.FulfillsInvariant(), "List does not fullfil invariant");
	if(testList.IsEmpty()){
		std::cout<<"List is empty, OK!\n";
		std::cout<<"Test finished Successfully!!\n";
/*		std::cout<<"Press enter to finish\n";
		std::cin>>c;*/}
	else
		throw Err("List is not empty, ERROR!");

}
catch(Err err) 
{
	err.Print();
}

return 0;

}

