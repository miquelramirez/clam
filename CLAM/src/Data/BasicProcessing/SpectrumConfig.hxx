#ifndef __SpectrumConfig__
#define __SpectrumConfig__

#include "DataTypes.hxx"
#include "SpecTypeFlags.hxx"
#include "GlobalEnums.hxx"
#include "ProcessingDataConfig.hxx"

namespace CLAM {

/**
* The Spectrum configuration object.
*/
class SpectrumConfig : public ProcessingDataConfig
{
public:
	DYNAMIC_TYPE_USING_INTERFACE (SpectrumConfig, 5, ProcessingDataConfig);
	/**
	* The kind of scale
	*/
	DYN_ATTRIBUTE (0, public, EScale, Scale);
	DYN_ATTRIBUTE (1, public, TData, SpectralRange);
	DYN_ATTRIBUTE (2, public, int, Size);
	DYN_ATTRIBUTE (3, public, SpecTypeFlags, Type);
	DYN_ATTRIBUTE (4, public, int, BPFSize);
public:
	/*
	* Sets to the default configuration.
	*/
	void DefaultValues();
	~SpectrumConfig(){};
protected:
	/*
	* Dynamic Type constructor: instanciates all the attributes.
	*/
	void DefaultInit();
};

} // namespace CLAM

#endif
