#include "SDIFFrame.hxx"
#include "SDIFMatrix.hxx"

using namespace SDIF;

Frame::Frame(const TypeId& type, CLAM::TFloat64 time,CLAM::TInt32 id)
:mHeader(type,time,id)
{
}

Frame::~Frame()
{
	typedef std::list<Matrix*>::iterator iterator;
	
	iterator it = mMatrixList.begin();
	iterator end = mMatrixList.end();
	
	while (it!=end)
	{
		delete *it;
		it++;
	}
}

void Frame::Add(Matrix* pMatrix)
{
	mMatrixList.push_back(pMatrix);
	mHeader.mnMatrices++;
	mHeader.mSize += pMatrix->SizeInFile();
};

