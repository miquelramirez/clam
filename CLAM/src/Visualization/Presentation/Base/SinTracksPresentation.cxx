#include "SinTracksPresentation.hxx"
#include "SinTracksModel.hxx"

namespace CLAMVM
{
		SinTracksPresentation::SinTracksPresentation()
		{
				SetTrackList.Wrap( this, &SinTracksPresentation::OnNewTrackList );
				SetSpectralRange.Wrap( this, &SinTracksPresentation::OnNewRange );
				SetTimeInterval.Wrap( this, &SinTracksPresentation::OnNewDuration );
		}

		SinTracksPresentation::~SinTracksPresentation()
		{
		}

		void SinTracksPresentation::AttachTo( SinTracksModel& stmodel )
		{
				stmodel.TrackListPublished.Connect( SetTrackList );
				stmodel.SpectralRangePublished.Connect( SetSpectralRange );
				stmodel.DurationPublished.Connect( SetTimeInterval );
		}

		void SinTracksPresentation::Detach()
		{
				SetTrackList.Unbind();
				SetSpectralRange.Unbind();
				SetTimeInterval.Unbind();
		}
}
