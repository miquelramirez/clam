#ifndef _KEYSPACE_
#define _KEISPACE_

#include "KeySpaceViewPort.hxx"
                                                                                                             
class KeySpace : public KeySpaceViewPort
{
public:
	KeySpace();

	void update();
	void display();

	void update_KeySpace(float *pKeySpaceValue,float centroidx,float centroidy)
	{
		memcpy(pKeySpaceValue_,pKeySpaceValue,sizeof(float)*24);
		centroidx_ = centroidx;
		centroidy_ = centroidy;
	}

	double wdist(double x1,double x2)
	{
		if (x2 > x1+.5)
			return 1. - (x2-x1);
		else
		if (x2 < x1-.5)
			return 1. - (x1-x2);
		else
		if (x2 >= x1)
			return x2-x1;
		else
			return x1-x2;
	}

	float centroidx_,centroidy_;
	float pKeySpaceValue_[24];
	float pRColor[201],pGColor[201],pBColor[201];
	int ColorsIndex[6];
};

#endif
