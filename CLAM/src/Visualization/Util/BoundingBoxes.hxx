#ifndef __BOUNDINGBOXES__
#define __BOUNDINGBOXES__

namespace CLAMVM
{

struct DataBoundBox
{
	float mLeft;
	float mRight;
	float mBottom;
	float mTop;
	float mNear;
	float mFar;				
};

struct ScreenBoundBox
{
	int pixel_left;
	int pixel_top;
	int pixel_width;
	int pixel_height;

	inline bool IsPointInside( int ex, int ey ) {

		return ( ex > pixel_left && ex < (pixel_left + pixel_width) ) 
			&& ( ey > pixel_top && ey < ( pixel_top + pixel_height ) );
	}
};

}

#endif // BoundingBoxes.hxx
