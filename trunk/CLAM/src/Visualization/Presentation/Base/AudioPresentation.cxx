#include "AudioPresentation.hxx"
#include "AudioModel.hxx"

namespace CLAMVM
{
		AudioPresentation::AudioPresentation()
		{
				SetAudio.Wrap( this, &AudioPresentation::OnNewAudio );
		}

		AudioPresentation::~AudioPresentation()
		{
		}

		void AudioPresentation::AttachTo( AudioModel& model )
		{
				model.ObjectPublished.Connect( SetAudio );
		}

		void AudioPresentation::Detach()
		{
				SetAudio.Unbind();
		}
		
}
