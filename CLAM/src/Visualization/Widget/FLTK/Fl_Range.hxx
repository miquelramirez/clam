#ifndef __FL_RANGE__
#define __FL_RANGE__

#include <FL/Fl_Slider.H> // needed for FL_HORIZONTAL definition
#include <FL/Fl_Widget.H>
#include "Signalv2.hxx"
#include "Slot.hxx"

namespace CLAMGUI
{
		class Fl_Range : public Fl_Widget
		{
		public:
				
				Fl_Range( int x, int y, int w, int h, int orientation );
				
				~Fl_Range();
				
				void draw();
				
				void resize( int newx, int newy, int neww, int newh )
				{
						Fl_Widget::resize(newx,newy,neww,newh);
						
						if ( type() == FL_HORIZONTAL )
								mPixelLen = float( w() );
						else
								mPixelLen = float( h() );

						CalculateDrawingConstants();
				}

				virtual void SetRangeNotifier( SigSlot::Signalv2<float,float>& sig );

				virtual void SetSpanNotifier( SigSlot::Signalv2<float,float>& sig );

				virtual void SetRange( float lowerBound, float upperBound );

				virtual void SetSpan( float left, float right );

				inline void GetRange( float& low, float& hi) const
				{
						low = mLowerBound;
						hi = mUpperBound;
				}

				inline void GetSpan( float& left, float& right ) const
				{
						left = mLeft;
						right = mRight;
				}

				inline void SetFontType( int flFont )
				{
						mFontType = flFont;
				}

		protected:
				
				void CalculateDrawingConstants();
				
		private:
				
				float mLowerBound;
				float mUpperBound;
				float mDistance;  // separation between the lower bound and the upper bound
				float mPixelLen;
				SigSlot::Slot  mRangeSlot;
				SigSlot::Slot  mSpanSlot;
				int   mFontType;
				float valuePerPixel; // position increment per pixel
				float pixelsPerValue; // pixels "occupied" by each value
				float dp; // pixel spacing between two values
				float every;
				float mDecimalDigits;
				float mLeft;
				float mRight; // mLeft and mRight express the visible range values span
		};
}

#endif // Fl_Range.hxx
