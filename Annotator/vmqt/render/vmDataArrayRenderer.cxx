/*
 * Copyright (c) 2001-2006 MUSIC TECHNOLOGY GROUP (MTG)
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

#include <algorithm>
#include "CLAMGL.hxx"
#include "vmDataArrayRenderer.hxx"
#include "vmPlot2D.hxx"

namespace CLAM
{
	namespace VM
	{

		DataArrayRenderer::DataArrayRenderer()
			: mHugeMode(false)
			, mHugeModeEnabled(true)
			, mMustProcessData(false)
			, mDataColor(0,0,255)
		{
		}

		DataArrayRenderer::~DataArrayRenderer()
		{
		}
	
		void DataArrayRenderer::SetData(const DataArray& data)
		{
			mCachedData.assign(&data[0], &data[0]+data.Size());
			mMustProcessData = true;
			_container->updateRenderers();
		}

		void DataArrayRenderer::SetDataColor(const QColor& c)
		{
			mDataColor = c;
			_container->needUpdate();
		}

		void DataArrayRenderer::SetHugeModeEnabled(bool en)
		{
			mHugeModeEnabled = en;
		}

		void DataArrayRenderer::Render()
		{
			if(!mEnabled || !mCachedData.size()) return;
			if(mMustProcessData) {ProcessData(); mMustProcessData=false;}
			glMatrixMode(GL_PROJECTION);
			glPushMatrix();
			glLoadIdentity();
			glOrtho(mLocalView.left,mLocalView.right,mLocalView.bottom,mLocalView.top,-1.0,1.0);
			glMatrixMode(GL_MODELVIEW);
			glLineWidth(1);
			glColor3ub(mDataColor.red(),mDataColor.green(),mDataColor.blue());
			(mHugeMode) ? DrawHugeMode() : DrawNormalMode();
			glMatrixMode(GL_PROJECTION);
			glPopMatrix();
			glMatrixMode(GL_MODELVIEW);
		}

		void DataArrayRenderer::SetHBounds(double left, double right)
		{
			if(!mCachedData.size()) return;
		  
			mView.left = (left-mXRange.min)*double(mCachedData.size())/mXRange.Span();
			mView.right = (right-mXRange.min)*double(mCachedData.size())/mXRange.Span();
			mMustProcessData = true;
		}

		void DataArrayRenderer::SetVBounds(double bottom, double top)
		{
			Renderer2D::SetVBounds(bottom,top);
			mLocalView.bottom = mView.bottom;
			mLocalView.top = mView.top;
		}

		void DataArrayRenderer::SetViewport(const GLViewport& v)
		{
			Renderer2D::SetViewport(v);
			mMustProcessData = true;
		}

		void DataArrayRenderer::ProcessData()
		{
			mLocalView.left = 0.0;
		   
			unsigned offset = (mView.left > 0) ? unsigned(mView.left) : 0;
			unsigned length = unsigned(mView.right-mView.left);

			if(mHugeModeEnabled && length/5 >= unsigned(mViewport.w) )
			{
				mHugeMode = true;
				unsigned max_size = (unsigned)mViewport.w;
				mMaxArray.resize(max_size);
				mMinArray.resize(max_size);

				unsigned search_interval_len = length / max_size;
				unsigned search_rem_interval_len = length % max_size;
		
				unsigned first_pass_iterations = (search_rem_interval_len) ? max_size-1 : max_size;
				unsigned start_search = offset;
				unsigned end_search = start_search + search_interval_len;				
		
				CLAM::TData * dataPtr = &mCachedData[0];
				for(unsigned i=0; i < first_pass_iterations; i++)
				{
					mMaxArray[i] = *std::max_element(dataPtr+start_search, 
													 dataPtr+end_search);
					mMinArray[i] = *std::min_element(dataPtr+start_search, 
													 dataPtr+end_search);
					start_search = end_search;
					end_search += search_interval_len;	
				}
				if(!search_rem_interval_len) return;
				mMaxArray[max_size-1] = *std::max_element(dataPtr+start_search, 
														  dataPtr+start_search+search_rem_interval_len);
				mMinArray[max_size-1] = *std::min_element(dataPtr+start_search, 
														  dataPtr+start_search+search_rem_interval_len);
				mLocalView.right = double(mMaxArray.size())-1;
				return;
			}
	
			mHugeMode = false;

			mProcessedData.resize(length+1);
			std::copy(&mCachedData[0]+offset, 
					  &mCachedData[0]+offset+length+1, 
					  &mProcessedData[0]);
			mLocalView.right = double(mProcessedData.size())-1;
		}

		void DataArrayRenderer::DrawHugeMode()
		{
			glBegin(GL_TRIANGLE_STRIP);
			for(unsigned i=0; i < mMaxArray.size(); i++)
			{
				glVertex2d(double(i),mMaxArray[i]);
				glVertex2d(double(i),mMinArray[i]);
			}
			glEnd();
		}

		void DataArrayRenderer::DrawNormalMode()
		{
			glBegin(GL_LINE_STRIP);
			for(unsigned i=0; i < mProcessedData.size(); i++)
			{
				glVertex2d(double(i),mProcessedData[i]);
			}
			glEnd();
		}
	}
}

// END

