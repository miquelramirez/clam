#ifndef __FL_ZOOMSLIDER__
#define __FL_ZOOMSLIDER__

#include <FL/Fl_Widget.H>
#include <FL/Fl_Valuator.H>

namespace CLAMGUI
{

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
						update2();
				}

				inline void SetOffset( float value )
				{
						offset_ = value;
						update2();
				}

				Fl_ZoomSlider( int X, int Y, int W, int H, int orientation );

				virtual ~Fl_ZoomSlider();

				void draw();

				int handle( int evt );

		protected:
				/**
				   Calculate scale and offset from the physical positions of the thumb-parts
				 */
				void UpdateScaleAndOffset()
				{
					if (type()==FL_HORIZONTAL) 
					{
							scale_=float(v2-v1+1)/float(w()-2);
							offset_=float(v1-1)/float(w()-2);
					}
					else
					{
							scale_=float(v2-v1+1)/float(h()-2);
							offset_=float(h()-2-v2)/float(h()-2);		
					}
					
				}
				/**
				   Calculate the physical positions of the thumb-parts for scale and offset
				*/
				void UpdateThumbParts()
				{
						if (type()==FL_HORIZONTAL) 
						{
								v1=int(offset_*float(w()-2))+1;
								v2=int((scale_)*float(w()-2))+v1-1;
						}
						else
						{
								v2=h()-2-int(offset_*float(h()-2));		
								v1=v2+1-int((scale_)*float(h()-2));
						}

				}

				/**
				   Resize the slider, and update the thumb positions
				 */
				void resize( int X, int Y, int W, int H )
				{
						Fl_Widget::resize(X,Y,W,H);
						UpdateThumbParts();
				}

		private:
				float scale_;
				float offset_;
				int v1,v2;

				int mOrientation; // either FL_HORIZONTAL or FL_VERTICAL
				
		};
		
}


#endif // Fl_ZoomSlider.hxx
