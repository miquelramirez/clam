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
		{
		        _nElems=0;
			SetStep(TData(1.0));
			SetColor(VMColor::Blue());
		}
	
		DataRenderer::~DataRenderer()
		{
		}
	
		void DataRenderer::SetDataPtr(const TData* data,unsigned nElems,int mode)
		{
			_data = data;
			_nElems = nElems;
			_mode = mode;
		}
	
		void DataRenderer::Render()
		{
			glColor3ub(GLubyte(_color.r),GLubyte(_color.g),GLubyte(_color.b));
			switch (_mode)
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
			for(unsigned i = 0;i < _nElems;i++)
			{
				glVertex3f(x,float(_data[i]),-1.0f);
				x += float(_step);
			}
			glEnd();
		}

		void DataRenderer::RenderingDetailMode()
		{
			const TData halfHop = TData(0.5);
			float x = 0.0f;

			glBegin( GL_LINE_STRIP );
			for(unsigned i = 0; i < _nElems; i++)
			{
				glVertex3f(x - halfHop,_data[i],-1.0f);
				glVertex3f(x + halfHop,_data[i],-1.0f);
				x += float(_step);
			}
			glEnd();
		}

		void DataRenderer::RenderingHugeMode()
		{
			CLAM_ASSERT( (_maxsPtr != NULL && _nElems > 0), "Invalid pointer!" );
			CLAM_ASSERT( (_minsPtr != NULL && _nElems > 0), "Invalid pointer!" );

			TData right = TData(_nElems);

			glMatrixMode(GL_PROJECTION);
			glPushMatrix();
			glLoadIdentity();
			glOrtho(0.0,right,_bottom,_top,-1.0,1.0);
			glMatrixMode(GL_MODELVIEW);
			glClear(GL_COLOR_BUFFER_BIT);
			glBegin(GL_LINE_STRIP);
			for(unsigned i = 0; i < _nElems;i++)
			{
				glVertex3f(float(i),float(*_maxsPtr++),-1.0f);
				glVertex3f(float(i),float(*_minsPtr++),-1.0f);			
			}
			glEnd();
			glMatrixMode(GL_PROJECTION);
			glPopMatrix();
			glMatrixMode(GL_MODELVIEW);
		}

		void DataRenderer::SetArrays(const TData* maxsArray,const TData* minsArray,TSize nElems)
		{
			_maxsPtr = maxsArray;
			_minsPtr = minsArray;
			_nElems = nElems;
			_mode = HugeMode;
		}

		void DataRenderer::SetStep(const TData& step)
		{
			_step = step;
		}

		TData DataRenderer::GetStep() const
		{
			return _step;
		}
	}
}

//END


