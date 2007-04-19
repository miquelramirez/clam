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

#include <CLAM/CLAMGL.hxx>
#include <CLAM/DataRenderer.hxx>

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
	
		void DataRenderer::SetData(const DataArray& data, int mode)
		{
			mData = &data;
			mElems = mData->Size();
			mMode = mode;
		}
	
		void DataRenderer::Render()
		{
			if(mElems < 4) return;
			glColor3ub(GLubyte(GetColor().r),GLubyte(GetColor().g),GLubyte(GetColor().b));
			if(!mReadPixels)
			{
				DrawPixels();
				return;
			}
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
			ReadPixels();
		}

		void DataRenderer::RenderingNormalMode()
		{
			float x = 0.0f;
			glBegin(GL_LINE_STRIP);
			for(unsigned i = 0;i < mElems;i++)
			{
				glVertex3f(x,float(*(mData->GetPtr()+i)),-1.0f);
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
				glVertex3f(x - halfHop,float(*(mData->GetPtr()+i)),-1.0f);
				glVertex3f(x + halfHop,float(*(mData->GetPtr()+i)),-1.0f);
				x += float(mStep);
			}
			glEnd();
		}

		void DataRenderer::RenderingHugeMode()
		{
			double right = double(mElems);

			glMatrixMode(GL_PROJECTION);
			glPushMatrix();
			glLoadIdentity();
			glOrtho(0.0,right,BottomBound(),TopBound(),-1.0,1.0);
			glMatrixMode(GL_MODELVIEW);

			glBegin(GL_LINE_STRIP);
			for(unsigned i = 0; i < mElems;i++)
			{
				glVertex3f(float(i),float(*(mMaxs->GetPtr()+i)),-1.0f);
				glVertex3f(float(i),float(*(mMins->GetPtr()+i)),-1.0f);			
			}
			glEnd();

			glMatrixMode(GL_PROJECTION);
			glPopMatrix();
			glMatrixMode(GL_MODELVIEW);
		}

		void DataRenderer::SetArrays(const DataArray& maxsArray,const DataArray& minsArray)
		{
			mMaxs = &maxsArray;
			mMins = &minsArray;
			mElems = mMaxs->Size();
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


