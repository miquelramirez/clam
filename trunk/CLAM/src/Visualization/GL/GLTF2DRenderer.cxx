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

#include "GLTF2DRenderer.hxx"
#include "SpectrumRasterizer.hxx"
#include "Pixel.hxx"
#include "CLAMGL.hxx"
#include "ErrGUI.hxx"


using namespace CLAMGUI;

void GLTF2DRenderer::SetupRender( TTime bTime, TTime eTime, TData specrng, unsigned int nFrames )
{

	// Viewport construction

	mView.top = specrng;
	mView.bottom = 0.0f;
	mView.left = float(bTime);
	mView.right = float(eTime);
	mView.isIntX = false;
	mView.isIntY = false;

	// SpriteBuffer construction
	// Initializing both the textures and the sprites

	TData dx = TData( eTime - bTime ) / TData( nFrames );
	TData x = 0;
	TData y = 0;
	TData w = dx;
	TData h = specrng;
	unsigned int cnt = 0;

	mTextureBuffer.resize( nFrames );
	mLastTex = 0;

	do
	{
		mSprites.push_back( new GLSprite( x , y, w, h ) );
		x += dx;
		cnt++;
	}
	while ( cnt < nFrames );

	SpriteBuffer::iterator        spbufIt;
	unsigned int texbufIt;

	CLAM_ASSERT( mTextureBuffer.size() == mSprites.size(), "Both texture and sprite buffer must have same size" );

	spbufIt = mSprites.begin();

	for ( texbufIt = 0; texbufIt != mTextureBuffer.size(); texbufIt++ )
	{
		mTextureBuffer[texbufIt] = 0; // We set initially textures to zero

		// Note that we assign the pointers to the Sprites. This may seem an extremely risky thing to do
		// but note that both Sprites and textures must be at all times of the same size. Besides, any resize
		// of the number of frames and that all implies a total reconfiguration. This way, one only has to
		// iterate over the texture, change their values, and that's all. Neat!
		(*spbufIt)->SetPixMap( &(mTextureBuffer[texbufIt]) ); 

		spbufIt++;
	}
}

void GLTF2DRenderer::RenderFrame( const DataArray& array, bool isLin )
{
	unsigned int nbins = array.Size();
	unsigned int LUTSize = 1024;

	if ( mpRasterizer == NULL )
	{
		TColorPath myPath;
		myPath.AddElem( Pixel( 0, 0, 0, 3 ) );      // Black
		myPath.AddElem( Pixel( 0, 0, 255, 3 ) );    // Blue
		myPath.AddElem( Pixel( 0, 255, 0, 3 ) );    // Green
		myPath.AddElem( Pixel( 255, 255, 0, 3 ) );  // Yellow
		myPath.AddElem( Pixel( 255, 0, 0, 3 ) );    // Red
		mpRasterizer = SpectrumRasterizer::GetInstance( LUTSize, myPath ); // Tendria que aceptarse parámetros por defecto...
	}
	if ( !mAllocRaster )
	{
		mRaster = new Raster( mTextureBuffer.size(), nbins );
		mAllocRaster = true;
	}
	else 
	{
		if ( nbins != mRaster->dim2() )
		{
			// Frame width changed, so we have to resize the raster
			delete mRaster;
			mRaster = new Raster( mTextureBuffer.size(), nbins );
		}
	}

	if ( mLastTex == mTextureBuffer.size() )
	{
	// Wraparound
	PrioritizeTex(); // Performance Hack, this may increase performance by making all textures cached in the card
	mLastTex = 0; // We start over
	if ( isLin )
		mpRasterizer->Rasterize( mLastTex, array, *mRaster );
	else
		mpRasterizer->LogDataRasterize( mLastTex, array, *mRaster );

	OverwriteTex();
	}
	else
	{
	// No wraparound
	if ( isLin )
		mpRasterizer->Rasterize( mLastTex, array, *mRaster );
	else
		mpRasterizer->LogDataRasterize( mLastTex, array, *mRaster );

	BuildNewTex();
	}
}

void GLTF2DRenderer::BuildNewTex()
{
	// TODO: if wraparound happens we should write over the texture, instead of creating
	// a new one. 

	GLuint texId;

	glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );

	glGenTextures( 1, &texId );

	if ( texId == 0 )
	{
		// Something bad has happenned
		GLenum error = glGetError();

		throw( GUIException( "Not able to create a texture" ) );//gluGetErrorString( error ) ) );
	}

	glBindTexture( GL_TEXTURE_1D, texId );

	glTexParameteri ( GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_REPEAT );
	glTexParameteri ( GL_TEXTURE_1D, GL_TEXTURE_WRAP_T, GL_REPEAT );

	glTexParameteri ( GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameteri ( GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );

	glTexImage1D ( GL_TEXTURE_1D, 0, GL_RGB8, mRaster->dim2() - 1, 0, GL_RGB, GL_UNSIGNED_BYTE, &( mRaster->row( mLastTex )[0] ) );

	SyncSprites( texId );
}

void GLTF2DRenderer::SyncSprites( GLuint new_tex )
{
	mTextureBuffer[ mLastTex ] = new_tex;
	mLastTex++;
}

void GLTF2DRenderer::OverwriteTex()
{
	glBindTexture( GL_TEXTURE_1D, mTextureBuffer[ mLastTex ] );

	glTexSubImage1D( GL_TEXTURE_1D, 0, 0, mRaster->dim2()-1, GL_RGB, GL_UNSIGNED_BYTE, & ( mRaster->row( mLastTex )[0] ) );

	mLastTex++;
}

void GLTF2DRenderer::PrioritizeTex()
{
	std::valarray< GLclampf > priorities(  1.0f , mTextureBuffer.size() ); // Max priority for already built textures

	glPrioritizeTextures( mTextureBuffer.size(), &( mTextureBuffer[0] ), &( priorities[0]) );
}

void GLTF2DRenderer::Draw()
{
	glMatrixMode(GL_MODELVIEW);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	GLSprite::SetOpenGLState( GL_TEXTURE_1D );
	for (int k=0; k < unsigned(mSprites.size()) ; k++ )
	{
		(mSprites[k])->ImmediateDisplaySprite( GL_TEXTURE_1D );
		CLAM_ASSERT( glIsTexture(*(mSprites[k]->GetPixMap())), "That wasn't a valid texture boy!");

	}	

	GLSprite::UnSetOpenGLState( GL_TEXTURE_1D );
}

void GLTF2DRenderer::DefineViewport( Viewport& view_specs )
{
	view_specs = mView;
}
