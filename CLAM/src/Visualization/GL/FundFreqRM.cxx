#include "FundFreqRM.hxx"
#include "CLAMGL.hxx"
#include "CLAM_Math.hxx"

namespace CLAMVM
{
	FundFreqRM::FundFreqRM()
		: mPalette( 0.0f ), mMustProcessData( false ),  mMustColorize( false )
	{
	}

	FundFreqRM::~FundFreqRM()
	{
	}

	void FundFreqRM::SetWorldBounds( const DataBoundBox& worldBox )
	{
		mDataBBox = worldBox;
		mMustProcessData=true;
	}

	void FundFreqRM::SetPixelBounds( const ScreenBoundBox& wndBox )
	{
		mScreenBBox = wndBox;
		mMustProcessData = true;
	}

	void FundFreqRM::CacheData( const CLAM::Array<TimeFreqMagPoint>& points )
	{
		if ( mFreqBuffer.Size() < points.Size() )
		{
			mFreqBuffer.Resize( points.Size() );
			mFreqBuffer.SetSize( points.Size() );
		}

		if ( mColorBuffer.Size() < points.Size() )
		{
			mColorBuffer.Resize( points.Size() );
			mColorBuffer.SetSize( points.Size() );
		}

		if ( mErrorBuffer.Size() < points.Size() )
		{
			mErrorBuffer.Resize( points.Size() );
			mErrorBuffer.SetSize( points.Size() );
		}

		for ( int i = 0; i < points.Size(); i++ )
		{
			mFreqBuffer[i] = points[i].mFreq;
			mErrorBuffer[i] = points[i].mAttribute;
		}

		mMustProcessData = true;
		mMustColorize = true;

	}

	void FundFreqRM::ProcessData( )
	{
		if ( mMustColorize )
		{
			Colorize();
			mMustColorize = false;
		}

		mMustProcessData = false;
	}

	void FundFreqRM::Colorize( )
	{
		float maxError = 0.0f;

		for ( int i = 0; i < mErrorBuffer.Size(); i++ )
		{
			float absError = fabs( mErrorBuffer[i] );

			if (  absError > maxError )
				maxError = absError;
		}
		
		float invMaxError = 1.0/maxError;
		float t = 0.0f;

		for ( int i = 0; i < mErrorBuffer.Size(); i++ )
		{
			t = 1.0f - (mErrorBuffer[i]*invMaxError);
			mColorBuffer[i] = mPalette.Get( t );
		}

	}

	void FundFreqRM::RenderData()
	{
		if ( mMustProcessData )
			ProcessData();

		// GL drawing code

		unsigned char colors[] = {0,255,0};
		unsigned char colors2[] = {255,0,0};
		glMatrixMode( GL_MODELVIEW );
		glShadeModel( GL_SMOOTH );

		glLoadIdentity();

		glBegin( GL_LINE_STRIP );
		
		int i = (int)mDataBBox.mLeft;
		int top = (int)mDataBBox.mRight+1;

		if ( top > mFreqBuffer.Size() ) top = mFreqBuffer.Size();
	
		CLAM_ASSERT( i >= 0 , "Bad bounds!" );

		for ( i = 0 ; i < mFreqBuffer.Size(); i++ )
		{
			if ( mFreqBuffer[i]!=-1 )
			{
//				mPalette.GetRGBFromIndex( mColorBuffer[i], colors[0], colors[1], colors[2] );
				glColor3ubv( colors ); 
				glVertex2f( (GLfloat)i, mFreqBuffer[i] );
			}
			else
			{
				glColor3ubv( colors2 );
				glVertex2f( (GLfloat)i, mFreqBuffer[(i-1 >=0)? i-1: 0] );
			}
		}				
		glEnd( );
		glShadeModel( GL_FLAT );
	}

}
