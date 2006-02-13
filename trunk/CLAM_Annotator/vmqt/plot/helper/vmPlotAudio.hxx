#ifndef __VMQT_PLOT_AUDIO_H__
#define __VMQT_PLOT_AUDIO_H__

#include <string>
#include "vmQAppWrapper.hxx"
#include "vmPlayableAudioPlot.hxx"

namespace CLAM
{
	namespace VM
	{
		int PlotAudio(const Audio& audio, 
					  Segmentation* s=0,
					  int x=100, 
					  int y=100, 
					  int w=600, 
					  int h=250, 
					  const std::string& title="Audio Plot")
		{
			QAppWrapper::Init();
			PlayableAudioPlot plot;
			plot.SetTitle(title.c_str());
			plot.SetGeometry(x,y,w,h);
			plot.SetData(audio);
			plot.SetSegmentation(s);
			plot.show();
			return QAppWrapper::Run();
		}
	}
}

#endif
