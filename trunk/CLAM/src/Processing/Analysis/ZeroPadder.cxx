#include "ZeroPadder.hxx"

namespace CLAM
{
	void ZeroPadderConfig::DefaultInit()
	{
		AddAll();
		UpdateData();
	}

	ZeroPadder::ZeroPadder()
		: mInput( "Input samples", this, 1 ),
		  mOutput( "Zero-padded samples", this, 1 )
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
		
		CLAM::DataArray& src = mInput.GetData().GetBuffer();
		CLAM::DataArray& dst = mOutput.GetData().GetBuffer();

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
