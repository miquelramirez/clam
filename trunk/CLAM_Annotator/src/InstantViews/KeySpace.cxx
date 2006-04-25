
#include "KeySpace.hxx"
#include <cmath>

struct TKeyNode
{
	float x;
	float y;
};

TKeyNode * getKeyNodes()
{
	static TKeyNode keyNodes[] = 
	{
		{0,0}, {0,1}, {0,2}, {0,3}, {0,4}, {0,5},
		{1,0}, {1,1}, {1,2}, {1,3}, {1,4}, {1,5},
		{2,0}, {2,1}, {2,2}, {2,3}, {2,4}, {2,5},
		{3,0}, {3,1}, {3,2}, {3,3}, {3,4}, {3,5}
	};
	return keyNodes;
}
unsigned nKeyNodes=24;
char ** getKeyNames()
{
	static char *names[]= {
		"A",
	};
	return names;
}


inline void Line(float width, float x1, float y1, float x2, float y2)
{
	glLineWidth(width);

	glBegin(GL_LINES);
		glVertex2f(x1,y1);
		glVertex2f(x2,y2);
	glEnd();
}

inline void Rectangle(float x, float y, float w, float h)
{
	glBegin(GL_QUADS);
		glVertex2f( x,   y );
		glVertex2f( x+w, y );
		glVertex2f( x+w, y+h );
		glVertex2f( x,   y+h );
	glEnd();
}



KeySpace::KeySpace() 
	: KeySpaceViewPort()
{
	x_res = 1;
	y_res = 1;

	centroidx_ = 0;
	centroidy_ = 0; 
	memset(pKeySpaceValue_,0,sizeof(float)*24);

	ColorsIndex[0] = 0; 
	ColorsIndex[1] = 30;
	ColorsIndex[2] = 70; 
	ColorsIndex[3] = 110;
	ColorsIndex[4] = 145;
	ColorsIndex[5] = 200;
	pRColor[ColorsIndex[0]] = 0;//RGB(0,0,0);
	pRColor[ColorsIndex[1]] = 58/4;//RGB(58/4,68/4,65/4);
	pRColor[ColorsIndex[2]] = 80;//RGB(80,100,153);
	pRColor[ColorsIndex[3]] = 90;//RGB(90,180,100);
	pRColor[ColorsIndex[4]] = 224;//RGB(224,224,44);
	pRColor[ColorsIndex[5]] = 255;//RGB(255,60,30);//RGB(255,155,80);
	pGColor[ColorsIndex[0]] = 0;//RGB(0,0,0);
	pGColor[ColorsIndex[1]] = 68/4;//RGB(58/4,68/4,65/4);
	pGColor[ColorsIndex[2]] = 100;//RGB(80,100,153);
	pGColor[ColorsIndex[3]] = 180;//RGB(90,180,100);
	pGColor[ColorsIndex[4]] = 224;//RGB(224,224,44);
	pGColor[ColorsIndex[5]] = 60;//RGB(255,60,30);//RGB(255,155,80);
	pBColor[ColorsIndex[0]] = 0;//RGB(0,0,0);
	pBColor[ColorsIndex[1]] = 65/4;//RGB(58/4,68/4,65/4);
	pBColor[ColorsIndex[2]] = 153;//RGB(80,100,153);
	pBColor[ColorsIndex[3]] = 100;//RGB(90,180,100);
	pBColor[ColorsIndex[4]] = 44;//RGB(224,224,44);
	pBColor[ColorsIndex[5]] = 30;//RGB(255,60,30);//RGB(255,155,80);
	int n = 0;
	int nDif = 0;
	int i,k;
	for(k=0; k<200; k+=nDif,n++)
	{
		nDif = ColorsIndex[n+1] - ColorsIndex[n];
		for(i=1; i<nDif; i++)
		{
			float Factor = (float)i / nDif;
			/*unsigned char R = GetRValue(pColor[k]) + (GetRValue(pColor[k+nDif]) - GetRValue(pColor[k]))*Factor;
			unsigned char G = GetGValue(pColor[k]) + (GetGValue(pColor[k+nDif]) - GetGValue(pColor[k]))*Factor;
			unsigned char B = GetBValue(pColor[k]) + (GetBValue(pColor[k+nDif]) - GetBValue(pColor[k]))*Factor;*/
			float R = pRColor[k] + (pRColor[k+nDif] - pRColor[k])*Factor;
			float G = pGColor[k] + (pGColor[k+nDif] - pGColor[k])*Factor;
			float B = pBColor[k] + (pBColor[k+nDif] - pBColor[k])*Factor;
			pRColor[k+i] = R;
			pGColor[k+i] = G;
			pBColor[k+i] = B;
		}
	}
	for(k=0; k<=200; k++)
	{
		pRColor[k] /= 255.;
		pGColor[k] /= 255.;
		pBColor[k] /= 255.;
	}
}

void KeySpace::update()
{
}

void KeySpace::display()
{
	y_res *= 2;
	KeySpaceViewPort::display();
	y_res /= 2;

	int nX = w / 4; // 4 pixels
	int nY = h / 4; // 4 pixels
	nX = 150;
	nY = 100;

	TKeyNode *pKeyNodes = getKeyNodes();

	int i,k,m;
	for(i=0; i<nX; i++)
	{
		float x1 = i / float(nX) * x_res;
		float x2 = (i+1) / float(nX) * x_res;
		for(k=0; k<nY; k++)
		{
			float y1 = k / float(nY) * y_res;
			float y2 = (k+1) / float(nY) * y_res;
			double value = 0.;
			double num = 0.;
			double den = 0.;
			for(m=0; m<nKeyNodes; m++)
			{
				double d1 = wdist(x1,pKeyNodes[m].x);
				double d2 = wdist(y1,pKeyNodes[m].y);
				double dist = d1*d1+d2*d2;											   
				double g = dist*dist;
				if (g < 1E-5)
					g = 1E-5;
				double weight = 1. / g;
				num += pKeySpaceValue_[m] * weight;
				den += weight;
			}
			if (den != 0.)
				value = num / den;

			float ColorIndex = value;
			if (ColorIndex > 1.0)
				ColorIndex = 1.0;	
			ColorIndex *= ColorIndex;
			ColorIndex = ColorIndex * 200.f;
			/*COLORREF color = pColor[(int)ColorIndex];
			float R = GetRValue(color)/255.f;
			float G = GetGValue(color)/255.f;
			float B = GetBValue(color)/255.f;
			glColor3d(R,G,B);*/
			int cidx = floorf(ColorIndex);
			glColor3d(pRColor[cidx],pGColor[cidx],pBColor[cidx]);
			Rectangle(x1,y_res+y1,x2-x1,y2-y1);
		}
	}

	// draw strings
	char **pKeyNames = getKeyNames();
	for(i=0; i<nKeyNodes; i++)
	{
		float x1 = pKeyNodes[i].x * x_res;
		float y1 = pKeyNodes[i].y * y_res;

		float value = pKeySpaceValue_[i]; 
		float ColorIndex = value;
		if (ColorIndex > 1.0)
			ColorIndex = 1.0;	
		ColorIndex *= ColorIndex;
		ColorIndex = ColorIndex * 200.f;
		int cidx = floorf(ColorIndex);
		float br = (pRColor[cidx]+pGColor[cidx]+pBColor[cidx]) / 3.;
		float invbr = 0;
		if (br < 0.4)        
			invbr = 1.; 
		glColor3d(invbr,invbr,invbr);
		int l = strlen(pKeyNames[i]);
		if (y1 < 4.*y_res/nY)
			y1 = 4.*y_res/nY;
		renderText(x1, y_res+y1, -1, pKeyNames[i]);
	}
}

