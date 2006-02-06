#ifndef __VMQT_PLOT_AUDIO_H__
#define __VMQT_PLOT_AUDIO_H__

#include <string>
#include "vm_qapp_wrapper.hxx"
#include "vm_playable_audio_plot.hxx"

namespace CLAM
{
	namespace VM
	{
		int plot_audio(const Audio& audio, 
					   Segmentation* s=0,
					   int x=100, 
					   int y=100, 
					   int w=600, 
					   int h=250, 
					   const std::string& title="Audio Plot")
		{
			QAppWrapper::init();
			PlayableAudioPlot plot;
			plot.set_title(title.c_str());
			plot.set_geometry(x,y,w,h);
			plot.set_data(audio);
			plot.set_segmentation(s);
			plot.show();
			return QAppWrapper::run();
		}
	}
}

#endif
