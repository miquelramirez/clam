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

#ifndef __GLTF2DRENDERER__
#define __GLTF2DRENDERER__

#include <valarray>
#include "Array.hxx"
#include "GLSprite.hxx"
#include "DataTypes.hxx"
#include "VertexFormats.hxx"
#include "ArrayRenderer.hxx"
#include "Raster.hxx"
using CLAM::DataArray;
using CLAM::TTime;
using CLAM::TData;

namespace CLAMGUI
{

	class SpectrumRasterizer;

	class GLTF2DRenderer
	{
	public:

		GLTF2DRenderer()
		: mAllocRaster( false ), mpRasterizer( NULL )
		{
		}

		virtual ~GLTF2DRenderer()
		{
			if ( mAllocRaster )
			delete mRaster;
		}

		virtual void SetupRender( TTime btime, TTime etime, TData specrng, unsigned int nframes );
		virtual void RenderFrame( const DataArray& array, bool isLinear );
		virtual void Draw();
		virtual void DefineViewport( Viewport& view_specs );

	private:

		virtual void BuildNewTex();
		virtual void SyncSprites( GLuint new_tex );
		virtual void OverwriteTex();
		virtual void PrioritizeTex();

		Viewport                 mView;
		SpriteBuffer             mSprites;
		std::valarray< GLuint >  mTextureBuffer;
		unsigned int             mLastTex;      // index to the last texture introduced...
		bool                     mAllocRaster;
		Raster*                  mRaster;
		SpectrumRasterizer*      mpRasterizer;
	};

}

#endif // GLTF2DRenderer.hxx
