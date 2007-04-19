#include <CLAM/CLAMGL.hxx>
#include <CLAM/SpectrogramRenderer.hxx>

namespace CLAM
{
    namespace VM
    {
		SpectrogramRenderer::SpectrogramRenderer()
			: mLeftIndex(0)
			, mRightIndex(0)
			, mBottomIndex(0)
			, mTopIndex(0)
			, mColorMap(CLAM::VM::ColorSonogram)
		{
		}

		SpectrogramRenderer::~SpectrogramRenderer()
		{
		}

	    
		void SpectrogramRenderer::SetIndexes(const TIndex& leftIndex, const TIndex& rightIndex,
											 const TIndex& bottomIndex, const TIndex& topIndex)
		{
			mLeftIndex = leftIndex;
			mRightIndex = rightIndex;
			mBottomIndex = bottomIndex;
			mTopIndex = topIndex;
		}

		void SpectrogramRenderer::SetColorData(std::vector< std::vector<Color> >& cdata)
		{
			mColorData.clear();
			mColorData = cdata;
		}

		void SpectrogramRenderer::SetBWData(std::vector< std::vector<Color> >& bwdata)
		{
			mBWData.clear();
			mBWData = bwdata;
		}

		void SpectrogramRenderer::Render()
		{
			if(!mColorData.size() && !mBWData.size()) return;

			if(!mReadPixels)
			{
				DrawPixels();
				return;
			}

			switch(mColorMap)
			{
				case CLAM::VM::ColorSonogram:
					DrawColorData();
					break;
				case CLAM::VM::BlackWhiteSonogram:
					DrawBWData();
					break;
				default:
					break;
			}
		   
			ReadPixels();
		}

		void SpectrogramRenderer::SetRenderingMode(CLAM::VM::SonogramCM colorMap)
		{
			mColorMap = colorMap;
		}

		void SpectrogramRenderer::DrawColorData()
		{
			glMatrixMode(GL_PROJECTION);
			glPushMatrix();
			glLoadIdentity();
			glOrtho(LeftBound(),RightBound(),BottomBound(),TopBound(),-1.0,1.0);
			glMatrixMode(GL_MODELVIEW);
			
			for(TIndex i = mBottomIndex; i < mTopIndex; i++)
			{
				glBegin(GL_QUAD_STRIP);
				TIndex k=0;
				glVertex2f(GLfloat(k),GLfloat(i));
				glVertex2f(GLfloat(k),GLfloat(i+1));
				for(TIndex j = mLeftIndex; j < mRightIndex; j++,k++)
				{
					glColor3ub(GLubyte(mColorData[j][i].r),GLubyte(mColorData[j][i].g),GLubyte(mColorData[j][i].b));
					glVertex2f(GLfloat(k+1),GLfloat(i));
					glVertex2f(GLfloat(k+1),GLfloat(i+1));
				}
				glEnd();
			}

			glMatrixMode(GL_PROJECTION);
			glPopMatrix();
			glMatrixMode(GL_MODELVIEW);
			glShadeModel( GL_FLAT );
		}

		void SpectrogramRenderer::DrawBWData()
		{
			glMatrixMode(GL_PROJECTION);
			glPushMatrix();
			glLoadIdentity();
			glOrtho(LeftBound(),RightBound(),BottomBound(),TopBound(),-1.0,1.0);
			glMatrixMode(GL_MODELVIEW);
			glClear(GL_COLOR_BUFFER_BIT);


			for(TIndex i = mBottomIndex; i < mTopIndex; i++)
			{
				glBegin(GL_QUAD_STRIP);
				TIndex k=0;
				glVertex2f(GLfloat(k),GLfloat(i));
				glVertex2f(GLfloat(k),GLfloat(i+1));
				for(TIndex j = mLeftIndex; j < mRightIndex; j++,k++)
				{
					glColor3ub(GLubyte(mBWData[j][i].r),GLubyte(mBWData[j][i].g),GLubyte(mBWData[j][i].b));
					glVertex2f(GLfloat(k+1),GLfloat(i));
					glVertex2f(GLfloat(k+1),GLfloat(i+1));
				}
				glEnd();
			}

			glMatrixMode(GL_PROJECTION);
			glPopMatrix();
			glMatrixMode(GL_MODELVIEW);
			glShadeModel( GL_FLAT );
		}
    }
}

// END

