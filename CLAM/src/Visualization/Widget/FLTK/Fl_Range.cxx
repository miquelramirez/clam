#include "Fl_Range.hxx"
#include <FL/fl_draw.H>
#include <cmath>
#include <iostream>

namespace CLAMGUI
{
		Fl_Range::Fl_Range( int x, int y, int w, int h, int orientation )
				: Fl_Widget( x, y, w, h, "" ),
				  mLowerBound(0), mUpperBound(0), mPixelLen(0), mFontType( FL_COURIER_BOLD )
		{
				box( FL_FLAT_BOX );
				type(orientation);
				resize( 0, 0, w, h );
		}

		Fl_Range::~Fl_Range()
		{
		}

		void Fl_Range::SetRangeNotifier( Signalv2<float,float>& sig )
		{
				sig.Connect( this, &Fl_Range::SetRange, mRangeSlot );
		}

		void Fl_Range::SetSpanNotifier( Signalv2<float,float>& sig )
		{
				sig.Connect( this, &Fl_Range::SetSpan, mSpanSlot );
		}

		void Fl_Range::SetRange( float lowerBound, float upperBound )
		{
				mLowerBound = lowerBound;
				mUpperBound = upperBound;
				mDistance = mUpperBound - mLowerBound;
				CalculateDrawingConstants();
		}

		void Fl_Range::SetSpan( float left, float right ) 
		{
				mLeft = left;
				mRight = right;

				// we clip the span to the current range
				if ( mLeft < mLowerBound )
						mLeft = mLowerBound;
				if ( mRight > mUpperBound )
						mRight = mUpperBound;
		}

		void Fl_Range::CalculateDrawingConstants()
		{
				double mul;

				valuePerPixel = fabs(mDistance)/mPixelLen; 
				pixelsPerValue = mPixelLen/fabs(mUpperBound - mLowerBound); // number of pixels per position increment
				/* we can draw a number at most every 30 pixels. we calculate how often that is */
				every = valuePerPixel*30.; // this value has to be rounded to a nice divider
				std::cout << "every: " << every << std::endl;

				mDecimalDigits = -int(log10(every)); // number of decimals
				std::cout << "mDecimalDigits: " << mDecimalDigits << std::endl;

				if (every<1) mDecimalDigits++;
				
				mul = pow(double(10),double(-mDecimalDigits));
				std::cout << "mul = pow( 10, " << -mDecimalDigits  << " ) = " << mul << std::endl;

				if (every>5*mul) 
				{
						mDecimalDigits--;
						mul*=10;
						std::cout << "every was > than 5*mul "
								  << "mDecimalDigits: " << mDecimalDigits 
								  << "mul: " << mul << std::endl;
				}
	
				if (every<=1*mul)
				{
						every = 1*mul;
						std::cout << "every was <= 1*mul "
								  << "every: " << every
								  << "mul: " << mul << std::endl;
				}
				else 
				{
						every = 5*mul;
						std::cout << "every was between 1*mul and 5*mul "
								  << "every: " << every 
								  << "mul: " << mul << std::endl;
				}
				
				if (mDecimalDigits<0) 
						mDecimalDigits=0;
				// every now is a nice divider for showing ruler numbers
				
				every/=10; // we want 10 lines per number
				
				dp = every*pixelsPerValue; // every dp pixels we draw a line
			
		}

		void Fl_Range::draw( void )
		{
				fl_push_clip(x(),y(),w(),h());
				
				int maxcnt = 10;
				int longercnt = 1;

				draw_box();
				fl_color(FL_BLACK);
				fl_font(mFontType,9);				
	
				double p,v;
				p = mLeft;
				if (mRight>0) 
				{
						v = ((int)(p / every + 0.5)) * every;
						p = int((v - p)*pixelsPerValue)+(type()==FL_HORIZONTAL?x():y())+0.5;
				}
				else
				{
						v = ((int)(p / every - 0.5)) * every;
						p = int((p - v)*pixelsPerValue)+(type()==FL_HORIZONTAL?x():y())+0.5;
				}
				
				/* we keep a counter to decide when to draw a number, and longer lines */
				int cnt; 
				double tmp = v/every;
				if (mRight<0) 
						tmp = -tmp;
				
				if (tmp<0) 
				{
						cnt = maxcnt - 1 - int(-0.5-tmp)%maxcnt;
				}
				else 
				{
						cnt = int(tmp)%maxcnt;	
				}
				
				if (type()==FL_HORIZONTAL)
				{
						mPixelLen = x()+mPixelLen;
				}
				else
				{
						mPixelLen = y()+mPixelLen;
				}
				
				if (mRight<0) 
						every=-every;
				
				int first = 1;
				float extra = every/100.;
	
				if (extra<0) 
						extra=-extra;
				char fmt[256];
				char txt[256];
				
				while (p<mPixelLen+30) 
				{
						int to;
						
						if (cnt==maxcnt) 
								cnt=0;
						
						if (cnt==0) 
						{
								sprintf(fmt,"%%.%df",int(mDecimalDigits));
//								sprintf(txt,fmt,v+extra);
								sprintf(txt,fmt,v);
								fl_color(FL_BLACK);
								
								if (type()==FL_HORIZONTAL) 
								{
										fl_draw(txt,int(p)+4,y()+20);
										
										if (first) 
										{
												sprintf(txt,fmt,v-every*10+every/100.);
												fl_draw(txt,int(p-dp*10.)+4,y()+20);
												first = 0;
										}
								} 
								else
								{
										fl_draw(txt,x()+8,int(p)-2);			
								}
								to = 16;
						}
						else
						{
								to = 8-((cnt&longercnt)<<2);
						}
						
// Ticks drawing
						if (type()==FL_HORIZONTAL)
						{
								fl_color(FL_BLACK);
								fl_yxline(int(p)-1,y(),y()+to);
								fl_yxline(int(p),y(),y()+to);
						} 
						else
						{
								fl_color(FL_BLACK);
								fl_xyline(x(),int(p)-1,x()+to);		
								fl_xyline(x(),int(p),x()+to);		
						}
						p+=dp;
						v+=every;
						cnt++;
				}
				
				fl_pop_clip();
		}
}
