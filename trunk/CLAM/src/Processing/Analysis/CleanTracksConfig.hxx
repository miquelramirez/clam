#ifndef _CleanTracksConfig_
#define _CleanTracksConfig_

#include "DataTypes.hxx"
#include "ProcessingConfig.hxx"

namespace CLAM {

	/**
	 * Config class for the CleanTracks Processing
	 */
	class CleanTracksConfig: public ProcessingConfig
	{
	public:
		DYNAMIC_TYPE_USING_INTERFACE(CleanTracksConfig, 6,ProcessingConfig);
		DYN_ATTRIBUTE (0, public, std::string, Name);
		/** Maximum tolerance (in frames) to peak discontinuation */
		DYN_ATTRIBUTE (1,public,TSize,MaxDropOut);
		/** The minimum lenght a track should have to be keeped */
		DYN_ATTRIBUTE (2,public,TSize,MinLength);
		/** Maximum frequency distance between two tracks to be considered one
		as continuation of the other, expressed as a percentage between 0 and 100 */
		DYN_ATTRIBUTE (3,public,TData,FreqDev);
		/** This attribute is necessary so that BinPosition can be recomputed*/
		DYN_ATTRIBUTE (4, public, TData, SamplingRate);
		/** This attribute is necessary so that BinPosition can be recomputed*/
		DYN_ATTRIBUTE (5, public, int, SpecSize);

		void DefaultInit();
		~CleanTracksConfig(){};
	};

} // namespace CLAM

#endif
