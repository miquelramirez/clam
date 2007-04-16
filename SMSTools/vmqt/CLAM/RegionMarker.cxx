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
#include "RegionMarker.hxx"

namespace CLAM
{
	namespace VM
	{
		RegionMarker::RegionMarker()
		{
			mData[0] = mData[1] = 0.0;
			SetColor(VMColor::LightGray());
		}

		RegionMarker::~RegionMarker()
		{
		}

		void RegionMarker::Render()
		{
			if(mData[0] < mData[1])	
			{
				glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
				glEnable(GL_BLEND);
				glColor4ub(GLubyte(GetColor().r),GLubyte(GetColor().g),GLubyte(GetColor().b),185);
				DrawRegion(GetDrawMode(0));
				glDisable(GL_BLEND);
			}
		}

		int RegionMarker::GetDrawMode(int index)
		{
			// 0 -> neither the points nor the region are visible
			// 1 -> the region is visible but the points are not  
			// 2 -> the first point is visible but the second is not
			// 3 -> the second point is visible but the first is not
			// 4 -> both points are visible
			double p1 = mData[index];
			double p2 = mData[index+1];
			if(p1 < LeftBound() && p2 > RightBound()) return 1;
			if(p1 >= LeftBound() && p2 > RightBound()) return 2;
			if(p1 < LeftBound() && p2 <= RightBound()) return 3;
			if(p1 >= LeftBound() && p2 <= RightBound()) return 4;
			return 0;
		}

		void RegionMarker::DrawRegion(int mode)
		{
			glBegin(GL_QUADS);
			switch(mode)
			{
				case 1:
					// the region is visible but the points are not
					glVertex3f(0.0f,float(TopBound()),0.5f);
					glVertex3f(float(RightBound()-LeftBound()),float(TopBound()),0.5f);
					glVertex3f(float(RightBound()-LeftBound()),float(BottomBound()),0.5f);
					glVertex3f(0.0f,float(BottomBound()),0.5f);
					break;
				case 2:
					// the first point is visible but the second is not
					glVertex3f(float(mData[0]-LeftBound()),float(TopBound()),0.5f);
					glVertex3f(float(RightBound()-LeftBound()),float(TopBound()),0.5f);
					glVertex3f(float(RightBound()-LeftBound()),float(BottomBound()),0.5f);
					glVertex3f(float(mData[0]-LeftBound()),float(BottomBound()),0.5f);
					break;
				case 3:
					// the second point is visible but the first is not
					glVertex3f(0.0f,float(TopBound()),0.5f);
					glVertex3f(float(mData[1]-LeftBound()),float(TopBound()),0.5f);
					glVertex3f(float(mData[1]-LeftBound()),float(BottomBound()),0.5f);
					glVertex3f(0.0f,float(BottomBound()),0.5f);
					break;
				case 4:
					// both points are visible
					glVertex3f(float(mData[0]-LeftBound()),float(TopBound()),0.5f);
					glVertex3f(float(mData[1]-LeftBound()),float(TopBound()),0.5f);
					glVertex3f(float(mData[1]-LeftBound()),float(BottomBound()),0.5f);
					glVertex3f(float(mData[0]-LeftBound()),float(BottomBound()),0.5f);
					break;
				default:
					// neither the points nor the region are visible
					break;
			}
			glEnd();
		}

		void RegionMarker::SetBegin(double value)
		{
			mData[0] = value;
		}

		void RegionMarker::SetEnd(double value)
		{
			mData[1] = value;
		}

		double RegionMarker::GetBegin() const
		{
			return mData[0];

		}

		double RegionMarker::GetEnd() const
		{
			return mData[1];
		}
	}
}

// END

