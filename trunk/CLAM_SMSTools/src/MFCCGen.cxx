	
#include "CLAM_Math.hxx"
#include <iostream>
#include <Processing.hxx>
#include "MFCCGen.hxx"


using namespace std;


namespace CLAM {

#define SILENT

void MFCCGenConfig::DefaultInit()
{
	AddName();
	AddNumFilter();
	UpdateData();
	SetNumFilter(40);

}


MFCCGen::MFCCGen()
{
	mFilterWeights = NULL;
	mNumFilter = 0;
	mDCTTable = NULL;
	mFiltTable = NULL;
	Configure(MFCCGenConfig());
}

MFCCGen::MFCCGen(MFCCGenConfig& c)
{
	mFilterWeights = NULL;
	mNumFilter = 0;
	mDCTTable = NULL;
	mFiltTable = NULL;
	Configure(c);
}

MFCCGen::~MFCCGen() 
{
	delete[] mDCTTable;
	delete[] mFiltTable;
}

bool MFCCGen::SetPrototypes(Spectrum& input) 
{
	cout << "MFCCGen::SetPrototypes not implemented" << endl;
	return false;
}

bool MFCCGen::SetPrototypes() 
{
	cout << "MFCCGen::SetPrototypes not implemented" << endl;
	return false;
}

bool MFCCGen::UnsetPrototypes()
{
	cout << "MFCCGen::UnsetPrototypes not implemented" << endl;
	return false;
}

bool MFCCGen::ConcreteConfigure(const ProcessingConfig& c)
{
	CopyAsConcreteConfig(mConfig, c);
	if (mNumFilter != mConfig.GetNumFilter()) {
		mNumFilter = mConfig.GetNumFilter();
		mFiltered.Resize(mNumFilter);
		mFiltered.SetSize(mNumFilter);



		mFilteredLog.Resize(mNumFilter);

		mFilteredLog.SetSize(mNumFilter);


		mMFCC.Resize(mNumFilter);
		mMFCC.SetSize(mNumFilter);

		MakeDCTTable(mNumFilter,mNumFilter);
		MakeFilterTable();
	}
	mSrate = 44100;
	mFFTSize = 512;
	return true;
}



void MFCCGen::MakeDCTTable(int sizex,int sizey)
{
	int i,j;

	if (mDCTTable) delete[] mDCTTable;
	mDCTTable = new TData[sizex*sizey];


	float scale_fac = TData(sqrt(2./mNumFilter));
	for (j = 0; j<mNumFilter; j++)
		for (i=0; i< mNumFilter; i++) {
			mDCTTable[i*sizex+j] = TData(scale_fac * cos(j*(2*i +1)*M_PI/2/mNumFilter));
			if (j == 0)
				mDCTTable[i*sizex+j] *= sqrt(TData(2.0))/TData(2.0);
		}  

#ifndef SILENT
	cout << "cosine Table" << endl;
	for (i=0;i<mNumFilter;i++)
		cout << i << " " << GetDCTCoeff(i,1) << endl;
#endif

}

void MFCCGen::MakeFilterTable()
{
	if (mFiltTable) delete[] mFiltTable;

	mFiltTable = new TData[mNumFilter+2];

	if (mFilterWeights) delete[] mFilterWeights;
	mFilterWeights = new TData[mNumFilter+2];

	TData lowestFrequency = TData(133.3333);
	TData linearFilters = TData(13);
	TData linearSpacing = TData(66.66666);
	TData logSpacing = TData(1.0711703);

	int i=0;

	while (i <  linearFilters) {
		mFiltTable[i] = lowestFrequency + i*linearSpacing;		
		i++;
	}
	while (i < mNumFilter+2) {
		mFiltTable[i] = mFiltTable[i-1]*logSpacing;
		i++;
	}

#ifndef SILENT
	for (i=0;i<mNumFilter+2;i++)
		cout << mFiltTable[i] << endl;
	cout << endl;
#endif
}


void  MFCCGen::CalcFilterWeights() {
	int i,j;

	/* The Slaney implementation uses weights calculated
	   as (2.0/(upperfreq - lowerfreq), but this doesn't 
	   give smooth values (Because the filter frequencies
	   are not on bin boundaries). 
	   With this weights calculation we have a smooth filter
	   response (e.g. a fft of 1.0 values for each bin gives
	   us 1.0 for each filter).
	*/	   

#if 0 /* Slaney calculation */
	for (j=0;j<mNumFilter;j++) 
		mFilterWeights[j] = 2./(mFiltTable[j+2] - mFiltTable[j]); 
#endif

#if 1
	for (j=0;j<mNumFilter;j++) {
		mFilterWeights[j] = 0.0; 
		for (i=0;i<mFFTSize/2+1;i++)
			mFilterWeights[j] += GetFilterCoeff(j,i);
		mFilterWeights[j] = TData(2.0)/mFilterWeights[j]; 
	}
#endif
}



TData MFCCGen::GetFilterCoeff(int numFilt,int bin)
{
	TData binfreq = bin*mSrate/mFFTSize;
	TData fstep = mFiltTable[numFilt+1]- mFiltTable[numFilt];

	TData out; 
	if (binfreq < mFiltTable[numFilt+1] && binfreq >= mFiltTable[numFilt])
		out = (binfreq -mFiltTable[numFilt])/fstep;
	else if (binfreq > mFiltTable[numFilt+1] && binfreq<mFiltTable[numFilt+2])
		out = (mFiltTable[numFilt+2]-binfreq)/fstep;
	else 
		out = 0.0;
	return out;
}


TData MFCCGen::GetDCTCoeff(int i,int j)
{
	return  mDCTTable[i*mNumFilter + j];
}


TData MFCCGen::GetMFCC(int num) 
{
	CLAM_DEBUG_ASSERT(num >= 0 && num < mNumFilter,"Invalid index");
	return mMFCC[num];
}



Array<TData>& MFCCGen::GetMFCC()

{

	return mMFCC;

}



Array<TData>& MFCCGen::GetFilteredData()

{

	return mFiltered;

}


void MFCCGen::FilterData(Spectrum& input) 
{
	TData* inputp = input.GetMagBuffer().GetPtr();

	TData* filtered = mFiltered.GetPtr();

	TData* filteredLog = mFilteredLog.GetPtr();

	int filtsize = mNumFilter;

	int i,j;

	// Calculate the Filter coefficients
	
	for (i=0;i<filtsize;i++) {
		filtered[i] = 0.0;

		filteredLog[i] = 0.0;
		for (j=0;j<mFFTSize/2+1;j++) {
			filtered[i] += inputp[j]*GetFilterCoeff(i,j);
		} 
	}

	for (i=0;i<filtsize;i++) 
		filtered[i]*=mFilterWeights[i];


	for (i=0;i<filtsize;i++) 
		if (filtered[i] != 0) 
			filteredLog[i]= log10(filtered[i]);
	

#ifndef SILENT

	cout << "Filtered:" << endl; 
	for (i=0;i<mNumFilter;i++)
		cout << i << " " << filtered[i] << endl;
	cout << endl;		
#endif


}

void MFCCGen::DCT() 
{
	int i,j;
	mNumCoeff = mNumFilter; // for now we calculate all the DCT coeffs

	for (j=0;j<mNumCoeff;j++) {
		mMFCC[j] = 0.0;
		for (i=0;i<mNumFilter;i++)			
			mMFCC[j] += mFilteredLog[i]*GetDCTCoeff(i,j);
	}
}
	
bool MFCCGen::Do(void) 
{
	return false;
}
	
bool MFCCGen::Do(Spectrum& input) {
	mSrate = input.GetSpectralRange()*TData(2.0);
	mFFTSize = (TSize) ((input.GetSize() - 1.0)*2.0);

	CalcFilterWeights();
	FilterData(input);
	DCT();


#ifndef SILENT

	int i;

	cout << "Coefficients:" << endl; 

	for (i=0;i<mNumCoeff;i++)

		cout << i << " " << mMFCC[i] << endl;

	cout << endl;		
#endif
	return true;
}



}

