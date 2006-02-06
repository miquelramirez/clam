#ifndef __VMQT_PLOT_AUDIO_STEREO_H__
#define __VMQT_PLOT_AUDIO_STEREO_H__

#include <string>
#include "vm_qapp_wrapper.hxx"
#include "vm_playable_audio_stereo_plot.hxx"

namespace CLAM
{
	namespace VM
	{
		int plot_audio_stereo(const Audio& chn0,
							  const Audio& chn1,
							  int x=100,
							  int y=100,
							  int w=600,
							  int h=350,
							  const std::string& title="Audio Stereo Plot")
		{
			QAppWrapper::init();
			PlayableAudioStereoPlot plot;
			plot.set_title(title.c_str());
			plot.set_geometry(x,y,w,h);
			plot.set_data(chn0,chn1);
			plot.show();
			return QAppWrapper::run();
		}
	}
}

#endif

