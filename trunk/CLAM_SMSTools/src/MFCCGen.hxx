#ifndef __CLAM_MFCCGEN_H__
#define __CLAM_MFCCGEN_H__

#include <Processing.hxx>
#include <Spectrum.hxx>


/**
 * The MFCCGen generates Mel Frequency Cepstral Coefficients using a
 * Spectrum in MagPhase representation.
 * <p>The MFCC Data Type is not defined yet (TODO!), so there is no 
 * output for the  Do function, but the ouput can be accessed thru
 * the GetMFCC(int num) member  function.
 * </p>
 */


namespace CLAM {

class MFCCGenConfig : public ProcessingConfig 
{
public:
	DYNAMIC_TYPE_USING_INTERFACE (MFCCGenConfig, 2,ProcessingConfig);
	DYN_ATTRIBUTE (0,public, std::string, Name);	
	DYN_ATTRIBUTE (1,public, TSize, NumFilter);	
	void DefaultInit();
};


class MFCCGen : public Processing {
public:
	MFCCGen();
	MFCCGen(MFCCGenConfig&);
	~MFCCGen();

	const char * GetClassName() const {return "MFCCGen";}

	/**
	 * GetFilterCoeff can be used to change
	 * the filters in a derived class.
	 */

	virtual TData GetFilterCoeff(int numFilt,int bin);
	TData GetDCTCoeff(int i,int j);
	void MakeDCTTable(int,int);

	void CalcFilterWeights();
	void MakeFilterTable();

	void FilterData(Spectrum&);
	void DCT();
	
	bool Do(void);
	
	bool Do(Spectrum& input);
	
	const ProcessingConfig &GetConfig() const { return mConfig;}

	bool SetPrototypes(Spectrum& input);
	
	bool SetPrototypes();
	
	bool UnsetPrototypes();
	
	bool MayDisableExecution() const {return true;}
	
	TData GetMFCC(int);

	Array<TData>& GetMFCC();

	Array<TData>& GetFilteredData();

private:
	bool ConcreteConfigure(const ProcessingConfig&  c);

		
	MFCCGenConfig    mConfig;
	DataArray        mFiltered;
	DataArray		 mFilteredLog;
	DataArray        mMFCC;
	TSize            mNumFilter;
	TSize            mNumCoeff;
	TData*           mDCTTable;
	TData*           mFiltTable;
	TData            mSrate;     // We can't rely on this before we get it
                                  // from the input spectrum
	TSize            mFFTSize;
	TData*            mFilterWeights;

};

}

#endif
