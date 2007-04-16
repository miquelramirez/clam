/*
 * Copyright (c) 2004 MUSIC TECHNOLOGY GROUP (MTG)
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
#include <iostream>
#include <stdlib.h>
#include "CLAMGL.hxx"
#include "Renderer.hxx"

namespace CLAM
{
	namespace VM
	{
		Renderer::Renderer()
			: mReadPixels(true)
			, mLeftBound(0.0)
			, mRightBound(1.0)
			, mTopBound(1.0)
			, mBottomBound(0.0)
			, mColor(VMColor::Green())
			, mWidth(1)
			, mHeight(0)
			, mCounter(0)
		{
			mBits=NULL;
		}

		Renderer::~Renderer()
		{
			if(mBits!=NULL) free(mBits);
		}

		void Renderer::SetHBounds(double left, double right)
		{
			mLeftBound = left;
			mRightBound = right;
		}

		void Renderer::SetVBounds(double bottom, double top)
		{
		    mTopBound = top;
			mBottomBound = bottom;
		}

		void Renderer::SetColor(const Color& c)
		{
			mColor = c;
		}

		const Color& Renderer::GetColor() const
		{
			return mColor;;
		}

		double Renderer::LeftBound() const
		{
			return mLeftBound;
		}

		double Renderer::RightBound() const
		{
			return mRightBound;
		}

		double Renderer::BottomBound() const
		{
			return mBottomBound;
		}

		double Renderer::TopBound() const
		{
			return mTopBound;
		}

		void Renderer::SaveScreen()
		{
			mCounter = 0;
			mReadPixels = true;
		}

		void Renderer::ReadPixels()
		{
			if(mBits!=NULL) 
			{
				free(mBits);
				mBits=NULL;
			}
 
			GLint viewport[4];
			glGetIntegerv(GL_VIEWPORT, viewport);
			mWidth = viewport[2];
			mHeight = viewport[3];

			int width = mWidth * 3;
			width   = (width + 3) & ~3;	   

		    int bitsize = width * mHeight;	
			if((mBits = calloc(bitsize, 1)) == NULL) return;  
			
			glFinish();
			glPixelStorei(GL_PACK_ALIGNMENT, 4);   
			glPixelStorei(GL_PACK_ROW_LENGTH, 0);
			glPixelStorei(GL_PACK_SKIP_ROWS, 0);
			glPixelStorei(GL_PACK_SKIP_PIXELS, 0);			
			glReadPixels(0, 0, mWidth, mHeight, GL_RGB, GL_UNSIGNED_BYTE, mBits);
			glFinish();
			if(mCounter > 0) 
			{
				mReadPixels=false;
				mCounter = 0;
			}
			mCounter++;
		}

		void Renderer::DrawPixels()
		{
			if(mBits==NULL) return;
			
			GLint viewport[4];
			glGetIntegerv(GL_VIEWPORT, viewport);
			int xsize = viewport[2];
			int ysize = viewport[3];
			float xscale  = (float)xsize / (float)mWidth;
			float yscale  = (float)ysize / (float)mHeight;

			glFinish();
			glMatrixMode(GL_PROJECTION);
			glPushMatrix();
			glLoadIdentity();
			glOrtho(0.0,double(viewport[2]),0.0,double(viewport[3]),-1.0,1.0);
			glMatrixMode(GL_MODELVIEW);
			
			glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
			glPixelZoom(xscale, yscale);
			glRasterPos2i(0, 0);
			glDrawPixels(mWidth, mHeight, GL_RGB, GL_UNSIGNED_BYTE, mBits);
			
			glMatrixMode(GL_PROJECTION);
			glPopMatrix();
			glMatrixMode(GL_MODELVIEW);
			glFinish();
		}
	}
}

// END

