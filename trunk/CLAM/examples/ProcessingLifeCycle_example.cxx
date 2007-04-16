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
#include <ctime>          // C++ version of good old <time.h>
#include <cstdlib>        // C++ version of good old <stdlib.h>
#include "Err.hxx"        // imports CLAM exception class declaration
#include "Processing.hxx" // imports Processing abstract class declaration
#include "Audio.hxx"      // imports the Audio class declaration
#include "CLAM_Math.hxx"

// Helper function used below
static inline CLAM::TData sign( CLAM::TData value )
{
	return ( value > 0.0 ) ? 1.0 : -1.0;
}

// In this example we will see two very important things if you want to
// develop with CLAM:
// 1) How to create a Processing of your own, thus extending CLAM framework
// 2) Learn the meaning and purpose of the Configure(), Start() and Stop()
//    Processing class methods

// If you haven't yet read the available documentation that explains the basics
// of the Processing objects interface as well as gives some notes on writing your own
// Processing's I recommend you to read it, so you can follow this example
// easily. If you don't feel like reading, be warned, snarks and long-toothed 
// white rabbits lurk around!

// For each Processing subclass a corresponding configuration class must exist. 
// As a 'dummy processing' is a subclass of Processing, the configuration class
// for such a 'dummy processing' will be a subclass of CLAM::ProcessingConfig.

class DummyProcessingConfig : public CLAM::ProcessingConfig
{
public:
	// If you take a look at ProcessingConfig.hxx, you will see that it is a DynamicType.
	// DynamicTypes are well explained in their own section of the CLAM manual, so here
	// we will just explain in a shallow way what we are doing.
	// Since DynamicTypes are quite special sort of objects, inheritance is handled in
	// a different way that C++ standard, non-dynamic, types.
	// For subclassing a given DynamicType you must use the following macro, where
	// the first parameter is the name of the concrete class, the second parameter
	// the number of attributes it will have, and finally, the name of the base class.
	DYNAMIC_TYPE_USING_INTERFACE( DummyProcessingConfig, 1, CLAM::ProcessingConfig );
	// The second attribute is just a floating-point 'magic' number
	DYN_ATTRIBUTE( 0, public, CLAM::TData, MagicNumber );

	// Another rather peculiar thing about DynamicTypes is that you can't write a constructor
	// for them, as you would do for normal C++ classes. This is because the DynamicType macros
	// already 'expand' two constructors - the default constructor and the copy constructor. 
	// Macro-generated default constructor will call a method named 'DefaultInit': as its name
	// implies, that method is meant to provide you with an entry-point for adding the necessary
	// initialization routines. Note that it is *not* mandatory to declare/define this method.
	void DefaultInit();
};

void DummyProcessingConfig::DefaultInit()
{
	// We just add all attributes, update the object and set
	// the name and magic number values to some default. For some info
	// on the meaning of AddAll() and UpdateData(), refer to DynamicTypes
	// documentation on CLAM manual.
	AddAll();
	UpdateData();
	SetMagicNumber( 0.5 );
}

// Writing a new Processing subclass can be broken down on the following tasks:
// 1) Define a ProcessingConfig subclass matching the new Processing
// 2) Implement the abstract ( pure virtual ) methods found on the Processing interface
// 3) Write the Processing Do's 

class DummyProcessing : public CLAM::Processing
{
public: // methods
	DummyProcessing();
	virtual ~DummyProcessing();
	
	// Supervised mode Do method
	virtual bool Do();
	// Non-supervised mode Do method
	virtual bool Do( const CLAM::Audio& inputSignal,
			 CLAM::Audio& outputSignal );

	// Required accessor to current Processing object configuration
	virtual const CLAM::ProcessingConfig& GetConfig() const;
	
	// Required accessor to the class name for objects of the DummyProcessing type
	virtual const char* GetClassName() const;

protected: // methods
	
	// These methods will be called by Processing::Configure, Processing::Start
	// and Processing::Stop. This may seem strange: why not directly override the
	// former methods? Because Processing::Configure, etc. contain behaviour that
	// is general for any Processing subclass. So you, in order to specify the
	// behaviour for Processing subclasses, just override these methods. This 'idiom'
	// is called by some people the "Holywood Principle" (don't call us, 
	// we'll call you): in contrast with a library, the methods you define to use 
	// the framework get called by the framework 'to do a hollywood'. This way, the
	// generic behaviour defined in the superclass is completely transparent to you.
	// Other examples of the 'Hollywood principle' are C library callbacks, such as GLUT 
	// or standard C library quicksort() function.
	virtual bool ConcreteConfigure( const CLAM::ProcessingConfig& configObject );
	// However, the ConcreteStart and ConcreteStop are not abstract: but we will define them
	// just to see what sort of uses we can find for them.
	virtual bool ConcreteStart();
	virtual bool ConcreteStop();
	
private: // attributes
	DummyProcessingConfig mCurrentConfig;
	CLAM::TData           mMagicNumber;
	int*                  mpDummyBuffer;
	CLAM::TSize           mSamplesProcessed;
};

// Default constructor
DummyProcessing::DummyProcessing()
	: mMagicNumber( 0.33 ), mpDummyBuffer( NULL ), mSamplesProcessed( 0 )
{
}

// Destructor
DummyProcessing::~DummyProcessing()
{
	if ( mpDummyBuffer != NULL )
		delete [] mpDummyBuffer;
}

// Note that this method is 'void'. Refer to examples about Supervised Mode CLAM 
// and you will see what must be done here.
bool DummyProcessing::Do()
{
	return true;
}

// Quite simple implementation for this one...
const CLAM::ProcessingConfig& DummyProcessing::GetConfig() const
{
	return mCurrentConfig;
}

// Here comes the 'heart' of the processing: the Do method
bool DummyProcessing::Do( const CLAM::Audio& inputSignal,
			  CLAM::Audio& outputSignal )
{
	// First we check if the object was Start()'ed before
	// calling the method
	if ( !AbleToExecute() )
		return false;

	// We get the buffer of samples of the input audio object. Note
	// that CLAM::DataArray is just a very efficient 'growing-array'
	// implementation - similar in intent to std::vector.
	const CLAM::DataArray& inSamples = inputSignal.GetBuffer();

	// We check the output buffer size. Since we do not want to
	// corrupt any memory, we resize the output buffer when necessary
	if ( outputSignal.GetSize() < inputSignal.GetSize() )
		outputSignal.SetSize( inputSignal.GetSize() );

	// Same as before, but for the output buffer
	CLAM::DataArray& outSamples = outputSignal.GetBuffer();

	// Signal processing loop. This, altough can seem quite complicated,
	// just clamps the input signal to the range [ -mMagicNumber, mMagicNumber ]
	for ( int i = 0; i < inSamples.Size(); i++ )
		outSamples[i] = 
			fabs( inSamples[i] ) > mMagicNumber ? 
			sign(inSamples[i])*mMagicNumber : inSamples[i];					

	// We count the samples processed so far
	mSamplesProcessed += inputSignal.GetSize();

	return true;
}

// Another trivial method...
const char* DummyProcessing::GetClassName() const
{
	return "DummyProcessing";
}

bool DummyProcessing::ConcreteConfigure( const CLAM::ProcessingConfig& configObject )
{
	// This method must be called first of all, and just consists in 'downcasting'
	// the reference to ProcessingConfig object to the type of the first parameter.
	// If types are related by inheritance, then mCurrentConfig will be an exact
	// copy of the referenced object.
	CopyAsConcreteConfig( mCurrentConfig, configObject );

	// We store the values present in the configuration
	mMagicNumber = mCurrentConfig.GetMagicNumber();

	return true;
}

bool DummyProcessing::ConcreteStart()
{
	// Here we see of the possible, specific uses for ConcreteStart(): internal
	// data structures initialization, bookkeeping initialization, etc.

	if ( mMagicNumber <= 0.5 )
		mpDummyBuffer = new int[ 512 ];
	else
		mpDummyBuffer = new int[ 1024 ];

	mSamplesProcessed = 0;

	return true;
}

bool DummyProcessing::ConcreteStop()
{
	// As you can see ConcreteStop should be symmetric with respect ConcreteStart: what
	// you do there, you 'undo' it here

	if ( mpDummyBuffer != NULL )
		delete [] mpDummyBuffer;

	std::cout << "Total samples processed: " << mSamplesProcessed << std::endl;

	return true;
}

int main( int argc, char** argv )
{
	try
	{
		// We seed the C standard library pseudo-random number generator
		srand( time( NULL ) );
		
		// We define the input and output audio objects for the dummy Processing
		CLAM::Audio in, out;
		
		// We set the input size and sample rate
		in.SetSize( 2048 );
		in.SetSampleRate( 8000 );

		// 'Synthesis' loop for the input signal. Just some noise...
		for ( CLAM::TIndex i = 0; i < in.GetSize(); i++ )
			in.GetBuffer()[i] = (CLAM::TData)rand()/(CLAM::TData)RAND_MAX;

		// We instantiate the configuration object and initialize
		// its values to whatever we may fancy
		DummyProcessingConfig cfg;
		cfg.SetMagicNumber( 0.52 );

		// We instantiate the processing
		DummyProcessing procObj;

		// And now, let's see what happens if we try to call procObj Do method

		// First uncomment the following line, compile and execute
		// procObj.Do( in, out );
		
		// Nice message, isn't it? Now comment the previous line, and uncomment the
		// following two
		// procObj.Start();
		// procObj.Do( in, out );
		
		// Well, it's about time to get something out of this example, isn't it?
		// Then comment the two lines you uncommented last, and uncomment the following
		// lines:
		// procObj.Configure( cfg );
		// procObj.Start();
		// procObj.Do( in, out );
		// procObj.Stop();

		// And you will be wondering by now when you can Configure() a Processing. It will
		// be easier to explain this, showing when you *cannot* do so
		// Uncomment the following lines, compile, link and execute
		// procObj.Start();
		// procObj.Configure( cfg );
		// procObj.Do( in, out );
		// procObj.Stop();
		
		// and uncomment, compile, link and run
		// procObj.Start();
		// procObj.Do( in, out );
		// procObj.Configure( cfg );
		// procObj.Stop();

		// In a nutshell, you cannot Configure() a Processing if it has
		// been previously started ( when you instantiate a Processing it is 'stopped' ). Also, you
		// should have noticed that you cannot call a Processing Do() method before it is Configure()'d
		// or Start()'ed, or after it has been Stop()'ped. 

	}
	catch( CLAM::Err& e )
	{
		e.Print();
		exit(-1);
	}
	catch( std::exception& e )
	{
		std::cerr << e.what() << std::endl;
		exit( -1 );
	}

	return 0;
}

