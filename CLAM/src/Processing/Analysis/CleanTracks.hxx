#ifndef _CLEANTRACKS__
#define _CLEANTRACKS__

#include "Processing.hxx"
#include "DynamicType.hxx"
#include "SpectralPeakArray.hxx"
#include "SpectralPeak.hxx"
#include "Array.hxx"
#include "SearchArray.hxx"
#include "Segment.hxx"



namespace CLAM {

	
	
	
	class CleanTracksConfig: public ProcessingConfig
	{
	public:
	  DYNAMIC_TYPE_USING_INTERFACE(CleanTracksConfig, 4,ProcessingConfig);
	  DYN_ATTRIBUTE (0, public, std::string, Name);
	  DYN_ATTRIBUTE (1,public,TSize,MaxDropOut);
	  DYN_ATTRIBUTE (2,public,TSize,MinLength);
	  DYN_ATTRIBUTE (3,public,TData,FreqDev);

	  void DefaultInit();
	  ~CleanTracksConfig(){};
	};

	typedef struct STrajectory
	{
		TIndex id;
		int beginPos;
		int length;
		TData initialFreq;
		TData finalFreq;
		TData initialMag;
		TData finalMag;
		//TData initialPhase;
		//TData finalPhase;
		int continuedAtId;
		friend bool operator >=(const struct STrajectory& first,const struct STrajectory& second)
		{
			return(first.id>=second.id);
		}
		friend bool operator <(const struct STrajectory& first,const struct STrajectory& second)
		{
			return(first.id<second.id);
		}
	}TTrajectory;

	

	class CleanTracks: public Processing
	{
		mutable CleanTracksConfig mConfig;

		virtual const char *GetClassName() {return "CleanTracks";} 

		/** Config change method
		 * @throw
		 * bad_cast exception when the argument is not an SpecAdderConfig
		 * object.
		 */
		virtual bool ConcreteConfigure(const ProcessingConfig&) throw(std::bad_cast);

	public:

		CleanTracks();
		CleanTracks(const CleanTracksConfig &c);
		~CleanTracks();

		//Configuration accessor
		const ProcessingConfig &GetConfig() const { return mConfig;};		

		//Peak Continuation for one frame
		bool Do(Array<SpectralPeakArray*>& peakArrayArray);
		bool Do(Segment& segment);
        bool Do(void);

		int GetnCleanedTracks() const {return mTrajectoryArray.Size();};
		
	private:

	


		void Clean (Array<SpectralPeakArray*>& peakArrayArray);
		void Continue(Array<SpectralPeakArray*>& peakArrayArray);
		void AddTrajectory(TTrajectory& trajectory);
		void DeleteTrajectory(int id);
		void UpdateTrackIds(Array<SpectralPeakArray*>& peakArrayArray);
		void Update(Array<SpectralPeakArray*>& peakArrayArray);
		void ContinuedAt(void);
		void interpolatePeaks(TTrajectory& trajectory, Array<SpectralPeakArray*>& peakArrayArray);

		TSize mMaxDropOut;
		TSize mMinLength;
		TData mFreqDev;
		Array<TTrajectory> mTrajectoryArray;
		SearchArray<TTrajectory> mSearchTrajectories;


			

	};

};//namespace

#endif