#include "GLSpectralBlock.hxx"

using namespace CLAMGUI;

GLSpectralBlock::GLSpectralBlock( int divs )
	: mDivisions( divs ), mDLid( 0 ), mDLready(false)
{

	mBlockHeights.Resize( divs );
	mBlockHeights.SetSize( divs );
}

void GLSpectralBlock::CacheData( const DataArray& array )
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

void GLSpectralBlock::DataTransform( const DataArray& array )
{

}

void GLSpectralBlock::DefineViewport( const DataArray& array, Viewport& v )
{
	v.bottom = 0;
	v.top = 1;
	v.left = 0;
	v.right = TData(mDivisions);
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
