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
#include <CLAM/SpectralPeaksRenderer.hxx>

namespace CLAM
{
	namespace VM
	{
		SpectralPeaksRenderer::SpectralPeaksRenderer()
		    : mCLine(VMColor::Cyan())
			, mCPoint(VMColor::Red())
		{
		}

		SpectralPeaksRenderer::~SpectralPeaksRenderer()
		{
		}

		void SpectralPeaksRenderer::SetData(std::vector<FreqMagPoint>& data)
		{
			mData = data;
		}

		void SpectralPeaksRenderer::SetPeakColor(const Color& cline, const Color& cpoint)
		{
			mCLine = cline;
			mCPoint = cpoint;
		}

		void SpectralPeaksRenderer::Render()
		{
			// points: (t,data[i].mMag)
			// vlines: top    (t,data[i].mMag) 
			//		   bottom (t,bottombound)
			glPointSize(2);
			for(unsigned i=0;i < mData.size(); i++)
			{
				// draw point
				glColor3ub(GLubyte(mCPoint.r),GLubyte(mCPoint.g),GLubyte(mCPoint.b));
				glBegin(GL_POINTS);
				glVertex2f(float(mData[i].freq-LeftBound()),float(mData[i].mag));
				glEnd();

				// draw vline
				glColor3ub(GLubyte(mCLine.r),GLubyte(mCLine.g),GLubyte(mCLine.b));
				glBegin(GL_LINES);
				glVertex2f(float(mData[i].freq-LeftBound()),float(mData[i].mag));
				glVertex2f(float(mData[i].freq-LeftBound()),float(BottomBound()));
				glEnd();
			}
		}
	}
}

// END

