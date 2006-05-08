#ifndef _KEYSPACE_
#define _KEISPACE_

#include "PcpTorus.hxx"
namespace CLAM {
namespace VM {

class KeySpace : public PcpTorus
{
protected:
	float x_res, y_res;

public:
	KeySpace(QWidget * parent);

	virtual void initializeGL();
	virtual void resizeGL(int width, int height);
	virtual void paintGL();
private:
	void DrawTiles();
	void DrawLabels();
	double wdist(double x1,double x2)
	{
		if (x2 > x1+.5) return 1. - (x2-x1);
		if (x2 < x1-.5) return 1. - (x1-x2);
		if (x2 >= x1)   return x2-x1;
		else            return x1-x2;
	}

	float centroidx_,centroidy_;
	float pKeySpaceValue_[24];
	float pRColor[201],pGColor[201],pBColor[201];
	int ColorsIndex[6];
};

}
}

#endif
