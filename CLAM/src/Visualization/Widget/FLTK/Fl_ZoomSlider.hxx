#ifndef __FL_ZOOMSLIDER__
#define __FL_ZOOMSLIDER__

#include <FL/Fl_Widget.H>
#include <FL/Fl_Valuator.H>
#include "Signalv2.hxx"
#include <cmath>

namespace CLAMVM
{
		using SigSlot::Signalv2;

		class Fl_ZoomSlider : public Fl_Widget
		{
				/**
				   Slider widget that can be used for both zooming and scrolling.
				   The thumb of the slider is divided into 3 parts, the top and bottom
				   for adjusting the scale factor, the middle for scrolling.
				 */

		public:

				inline float GetScale()
				{
						return scale_;
				}

				inline float GetOffset()
				{
						return offset_;
				}

				inline void SetScale( float value )
				{
						scale_ = value;
						UpdateThumbParts();
				}

				inline void SetOffset( float value )
				{
						offset_ = value;
						UpdateThumbParts();
				}

				Fl_ZoomSlider( int X, int Y, int W, int H, int orientation );

				virtual ~Fl_ZoomSlider();

				void draw();

				int handle( int evt );

				Signalv2<double, double> SpanChanged;
				/**
				   Resize the slider, and update the thumb positions
				 */
				void resize( int X, int Y, int W, int H );

		protected:
				/**
				   Calculate scale and offset from the physical positions of the thumb-parts
				 */
				void UpdateScaleAndOffset()
				{
					if (type()==FL_HORIZONTAL) 
					{
							scale_=float(mLastPixel-mFirstPixel+1)/float(w()-2);
							offset_=float(mFirstPixel-1)/float(w()-2);
					}
					else
					{
							scale_=float(mLastPixel-mFirstPixel+1)/float(h()-2);
							offset_=float(h()-2-mLastPixel)/float(h()-2);		
					}
					
				}
				/**
				   Calculate the physical positions of the thumb-parts for scale and offset
				*/
				void UpdateThumbParts()
				{
						if (type()==FL_HORIZONTAL) 
						{
								mFirstPixel=int(offset_*float(w()-2))+1;
								mLastPixel=int((scale_)*float(w()-2))+mFirstPixel-1;
								mPixelLen = mLastPixel - mFirstPixel;
						}
						else
						{
								mLastPixel=h()-2-int(offset_*float(h()-2));		
								mFirstPixel=mLastPixel+1-int((scale_)*float(h()-2));
								mPixelLen = mLastPixel - mFirstPixel;

						}

				}

				void UpdatePixelLen()
				{			

					if ( mOrientation == FL_HORIZONTAL )
							mPixelLen = fabs( double(h() - y()) ) -offset_*2;
					else if ( mOrientation == FL_VERTICAL )
							mPixelLen = fabs( double(w() - x()) ) - offset_ * 2;
						
				}


		private:
				float scale_;
				float offset_;
				int mFirstPixel, mLastPixel;
				int mOrientation; // either FL_HORIZONTAL or FL_VERTICAL
				int prev;
				int dragging;
				float mPixelLen;
		};
		
}


#endif // Fl_ZoomSlider.hxx
