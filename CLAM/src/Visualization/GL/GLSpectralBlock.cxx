#include "GLSpectralBlock.hxx"

using namespace CLAMGUI;

GLSpectralBlock::GLSpectralBlock( int divs )
	: mDivisions( divs ), mDLid( 0 ), mDLready(false), mSpectralRange( 0 ),
	  mF0( 20.0f ), mFf( 20000.0f ), mMustCalculateIndexes(false),
	  Th(0.007f),A(0.45f), r(2), 
	  mBands(100)
{
	// int N = log(fMax/fInit)/log(octavePart); //num of bands
	mDivisions = int(log10f(mFf/mF0)/log10f(4.0/3.0));

	mBlockHeights.Resize( mBands );
	mBlockHeights.SetSize( mBands );


	
	mOctaveStride = 4.0f/3.0f;
}

void GLSpectralBlock::CacheSampleRate( TData sampleRate )
{
	if ( mSpectralRange != sampleRate )
	{	
		mSpectralRange = sampleRate;
		
		mMustCalculateIndexes = true;
	}
}
/*
void oldCacheData( const DataArray& array )
{
	// We will hop on the array with a fixed delta that depends
	// on the number of divisions (bands) to be measured
	int delta_bin = array.Size() / mDivisions;

	TData accum = TData(0.0); // Here we will accumulate the spectrum power of the band
	int max_idx = 0;          // Here we will store the band wich has the maximum energy
	int height_idx = 0;
	int band_offset, band_end, high_bound;


	const TData* arrayptr = array.GetPtr();

	high_bound = delta_bin * mDivisions;
	band_offset = 0;
	band_end = delta_bin;

	while ( band_offset < high_bound )
		{
			for ( accum = 0; band_offset < band_end; band_offset++ )
				{
					accum += *arrayptr++;
				}
			
			band_end += delta_bin;
			mBlockHeights[ height_idx ] = accum;

			if ( mBlockHeights[max_idx] < mBlockHeights[ height_idx ] )
				max_idx = height_idx;

			height_idx++;
		} 

	// Once we have determined both the energy level for each band
	// and the band with the maximum energy we now calculate the 
	// scaling for each quad representing each band

	TData inv_max_energy = TData(1.0) / (CLAM_DB_SCALING *delta_bin);//mBlockHeights[ max_idx ];

	TData* blockh = mBlockHeights.GetPtr();
	TData* blockend = mBlockHeights.GetPtr() + mBlockHeights.Size();

	while ( blockh != blockend )
	{
		if( (*blockh *= inv_max_energy )> TData(1) )  *blockh=TData(1);// ;
		blockh++;
	}

}
*/

void GLSpectralBlock::CalculateBandRanges( int arraySize )
{
	mRangesList.clear();

	float invSampRate = 1.0f/float(mSpectralRange);
	float convFactor = (arraySize-1)*invSampRate;

	tIndexRange tmp = { 0, 0, 0 };

	tmp.right = mF0 * convFactor;

	for ( int k = 0; k < mBands; k++ )
	{
		float exponent= float((k+1)*mDivisions)/mBands;

		tmp.left = tmp.right;
		tmp.right = int(mF0*(powf(mOctaveStride, exponent))*convFactor);

		tmp.size = tmp.right - tmp.left + 1;

		CLAM_DEBUG_ASSERT( tmp.right < arraySize, "Out of bonds!!!!" );
		mRangesList.push_back( tmp );

	}
}


void GLSpectralBlock::GenerateBlockHeights( const DataArray& array)
{
	// first we generate the sums "tal cual"

	tIndexList::iterator pItem = mRangesList.begin();
	tIndexList::iterator pEnd = mRangesList.end();

	CLAM_ASSERT( mBlockHeights.Size() == mRangesList.size(), "Check the nodes in the list boy!" );

	float  accum=0;
	const TData* dataPtr0, *dataPtr1;
	float invsize;
	int k = 0; // k is the block height index

	while ( pItem!= pEnd )
	{
		
		dataPtr0 = array.GetPtr()+pItem->left;
		dataPtr1 = array.GetPtr()+pItem->left+pItem->size;
		CLAM_DEBUG_ASSERT( dataPtr0!=dataPtr1, "Must not be equal_!!!" );
		invsize = 1.0f/float(pItem->size);

		while ( dataPtr0 != dataPtr1 )
		{
			accum+= *dataPtr0++;
		}

		mBlockHeights[k] = compress( accum*invsize );
		
		accum = 0;
		k++;
		pItem++;
	}

}

void GLSpectralBlock::CacheData( const DataArray& array )
{
	if ( mMustCalculateIndexes )
		{
			CalculateBandRanges(array.Size());
			mMustCalculateIndexes = false;
		}

	GenerateBlockHeights(array);
}

void GLSpectralBlock::DataTransform( const DataArray& array )
{

}

void GLSpectralBlock::DefineViewport( const DataArray& array, Viewport& v )
{
	v.bottom = 0;
	v.top = 1;
	v.left = 0;
	v.right = TData(mBands);
}

void GLSpectralBlock::BuildDisplayList()
{
	mDLid = glGenLists(1);

	if ( mDLid != 0 )
		{
			mDLready = true;
			
			glNewList( mDLid, GL_COMPILE );
			{
				glBegin( GL_QUAD_STRIP );
				{
					glColor3ub( 255, 0, 0 );
					glVertex2f( 0.0f, 0.0f );
					glVertex2f( 1.0f, 0.0f );
					glColor3ub( 238, 151, 74 ); // Orange
					glVertex2f( 0.0f, 0.2f );
					glVertex2f( 1.0f, 0.2f );
					glColor3ub( 233, 241, 5 ); // Yellow
					glVertex2f( 0.0f, 0.7f );
					glVertex2f( 1.0f, 0.7f );
					glColor3ub( 0, 0, 0 );
					glVertex2f( 0.0f, 1.0f );
					glVertex2f( 1.0f, 1.0f );
				}
				glEnd();
			}
			glEndList();
		}
}

void GLSpectralBlock::Draw()
{
	if ( !mDLready )
		BuildDisplayList();
	else
		{
		
			glClear( GL_COLOR_BUFFER_BIT );
			glMatrixMode( GL_MODELVIEW );
			glLoadIdentity();
			for ( int k = 0; k < mBlockHeights.Size(); k++ )
				{
					glPushMatrix();
					{
						glTranslatef( k , 0, 0 );
						glScalef( 1.0f, mBlockHeights[k], 1.0f );
						glCallList( mDLid );
					}
					glPopMatrix();
				}
		}
	
}







