#ifndef __VMQT_PLOT_AUDIO_STEREO_H__
#define __VMQT_PLOT_AUDIO_STEREO_H__

#include <string>
#include "vmQAppWrapper.hxx"
#include "vmPlayableAudioStereoPlot.hxx"

namespace CLAM
{
	namespace VM
	{
		int PlotAudioStereo(const Audio& chn0,
							const Audio& chn1,
							int x=100,
							int y=100,
							int w=600,
							int h=350,
							const std::string& title="Audio Stereo Plot")
		{
			QAppWrapper::Init();
			PlayableAudioStereoPlot plot;
			plot.SetTitle(title.c_str());
			plot.SetGeometry(x,y,w,h);
			plot.SetData(chn0,chn1);
			plot.show();
			return QAppWrapper::Run();
		}
	}
}

#endif

