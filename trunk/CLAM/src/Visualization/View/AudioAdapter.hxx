#ifndef __AUDIOADAPTER__
#define __AUDIOADAPTER__

#include "ProcessingDataAdapter.hxx"
#include "AudioModel.hxx"

namespace CLAM
{
	class Audio;
	class ProcessingData;
}

namespace CLAMVM
{
	using CLAM::Audio;
	using CLAM::ProcessingData;

	class AudioAdapter 
		: public ProcessingDataAdapter, public AudioModel
	{
		// attributes
	private:
		const Audio*         mObserved;

	protected:
		// methods
	public:
		AudioAdapter();

		virtual ~AudioAdapter();

		virtual const char* GetClassName() const
		{
			return "AudioAdapter";
		}

		virtual bool Publish();

		virtual bool BindTo( const ProcessingData& audioObj );

	};
}


#endif // AudioAdapter.hxx
