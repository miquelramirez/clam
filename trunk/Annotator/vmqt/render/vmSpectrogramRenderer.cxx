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

#include "CLAMGL.hxx"
#include "vmSonogramBuilder.hxx"
#include "vmSpectrogramRenderer.hxx"
#include "vmPlot2D.hxx"

namespace CLAM
{
	namespace VM
	{
		SpectrogramRenderer::SpectrogramRenderer()
			: mglColorList(0)
			, mglBlackWhiteList(0)
			, mColormap(COLOR_SONOGRAM)
		{
		}

		SpectrogramRenderer::~SpectrogramRenderer()
		{
		}

		void SpectrogramRenderer::SetData(const Array<Spectrum>& specMtx)
		{
			if(!specMtx.Size()) return;
			mComputedData.clear();
			mColorData.clear();
			mBlackWhiteData.clear();
			SonogramBuilder sbuilder;
			sbuilder.MakeSonogram(specMtx,mComputedData,mColorData,mBlackWhiteData);
			_container->updateRenderers();
			emit fixedLabels(QString::number(specMtx[0].GetSpectralRange(),'f',0),
							 QString::number(mComputedData.size()));
		}

		void SpectrogramRenderer::colorSonogram()
		{
			mColormap = COLOR_SONOGRAM;
			_container->needUpdate();
		}

		void SpectrogramRenderer::blackWhiteSonogram()
		{
			mColormap = BW_SONOGRAM;
			_container->needUpdate();
		}

		void SpectrogramRenderer::Render()
		{
			if(!mEnabled) return;
			
			glMatrixMode(GL_PROJECTION);
			glPushMatrix();
			glLoadIdentity();
			glOrtho(mLocalView.left,mLocalView.right,mLocalView.bottom,mLocalView.top,-1.0,1.0);
			glMatrixMode(GL_MODELVIEW);

			switch(mColormap)
			{
				case COLOR_SONOGRAM:
					DrawColorData();
					break;
				case BW_SONOGRAM:
					DrawBlackWhiteData();
					break;
				default:
					break;
			}

			glMatrixMode(GL_PROJECTION);
			glPopMatrix();
			glMatrixMode(GL_MODELVIEW);

		}

		void SpectrogramRenderer::SetHBounds(double left, double right)
		{
			if(!mColorData.size()) return;
			mLocalView.left = (left-mXRange.min)*double(mColorData.size())/mXRange.Span();
			mLocalView.right = (right-mXRange.min)*double(mColorData.size())/mXRange.Span()-1.0;
		}

		void SpectrogramRenderer::SetVBounds(double bottom, double top)
		{
			if(!mColorData.size()) return;
			if(!mColorData[0].size()) return;
			mLocalView.bottom = (bottom-mYRange.min)*double(mColorData[0].size())/mYRange.Span();
			mLocalView.top = (top-mYRange.min)*double(mColorData[0].size())/mYRange.Span()-1.0;
 		}

		void SpectrogramRenderer::MouseMoveEvent(double x, double y)
		{
			if(!mColorData.size()) return;
			if(!mColorData[0].size()) return;
			if(x < mXRange.min || x > mXRange.max) return;
			if(y < mYRange.min || y > mYRange.max) return;
			
			int i = int((x-mXRange.min)*double(mColorData.size())/mXRange.Span());
			int j = int((y-mYRange.min)*double(mColorData[0].size())/mYRange.Span());
			
			if(i < 0 || i >= (int)mColorData.size()) return;
			if(j < 0 || j >= (int)mColorData[0].size()) return;
			
			emit labels(QString::number(y,'f',0),
						QString::number(mComputedData[i][j],'f',0),
						QString::number(i),
						QString::number(x,'f',2));
		}

		void SpectrogramRenderer::LeaveEvent()
		{
			emit labels("--","--","--","--");
		}

		void SpectrogramRenderer::DrawColorData()
		{
			if(!mColorData.size()) return;
			if(!mColorData[0].size()) return;
			if(!mglColorList)
			{
				mglColorList = GetglList();
				glNewList(mglColorList,GL_COMPILE_AND_EXECUTE);
				for(unsigned i = 0; i < mColorData[i].size(); i++)
				{
					glBegin(GL_QUAD_STRIP);
					unsigned k = 0;
					glVertex2d(GLdouble(k),GLdouble(i));
					glVertex2d(GLdouble(k),GLdouble(i+1));
					for(unsigned j = 0; j < mColorData.size(); j++,k++)
					{
						glColor3ub(mColorData[j][i].red(),mColorData[j][i].green(),mColorData[j][i].blue());
						glVertex2d(GLdouble(k+1),GLdouble(i));
						glVertex2d(GLdouble(k+1),GLdouble(i+1));
					}
					glEnd();
				}
				glEndList();
			}
			else
			{
				glCallList(mglColorList);
			}
		}

		void SpectrogramRenderer::DrawBlackWhiteData()
		{
			if(!mBlackWhiteData.size()) return;
			if(!mBlackWhiteData[0].size()) return;
			if(!mglBlackWhiteList)
			{
				mglBlackWhiteList = GetglList();
				glNewList(mglBlackWhiteList,GL_COMPILE_AND_EXECUTE);
				for(unsigned i = 0; i < mBlackWhiteData[i].size(); i++)
				{
					glBegin(GL_QUAD_STRIP);
					unsigned k = 0;
					glVertex2d(GLdouble(k),GLdouble(i));
					glVertex2d(GLdouble(k),GLdouble(i+1));
					for(unsigned j = 0; j < mBlackWhiteData.size(); j++,k++)
					{
						glColor3ub(mBlackWhiteData[j][i].red(),mBlackWhiteData[j][i].green(),mBlackWhiteData[j][i].blue());
						glVertex2d(GLdouble(k+1),GLdouble(i));
						glVertex2d(GLdouble(k+1),GLdouble(i+1));
					}
					glEnd();
				}
				glEndList();
			}
			else
			{
				glCallList(mglBlackWhiteList);
			}
		}
	}
}

// END

