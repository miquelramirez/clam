#ifndef _SegmentatorConfig_
#define _SegmentatorConfig_

#include "SearchArray.hxx"
#include "ProcessingConfig.hxx"
#include <iosfwd>

namespace CLAM {

typedef struct //may need further additions
{
	TIndex id;
	TData threshold;
	TData percentil;
	TData nPreviousFrames;
} TDescriptorsParams;

inline bool operator<(const TDescriptorsParams& param1,const TDescriptorsParams& param2)
{
	if (param1.id<param2.id) return true;
	else return false;
}

inline bool operator>=(const TDescriptorsParams& param1,const TDescriptorsParams& param2)
{
	if (param1.id>=param2.id) return true;
	else return false;
}

inline bool operator==(const TDescriptorsParams& param1,const TDescriptorsParams& param2)
{
	if (param1.id==param2.id) return true;
	else return false;
}

inline bool operator==(const TDescriptorsParams& param1,TIndex id)
{
	if (param1.id==id) return true;
	else return false;
}

std::ostream& operator << (std::ostream& myStream, const TDescriptorsParams& a);

std::istream& operator >> (std::istream& myStream, const TDescriptorsParams& a);

	/**
	 * Config class for the Segmentator Processing
	 */
class SegmentatorConfig : public ProcessingConfig
{
friend class Segmentator;
public:
	DYNAMIC_TYPE_USING_INTERFACE(SegmentatorConfig,2,ProcessingConfig);
	DYN_ATTRIBUTE (0, public, int, MinSegmentLength);
	DYN_ATTRIBUTE (1, public, Array<TDescriptorsParams>, DescriptorsParams);
protected:
	void DefaultInit();
/*Public Interface*/
public:
	void AddDescParams(const TDescriptorsParams& descParams);
	bool FindDescParams(TDescriptorsParams& descParams);
	void ClearDescParams();

	void SetDescriptorsSearch (const SearchArray<TDescriptorsParams> & frame) {
		mDescriptorsSearch=frame;
	}
	const SearchArray<TDescriptorsParams> & GetDescriptorsSearch () const {
		return mDescriptorsSearch;
	}
	SearchArray<TDescriptorsParams> & GetDescriptorsSearch () {
		return mDescriptorsSearch;
	}
	~SegmentatorConfig(){};
private:
	SearchArray<TDescriptorsParams> mDescriptorsSearch;
};

} // namespace CLAM

#endif
