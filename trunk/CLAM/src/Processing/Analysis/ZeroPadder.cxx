#include "ZeroPadder.hxx"

namespace CLAM
{
	ZeroPadder::ZeroPadder()
		: mInput( "Input samples", this ),
		  mOutput( "Zero-padded samples", this )
	{
	}

	ZeroPadder::~ZeroPadder()
	{
	}

	bool ZeroPadder::ConcreteConfigure( const ProcessingConfig& c )
	{
		CopyAsConcreteConfig( mConfig, c );

		return true;
	}

	bool ZeroPadder::ConcreteStart()
	{
		return true;
	}

	bool ZeroPadder::ConcreteStop()
	{
		return true;
	}

	bool ZeroPadder::Do( )
	{
		CLAM_ASSERT( AbleToExecute(), 
			     "This processing is not Ready to Do() anything" );
		
		CLAM::DataArray& src = mInput.GetAudio().GetBuffer();
		CLAM::DataArray& dst = mOutput.GetAudio().GetBuffer();

		int j = 0;
		int i = 0;

		for ( i = 0; i < src.Size(); i++, j++ )
		{
			dst[j] = src[i];
		}

		for ( ; j < dst.Size(); j++ )
		{
			dst[j] = TData(0.0);
		}

		return true;
	}
	
	
}
