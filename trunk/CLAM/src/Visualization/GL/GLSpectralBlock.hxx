#ifndef __GLSPECTRALBLOCK__
#define __GLSPECTRALBLOCK__

#include "ArrayRenderer.hxx"
#include "CLAMGL.hxx"
#include "TabFunct.hxx"
#include "CLAM_Math.hxx"
#include "Assert.hxx"
#include <iostream>
#include <list>
#include <utility>

namespace CLAMGUI
{

using CLAM::TData;

class GLSpectralBlock
	: public ArrayRenderer
{

	struct tIndexRange
	{
		int left;
		int right;
		int size;
	};
	typedef std::list<tIndexRange> tIndexList;

public:

	GLSpectralBlock( int divs );

	virtual ~GLSpectralBlock()
	{
	}

	void CacheData( const DataArray& );

	void CacheSampleRate( TData sampleRate );

	void DefineViewport( const DataArray&, Viewport& );

	void Draw();

protected:

	void CalculateBandRanges( int arraySize );

	void GenerateBlockHeights( const DataArray& array );

	void DataTransform( const DataArray& );

	void XaxisTransform( TData left, TData right, TData& transleft, TData& transright, bool& integer )
	{
	}

	void YaxisTransform( TData top, TData bottom, TData& transtop, TData& transbottom, bool& integer )
	{
	}

	void BuildDisplayList();

private:	
	inline float compress( float x )
	{
		CLAM_DEBUG_ASSERT( Th > 0.0f, "Threshold must be positive and greater thant zero" );
		if ( x > A )
			x = A;
		
		float h = ( A - Th )/r;

		if ( x < Th )
		{
			return x*((1.0f-h)/Th);
		}
		else if ( x >= Th )
		{
//			std::cout << "G: " << (1.0f-h)+(x-Th)/r << std::endl;
			return (1.0f-h)+(x-Th)/r;
		}
	}


	struct Log10_20
	{
		inline TData operator()(const TData arg)
		{
			return 20.0f * log10( arg );
		}
	};

	int                       mDivisions;
	DataArray                 mBlockHeights;
	GLuint                    mDLid;
	bool                      mDLready;
	TData                     mSpectralRange;
	//	CLAM::TabFunct<Log10_20>  mLogFunc;
	float                     mOctaveStride;
	const float               mF0;
	const float               mFf;
	bool                      mMustCalculateIndexes;
	tIndexList                mRangesList;

	// Compressor variables
	const float               Th; // threshold
	const float               A;  // maximum input amplitude
	const float               r;  // compression ratio [ 1, 10 ]
	unsigned                  mBands;

};

}

#endif // GLSpectralBlock.hxx
