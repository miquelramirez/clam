/*
 * Copyright (c) 2001-2002 MUSIC TECHNOLOGY GROUP (MTG)
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

//#include <stdio.h>

/*
#include "BPFView.hxx"
#include "Builder.hxx"

#define function_left_limit 0
#define function_top_limit 100
#define function_right_limit 2000
#define function_bottom_limit 0*/

#include "Point.hxx"
#include "BPF.hxx"
#include "GlobalEnums.hxx"
#include <vector>
#include <time.h>
//#include "XMLTestHelper.hxx"

using namespace CLAM;

float GetXValue(BPF& testBPF,int index);
float GetValue(BPF& testBPF,float xValue);
float GetValueFromIndex(BPF& testBPF,int index);
void DumpOrderedBPFPoints(FILE* pFile, BPF &bpf, const char* title);
void DumpRandomBPFPoints(FILE* pFile, BPF &bpf, const char* title);
void DumpOrderedBPF(FILE* pFile, BPF &bpf, const char* title, unsigned int points=0);
void DumpRandomBPF(FILE* pFile, BPF &bpf, const char* title, unsigned int points=0);

typedef enum {
	eLinear,
	eRandom
} ETestType;

void SetTestBPF(BPF &bpf, ETestType,int size=20);


int main()
{

try{

	EInterpolation testInt=EInterpolation::eLinear;
	srand((unsigned)time(NULL)); 
	int size = 50;
	int points = 500;

	BPF testBPF;

	FILE *pFile;
	pFile = fopen( "BPFText.txt", "w" );

	fprintf(pFile,"\n# ***************************BPF TEST**************************\n\n\n");
	fprintf(pFile,"\n# ----------------------Inserting points in order---------------\n\n\n");
	fprintf(pFile,"\n# Inserting %d points\n",size);
	/*Note the Y values will always be between 0 and 1*/

	SetTestBPF(testBPF,eLinear,size);

	DumpOrderedBPFPoints(pFile,testBPF,"Points inserted to the BPF");

	DumpOrderedBPF(pFile,testBPF,"Linear Interpolation: Getting points in order",points);
	DumpRandomBPF (pFile,testBPF,"Linear Interpolation: Getting points in random order",points);

	testInt=EInterpolation::ePolynomial2;
	testBPF.SetIntpType(testInt);
	DumpOrderedBPF(pFile,testBPF,"Parabolic Interpolation",points);

	testInt=EInterpolation::ePolynomial3;
	testBPF.SetIntpType(testInt);
	DumpOrderedBPF(pFile,testBPF,"3rd Order Polynomial Interpolation",points);

	testInt=EInterpolation::eSpline;
	testBPF.SetIntpType(testInt);
	testBPF.UpdateSplineTable();
	DumpOrderedBPF(pFile,testBPF,"3rd Order Spline Interpolation",points);

	/*Now let's see what happens inserting random points*/
	fprintf(pFile,"\n# --------------------Inserting points in random order--------------\n\n\n");
	BPF testBPF2;

	SetTestBPF(testBPF2,eRandom,size);

	DumpOrderedBPFPoints(pFile,testBPF2,"Points inserted to the BPF");

	DumpOrderedBPF(pFile,testBPF2,"Linear Interpolation: Getting points in order",points);
	DumpRandomBPF(pFile,testBPF2,"Linear Interpolation: Getting points in random order",points);

	testInt=EInterpolation::ePolynomial2;
	testBPF2.SetIntpType(testInt);
	DumpOrderedBPF(pFile,testBPF2,"Parabolic Interpolation",points);

	testInt=EInterpolation::ePolynomial3;
	testBPF2.SetIntpType(testInt);
	DumpOrderedBPF(pFile,testBPF2,"3rd Order Polynomial Interpolation",points);

	testInt=EInterpolation::eSpline;
	testBPF2.SetIntpType(testInt);
	testBPF2.UpdateSplineTable();
	DumpOrderedBPF(pFile,testBPF2,"3rd Order Spline Interpolation",points);


	/* Now we test the spline a bit harder... */

	testBPF2.SetLeftDerivative(0.1);
	testBPF2.SetRightDerivative(0.1);
	testBPF2.UpdateSplineTable();
	DumpOrderedBPF(pFile,testBPF2,"3rd Order Spline Interpolation with flat borders",points);
/*
	// if you want to view your BPF, add the necessary files and...
	BPFView myBPFView;
	BPFViewConfig cfg;
	cfg.SetLabel( "BPF WATCHER");
	cfg.SetScreenArea( CLAM::Recti(100, 100, 300, 240 ) );
	// function_left_limit
	cfg.SetFOV( FOV( function_left_limit, function_top_limit, function_right_limit, function_bottom_limit ) ); 
	myBPFView.BindWith( &testBPF2, cfg );

	Builder*  pb = CLAM::Builder::GetInstance("FLTK");
	GUIObject* gui = pb->GetGUI();

	do 
	{
		gui->Refresh(0.0);
	}while(!gui->End());
 */



	fclose(pFile);
	}
	catch(Err err) {
		err.Print();
		return 1;
	}
	return 0;
}

void SetTestBPF(BPF &testBPF,ETestType test, int size)
{
	int n,index;
	float xValue,yValue;
	std::vector<TData> values(size);
	TData delta = 1.0/(size-1);
	TData pos=0.0;


	testBPF.SetSize(0);
	testBPF.Resize(0);

	switch (test) {
	case eLinear:

		for(n=0;n<size;n++)
		{
			testBPF.Insert(float(n)/float(size),float(n)/size);
		}
		break;

	case eRandom:
		for(n=0; n<size; n++) {
			values[n]=pos;
			pos += delta;
		}

		for(n=0;n<size;n++)
		{
			index = int((size-n-1)*double(rand())/double(RAND_MAX));
			xValue=values[index];
			yValue = float(rand())/float(RAND_MAX);
			testBPF.Insert(xValue,yValue);
			values.erase(values.begin()+index);
		}
		break;

	default:
		fprintf(stderr,"SetTestBPF: No such test, %d\n",test);
		exit(1);
	}
}

// Test output generation functions

void DumpOrderedBPFPoints(FILE* pFile, BPF &bpf, const char* title)
{
	int n;
	float xValue, yValue;
	int size = bpf.Size();

	fprintf(pFile,"\n# %s\n",title);
	for(n=0;n<size;n++)
	{
		xValue=GetXValue(bpf,n);
		yValue=GetValueFromIndex(bpf,n);
		fprintf(pFile,"%f %f\n",xValue,yValue);
	}
	fprintf(pFile,"\n\n");
	CLAM_ASSERT(CLAMTest::XMLInputOutputMatches( bpf, "BPFTest.xml"),
			"XML output of a BPF failed");
}

void DumpRandomBPFPoints(FILE* pFile, BPF &bpf, const char* title)
{
	int n;
	float xValue,result;
	int size = bpf.Size();

	fprintf(pFile,"\n# %s\n",title);
	for(n=0;n<size ;n++)
	{
		xValue=int(double((size-1))*double(rand())/double(RAND_MAX));
		result=GetValue(bpf,xValue);
		fprintf(pFile,"%f %f\n",xValue,result);
	}
	fprintf(pFile,"\n\n");
}

void DumpOrderedBPF(FILE* pFile, BPF &bpf, const char* title, unsigned int points)
{
	double f,result,delta;
	int size = bpf.Size();

	if (points == 0)
		delta=0.25/double(size-1);
	else
		delta=1.0/(points-1);

	fprintf(pFile,"\n# %s\n",title);
	for(f=0;f<=1.0;f+=delta)
	{
		result=GetValue(bpf,f);
		fprintf(pFile,"%f %f\n",f,result);
	}
	fprintf(pFile,"\n\n");
}

void DumpRandomBPF(FILE* pFile, BPF &bpf, const char* title, unsigned int points)
{
	unsigned int n;
	double result,xValue;
	int size = bpf.Size();

	if (points == 0)
		points = size * 4;

	fprintf(pFile,"\n# %s\n",title);
	for(n=0; n<points; n++)
	{
		xValue = float(double(rand())/double(RAND_MAX));
		result=GetValue(bpf,xValue);
		fprintf(pFile,"%f,",xValue);
		fprintf(pFile,"%f\n",result);
	}
	fprintf(pFile,"\n\n");
}




/*Here all BPF methods are reimplemented with a concrete type
so they are really compiled under windows*/


TSize AllocatedSize(BPF& testBPF) {return testBPF.AllocatedSize();}
void DeleteBetweenIndex(BPF& testBPF, TIndex leftIndex,TIndex rightIndex)
	{testBPF.DeleteBetweenIndex(leftIndex,rightIndex);}
void DeleteBetweenXValues(BPF& testBPF,float leftX,float leftY)
	{testBPF.DeleteBetweenXValues(leftX,leftY);}
void DeleteIndex(BPF& testBPF,TIndex index){testBPF.DeleteIndex(index);}
void DeleteThroughXValue(BPF& testBPF,float x){testBPF.DeleteThroughXValue(x);}
const EInterpolation	&GetInterpolation(BPF& testBPF) 
	{return testBPF.GetInterpolation(); }
void GetnClosest(BPF& testBPF,TIndex i)
	{testBPF.GetnClosest(i);}
Array<Point> &GetPointArray( BPF& testBPF)
	{return testBPF.GetPointArray();}
TIndex GetPosition(BPF& testBPF,float x) {return testBPF.GetPosition(x);}
TSize GetStep(BPF& testBPF) {return testBPF.GetStep();}
float GetValue(BPF& testBPF,float xValue)
	{return testBPF.GetValue(xValue);}
float GetValue (BPF& testBPF,float x,const EInterpolation& eInterpolation)
	{return testBPF.GetValue(x,eInterpolation);}
float GetValueFromIndex(BPF& testBPF,int index)
	{return testBPF.GetValueFromIndex(index);}
float GetXValue(BPF& testBPF,int index)
	{return testBPF.GetXValue(index);}
void Init(BPF& testBPF){testBPF.Init();}
void Insert(BPF& testBPF,const Point &point)
	{testBPF.Insert(point);}
void Insert(BPF& testBPF,float x,float y){testBPF.Insert(x,y);}
void Resize (BPF& testBPF,TSize newSize){testBPF.Resize(newSize);}
void SetIntpType(BPF& testBPF,const EInterpolation& eInterpolation)
	{testBPF.SetIntpType(eInterpolation);}
void SetSize (BPF& testBPF,TSize newSize){testBPF.SetSize(newSize);}
void SetStep(BPF& testBPF,TSize step){testBPF.SetStep(step);}
void SetValue(BPF& testBPF,TIndex index,float y)
	{testBPF.SetValue(index,y);}
void SetXValue(BPF& testBPF,TIndex index,float x)
	{testBPF.SetXValue(index,x);}
TSize Size(BPF& testBPF) {return testBPF.Size();}
void UpdateSplineTable(BPF& testBPF){testBPF.UpdateSplineTable();}




