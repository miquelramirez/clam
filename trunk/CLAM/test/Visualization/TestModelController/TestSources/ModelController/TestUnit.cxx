#include <Err.hxx>
#include <Assert.hxx>
#include <exception>
#include <iostream>
#include "TestCases.hxx"

void PrintSuccessMessage( const char* tcname )
{
	std::cout << tcname << " passed" << std::endl;
}

void DecorateException( const char* tcname, CLAM::ErrAssertionFailed& e )
{
		std::cerr << tcname << " FAILED! due to ASSERTION FAILURE" << std::endl;
		e.Print();		
}

void DecorateException( const char* tcname, CLAM::Err& e )
{
		std::cerr << tcname << " FAILED! ";
		std::cerr << "due to a CLAM GENERIC EXCEPTION!" << std::endl;
		e.Print();		
}

void DecorateException( const char* tcname, std::exception& e )
{
		std::cerr << tcname << " FAILED! ";
		std::cerr << "due to a Standard Library GENERIC EXCEPTION!" << std::endl;
		std::cerr << "message: " << e.what() << std::endl;
}

void DecorateException( const char* tcname )
{
		std::cerr << tcname << " FAILED! ";
		std::cerr << "due to a UNKNOWN EXCEPTION";				
}

int main( int arc, char** argv )
{

	try
	{
		try
		{
				CLAMDraft::TestBindingProcessingAdapter();
				PrintSuccessMessage( "TestBinding" );
		}
		catch( CLAM::ErrAssertionFailed& e )
		{
				DecorateException( "TestBinding", e );
		}
		catch( CLAM::Err& e )
		{
				DecorateException( "TestBinding",  e );
		}
		catch( std::exception& e )
		{
				DecorateException( "TestBinding", e );
		}
		catch( ... )
		{
				DecorateException( "TestBinding" );
		}
		
		try
		{
			CLAMDraft::TestAttachPresentationAdapter();
			PrintSuccessMessage( "TestAttachPresentationAdapter" );
		}
		catch( CLAM::ErrAssertionFailed& e )
		{
				DecorateException( "TestAttachPresentationAdapter", e );
		}
		catch( CLAM::Err& e )
		{
				DecorateException( "TestAttachPresentationAdapter",  e );
		}
		catch( std::exception& e )
		{
				DecorateException( "TestAttachPresentationAdapter", e );
		}
		catch( ... )
		{
				DecorateException( "TestAttachPresentationAdapter" );
		}


		
		try
		{
			CLAMDraft::TestControlUpdating();
			PrintSuccessMessage( "TestControlUpdating" );

		}
		catch( CLAM::ErrAssertionFailed& e )
		{
				DecorateException( "TestControlUpdating", e );
		}
		catch( CLAM::Err& e )
		{
				DecorateException( "TestControlUpdating",  e );
		}
		catch( std::exception& e )
		{
				DecorateException( "TestControlUpdating", e );
		}
		catch( ... )
		{
				DecorateException( "TestControlUpdating" );
		}
		
		try
		{
			CLAMDraft::TestSeveralControlsUpdating();
			PrintSuccessMessage( "TestSeveralControlsUpdating" );

		}
		catch( CLAM::ErrAssertionFailed& e )
		{
				DecorateException( "TestSeveralControlsUpdating", e );
		}
		catch( CLAM::Err& e )
		{
				DecorateException( "TestSeveralControlsUpdating",  e );
		}
		catch( std::exception& e )
		{
				DecorateException( "TestSeveralControlsUpdating", e );
		}
		catch( ... )
		{
				DecorateException( "TestSeveralControlsUpdating" );
		}
		
		std::cout << "Unit test executed" << std::endl;

	}
	catch( CLAM::Err& e )
	{
			std::cerr << "#####CLAM Exception Caught at " << __FILE__ << __LINE__ << "#####" << std::endl;
			std::cerr << "CLAM::Err Printing follows: " << std::endl;
			e.Print();

			exit(-1);
	}
	catch( std::exception& e )
	{

			std::cerr << "#####Standard Library Exception Caught at " << __FILE__ << __LINE__ << "#####" << std::endl;
			std::cerr << "CLAM::Err Printing follows: " << std::endl;
			std::cerr << e.what() << std::endl;

			exit(-1);

	}
	catch(...)
	{
			std::cerr << "#####Unknown Exception Caught at " << __FILE__ << __LINE__ << "#####" << std::endl;

			exit(-1);
	}
	
	return 0;

}

