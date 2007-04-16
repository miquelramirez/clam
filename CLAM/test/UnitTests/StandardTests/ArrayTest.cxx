#include <cppunit/extensions/HelperMacros.h>
#include "cppUnitHelper.hxx"
#include "Array.hxx" // CLAM

namespace CLAMTest
{


class ArrayTest;

CPPUNIT_TEST_SUITE_REGISTRATION( ArrayTest );

class ArrayTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( ArrayTest );

	CPPUNIT_TEST( test_Array_Constructor_NonMemoryOwning_With_Null_Pointer );
	CPPUNIT_TEST( test_Array_Constructor_NonMemoryOwning_With_Valid_Pointer );

	CPPUNIT_TEST( test_Array_CopyOperator_MemoryOwning_Into_MemoryOwning );
	CPPUNIT_TEST( test_Array_CopyOperator_Being_The_Array_The_Placeholder_For_A_WriteStreamRegion );

	CPPUNIT_TEST( test_SetPtr_OnEmptyArray );
	CPPUNIT_TEST( test_TwoConsecutiveSetPtrs_OnEmptyArray );
	CPPUNIT_TEST( test_SetPtr_OnNonEmptyArray );

	CPPUNIT_TEST( test_Resize_To_Zero_Flushes_The_Array );
	CPPUNIT_TEST( test_Resize_To_Negative_Triggers_Assertion_Failure );

	CPPUNIT_TEST( test_GiveChunk_OutOfBounds_Chunk_Triggers_Assertion );
	CPPUNIT_TEST( test_CopyChunk_OutOfBounds_Chunk_Triggers_Assertion );
	CPPUNIT_TEST( test_CopyChunk_Legal_Chunk_On_Empty_Array_Triggers_Assertion );

	CPPUNIT_TEST( test_Add_Element_To_Beginning_Of_Array );
	CPPUNIT_TEST( test_Add_Element_To_End_Of_Array );
	CPPUNIT_TEST( test_Add_Element_To_Middle_Of_Array );
	
	CPPUNIT_TEST( test_Delete_Element_From_Beginning_Of_Array );
	CPPUNIT_TEST( test_Delete_Element_From_End_Of_Array );
	CPPUNIT_TEST( test_Delete_Element_From_Middle_Of_Array );

	CPPUNIT_TEST( test_Add_Element_To_Empty_Array );
	CPPUNIT_TEST( test_Insert_Element_Into_Empty_Array );
	
	CPPUNIT_TEST( test_Delete_Element_From_Empty_Array );
	
	CPPUNIT_TEST_SUITE_END();

public:
	/// Common initialization, executed before each test method
	void setUp() { }

	/// Common clean up, executed after each test method
	void tearDown() { }

private:
	
	void test_SetPtr_OnEmptyArray()
	{
		int buffer[] = { 1, 2, 3, 4};

		CLAM::Array<int> array;

		array.SetPtr( buffer, 4 );

		CPPUNIT_ASSERT( !array.OwnsMemory() );
		CPPUNIT_ASSERT( array.Size() == 4 );
		CPPUNIT_ASSERT( array.GetPtr() == buffer );
	}

	void test_TwoConsecutiveSetPtrs_OnEmptyArray()
	{
		int buffer[] = { 1, 2, 3, 4};

		CLAM::Array<int> array;

		array.SetPtr( buffer, 4 );

		int buffer2[] = { 1, 2, 3, 4, 5, 6 };

		array.SetPtr( buffer2, 6 );

		CPPUNIT_ASSERT( !array.OwnsMemory() );
		CPPUNIT_ASSERT( array.Size() == 6 );
		CPPUNIT_ASSERT( array.GetPtr() == buffer2 );

	}

	void test_SetPtr_OnNonEmptyArray()
	{
		int buffer[] = { 1, 2, 3, 4};

		CLAM::Array<int> array;
		array.Resize( 4 );
		array.SetSize( 4 );


		try
		{
			array.SetPtr( buffer, 4 );
		}
		catch( CLAM::ErrAssertionFailed& e )
		{
			return;
		}

		CPPUNIT_FAIL( "Array::SetPtr() on Memory Owning, non-empty Array must raise and assertion failure" );

	}

	void test_Array_Constructor_NonMemoryOwning_With_Valid_Pointer()
	{
		int buffer[] = { 1, 2, 3, 4};

		CLAM::Array<int> array( buffer, 4 );

		CPPUNIT_ASSERT( !array.OwnsMemory() );
		CPPUNIT_ASSERT( array.Size() == 4 );
		CPPUNIT_ASSERT( array.GetPtr() == buffer );

	}


	void test_Array_Constructor_NonMemoryOwning_With_Null_Pointer()
	{
		try
		{
			CLAM::Array<int> array( (int*)NULL, 0 );
		}
		catch( CLAM::ErrAssertionFailed& e )
		{
			return;
		}

		CPPUNIT_FAIL("Invoking the Array::Array( T*, int) constructor passing a "
			     "NULL pointer should have triggered an assertion failure!");
	}


	void test_Array_CopyOperator_MemoryOwning_Into_MemoryOwning()
	{
		CLAM::DataArray src;
		src.Resize( 10 );
		src.SetSize( 10 );

		for ( int i = 0; i < src.Size(); i++ )
			src[i] = i;

		CLAM::DataArray dst;
		dst.Resize( 10 );
		dst.SetSize( 10 );

		dst = src;

		bool sizeItsTheSame = ( src.Size() == dst.Size() );
		bool dstDoesOwnMemory = dst.OwnsMemory();
		bool allElemsEqual = true;
		
		for ( int i = 0; i < dst.Size(); i++ )
		{
			allElemsEqual &= ( dst[i] == i );
		}
		
		CPPUNIT_ASSERT( allElemsEqual );
		CPPUNIT_ASSERT( sizeItsTheSame );
		CPPUNIT_ASSERT( dstDoesOwnMemory );


	}

	void test_Array_CopyOperator_Being_The_Array_The_Placeholder_For_A_WriteStreamRegion()
	{

		CLAM::Array<int> streamBuffer;
		streamBuffer.Resize(4);
		streamBuffer.SetSize(4);

		streamBuffer[0] = 1;
		streamBuffer[1] = 2;
		streamBuffer[2] = 3;
		streamBuffer[3] = 4;
		


		CLAM::Array<int> array;		
		streamBuffer.GiveChunk( 1, 2, array );

		CLAM::Array<int> data;
		data.Resize(2);
		data.SetSize(2);

		array = data;


		CPPUNIT_ASSERT( streamBuffer[0] == 1 );
		CPPUNIT_ASSERT( streamBuffer[1] == 0 );
		CPPUNIT_ASSERT( streamBuffer[2] == 0 );
		CPPUNIT_ASSERT( streamBuffer[3] == 4 );

		
	}
	
	void test_Resize_To_Zero_Flushes_The_Array()
	{
		CLAM::Array<int> array;

		//Unused variable: const int* oldPointer = array.GetPtr();

		array.Resize( 0 );

		CPPUNIT_ASSERT( array.GetPtr() == NULL );
		CPPUNIT_ASSERT( array.Size() == 0 );
		CPPUNIT_ASSERT( array.AllocatedSize() == 0 );
		CPPUNIT_ASSERT( array.Empty() == true );
	}

	void test_Resize_To_Negative_Triggers_Assertion_Failure()
	{
	
		CLAM::Array<int> array;

		try
		{
			array.Resize( -150 );
		}
		catch( CLAM::ErrAssertionFailed& e )
		{
			return;
		}

		CPPUNIT_FAIL( "Array::Resize( int ) : with negative arguments should have triggered "
			      "an assertion failure.");
	}

	void test_GiveChunk_OutOfBounds_Chunk_Triggers_Assertion()
	{
		CLAM::Array<int> array;
		array.Resize(10);
		array.SetSize( 10 );

		CLAM::Array<int> chunkReceiver;

		try
		{
			array.GiveChunk( 5, 25, chunkReceiver );
		} 
		catch( CLAM::ErrAssertionFailed& e )
		{
			return;
		}

		CPPUNIT_FAIL( "Array::GiveChunk() : when chunk offset plus length surpasses array logical size "
			      "an assertion failure should be raised.");

	}

	void test_CopyChunk_OutOfBounds_Chunk_Triggers_Assertion()
	{
		CLAM::Array<int> array;
		array.Resize(10);
		array.SetSize( 10 );

		CLAM::Array<int> chunkReceiver;
		chunkReceiver.Resize( 20 );
		chunkReceiver.SetSize( 20 );

		try
		{
			array.CopyChunk( 5, 25, chunkReceiver );
		} 
		catch( CLAM::ErrAssertionFailed& e )
		{
			return;
		}

		CPPUNIT_FAIL( "Array::CopyChunk() : when chunk offset plus length surpasses array logical size "
			      "an assertion failure should be raised.");
	}

	void test_CopyChunk_Legal_Chunk_On_Empty_Array_Triggers_Assertion()
	{
		CLAM::Array<int> array;
		array.Resize(10);
		array.SetSize( 10 );

		CLAM::Array<int> chunkReceiver;

		try
		{
			array.CopyChunk( 5, 25, chunkReceiver );
		} 
		catch( CLAM::ErrAssertionFailed& e )
		{
			return;
		}

		CPPUNIT_FAIL( "Array::CopyChunk() : destination array is expected to own ( or point to ) "
			      "a large enough pool of memory, but no assertion failure has been raised ");
		
	}

	void test_Add_Element_To_Beginning_Of_Array()
	{
		CLAM::Array<int> testArray;
		testArray.Resize( 4 );
		testArray.SetSize( 4 );

		testArray[0] = 1; testArray[1] = 2; testArray[2] = 3; testArray[3] = 4;

		testArray.InsertElem( 0, 0 );

		CPPUNIT_ASSERT( testArray.Size() == 5 );
		CPPUNIT_ASSERT( testArray.AllocatedSize() == 5 );
		CPPUNIT_ASSERT( testArray.OwnsMemory() == true );
		CPPUNIT_ASSERT( testArray[0] == 0 );
		CPPUNIT_ASSERT( testArray[1] == 1 );
		CPPUNIT_ASSERT( testArray[2] == 2 );
		CPPUNIT_ASSERT( testArray[3] == 3 );
		CPPUNIT_ASSERT( testArray[4] == 4 );

	}

	void test_Add_Element_To_End_Of_Array()
	{
		CLAM::Array<int> testArray;
		testArray.Resize( 4 );
		testArray.SetSize( 4 );

		testArray[0] = 1; testArray[1] = 2; testArray[2] = 3; testArray[3] = 4;

		testArray.AddElem( 5 );

		CPPUNIT_ASSERT( testArray.Size() == 5 );
		CPPUNIT_ASSERT( testArray.AllocatedSize() == 5 );
		CPPUNIT_ASSERT( testArray.OwnsMemory() == true );
		CPPUNIT_ASSERT( testArray[0] == 1 );
		CPPUNIT_ASSERT( testArray[1] == 2 );
		CPPUNIT_ASSERT( testArray[2] == 3 );
		CPPUNIT_ASSERT( testArray[3] == 4 );
		CPPUNIT_ASSERT( testArray[4] == 5 );

		
	}

	void test_Add_Element_To_Middle_Of_Array()
	{
		CLAM::Array<int> testArray;
		testArray.Resize( 4 );
		testArray.SetSize( 4 );

		testArray[0] = 1; testArray[1] = 3; testArray[2] = 4; testArray[3] = 5;


		testArray.InsertElem( 1, 2 );

		CPPUNIT_ASSERT( testArray.Size() == 5 );
		CPPUNIT_ASSERT( testArray.AllocatedSize() == 5 );
		CPPUNIT_ASSERT( testArray.OwnsMemory() == true );
		CPPUNIT_ASSERT( testArray[0] == 1 );
		CPPUNIT_ASSERT( testArray[1] == 2 );
		CPPUNIT_ASSERT( testArray[2] == 3 );
		CPPUNIT_ASSERT( testArray[3] == 4 );
		CPPUNIT_ASSERT( testArray[4] == 5 );

		
	}

	void test_Delete_Element_From_Beginning_Of_Array()
	{
		CLAM::Array<int> testArray;
		testArray.Resize( 4 );
		testArray.SetSize( 4 );

		testArray[0] = 1; testArray[1] = 2; testArray[2] = 3; testArray[3] = 4;

		const int* oldPtr = testArray.GetPtr();

		testArray.DeleteElem( 0 );

		CPPUNIT_ASSERT( oldPtr == testArray.GetPtr() );
		CPPUNIT_ASSERT( testArray.Size() == 3 );
		CPPUNIT_ASSERT( testArray.AllocatedSize() == 4 );
		CPPUNIT_ASSERT( testArray.OwnsMemory() == true );
		CPPUNIT_ASSERT( testArray[0] == 2 );
		CPPUNIT_ASSERT( testArray[1] == 3 );
		CPPUNIT_ASSERT( testArray[2] == 4 );

		
	}

	void test_Delete_Element_From_End_Of_Array()
	{
		CLAM::Array<int> testArray;
		testArray.Resize( 4 );
		testArray.SetSize( 4 );

		testArray[0] = 1; testArray[1] = 2; testArray[2] = 3; testArray[3] = 4;

		const int* oldPtr = testArray.GetPtr();
		
		testArray.DeleteElem( testArray.Size() - 1 );

		CPPUNIT_ASSERT( testArray.GetPtr() == oldPtr );
		CPPUNIT_ASSERT( testArray.Size() == 3 );
		CPPUNIT_ASSERT( testArray.AllocatedSize() == 4 );
		CPPUNIT_ASSERT( testArray.OwnsMemory() == true );
		CPPUNIT_ASSERT( testArray[0] == 1 );
		CPPUNIT_ASSERT( testArray[1] == 2 );
		CPPUNIT_ASSERT( testArray[2] == 3 );

	}

	void test_Delete_Element_From_Middle_Of_Array()
	{
		CLAM::Array<int> testArray;
		testArray.Resize( 4 );
		testArray.SetSize( 4 );

		testArray[0] = 1; testArray[1] = 2; testArray[2] = 3; testArray[3] = 4;

		const int* oldPtr = testArray.GetPtr();

		testArray.DeleteElem( 1 );

		CPPUNIT_ASSERT( testArray.GetPtr() == oldPtr );
		CPPUNIT_ASSERT( testArray.Size() == 3 );
		CPPUNIT_ASSERT( testArray.AllocatedSize() == 4 );
		CPPUNIT_ASSERT( testArray.OwnsMemory() == true );
		CPPUNIT_ASSERT( testArray[0] == 1 );
		CPPUNIT_ASSERT( testArray[1] == 3 );
		CPPUNIT_ASSERT( testArray[2] == 4 );

	}

	void test_Add_Element_To_Empty_Array()
	{
		CLAM::Array<int> testArray;

		testArray.AddElem( 33 );

		CPPUNIT_ASSERT( testArray[0] == 33 );
		CPPUNIT_ASSERT( testArray.Empty() == false );
		CPPUNIT_ASSERT( testArray.Size() == 1 );
		CPPUNIT_ASSERT( testArray.AllocatedSize() == 1 );
		CPPUNIT_ASSERT( testArray.OwnsMemory() == true );
	}

	void test_Insert_Element_Into_Empty_Array()
	{
		CLAM::Array<int> testArray;


		try
		{
			testArray.InsertElem( 0, 33 );
		}
		catch( CLAM::ErrAssertionFailed& e )
		{
			return;
		}

		CPPUNIT_FAIL( "Array::InsertElem() : This method cannot be used on empty arrays, so an assertion "
			      "should have been raised");
	}

	void test_Delete_Element_From_Empty_Array()
	{
		CLAM::Array<int> testArray;

		try
		{
			testArray.DeleteElem( 0 );
		}
		catch( CLAM::ErrAssertionFailed& e )
		{
			return;
		}
		
		CPPUNIT_FAIL( "Array::DeleteElem() : deleting an element from an empty array should fail " );

	}

};






} // namespace CLAMTest

