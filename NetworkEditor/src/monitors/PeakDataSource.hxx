#ifndef PeakDataSource_hxx
#define PeakDataSource_hxx

#include <CLAM/DataTypes.hxx>

class PeakDataSource
{
public:
	virtual ~PeakDataSource() {}
	virtual const CLAM::TData * positionData()=0;
	virtual const CLAM::TData * magnitudeData()=0;
	virtual unsigned nBins() const=0;
	virtual bool isEnabled() const=0;
};

#endif//PeakDataSource_hxx
