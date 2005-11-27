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

#include "CLAMGL.hxx"
#include "DataRenderer.hxx"

namespace CLAM
{
	namespace VM
	{
		DataRenderer::DataRenderer()
			: mElems(0)
			, mStep(TData(1.0))
		{
			SetColor(VMColor::Blue());
		}
	
		DataRenderer::~DataRenderer()
		{
		}
	
		void DataRenderer::SetDataPtr(const TData* data,unsigned nElems,int mode)
		{
			mData = data;
			mElems = nElems;
			mMode = mode;
		}
	
		void DataRenderer::Render()
		{
			glColor3ub(GLubyte(GetColor().r),GLubyte(GetColor().g),GLubyte(GetColor().b));
			switch (mMode)
			{
				case NormalMode:
					RenderingNormalMode();
					break;
				case DetailMode:
					RenderingDetailMode();
					break;
				case HugeMode:
					RenderingHugeMode();
					break;
				default:
					break;
			}
			
		}

		void DataRenderer::RenderingNormalMode()
		{
			float x = 0.0f;
			glBegin(GL_LINE_STRIP);
			for(unsigned i = 0;i < mElems;i++)
			{
				glVertex3f(x,float(mData[i]),-1.0f);
				x += float(mStep);
			}
			glEnd();
		}

		void DataRenderer::RenderingDetailMode()
		{
			const float halfHop = 0.5f;
			float x = 0.0f;

			glBegin( GL_LINE_STRIP );
			for(unsigned i = 0; i < mElems; i++)
			{
				glVertex3f(x - halfHop,float(mData[i]),-1.0f);
				glVertex3f(x + halfHop,float(mData[i]),-1.0f);
				x += float(mStep);
			}
			glEnd();
		}

		void DataRenderer::RenderingHugeMode()
		{
			CLAM_ASSERT((mMaxsPtr != NULL && mElems > 0), "DataRenderer::RenderingHugeMode(): Invalid pointer!" );
			CLAM_ASSERT((mMinsPtr != NULL && mElems > 0), "DataRenderer::RenderingHugeMode(): Invalid pointer!" );

			double right = double(mElems);

			glMatrixMode(GL_PROJECTION);
			glPushMatrix();
			glLoadIdentity();
			glOrtho(0.0,right,BottomBound(),TopBound(),-1.0,1.0);
			glMatrixMode(GL_MODELVIEW);

			glBegin(GL_LINE_STRIP);
			for(unsigned i = 0; i < mElems;i++)
			{
				glVertex3f(float(i),float(*mMaxsPtr++),-1.0f);
				glVertex3f(float(i),float(*mMinsPtr++),-1.0f);			
			}
			glEnd();

			glMatrixMode(GL_PROJECTION);
			glPopMatrix();
			glMatrixMode(GL_MODELVIEW);
		}

		void DataRenderer::SetArrays(const TData* maxsArray,const TData* minsArray,TSize nElems)
		{
			mMaxsPtr = maxsArray;
			mMinsPtr = minsArray;
			mElems = nElems;
			mMode = HugeMode;
		}

		void DataRenderer::SetStep(const TData& step)
		{
			mStep = step;
		}

		const TData& DataRenderer::GetStep() const
		{
			return mStep;
		}
	}
}

//END


