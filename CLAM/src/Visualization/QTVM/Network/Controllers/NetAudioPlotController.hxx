#ifndef __NETAUDIOPLOTCONTROLLER__
#define __NETAUDIOPLOTCONTROLLER__

#include "Audio.hxx"
#include "DataRenderer.hxx"
#include "NetPlotController.hxx"

namespace CLAM
{
	namespace VM
	{
		class NetAudioPlotController : public NetPlotController
		{
			public:
				NetAudioPlotController();
				virtual ~NetAudioPlotController();

				void SetData(const Audio& audio);
				void SetDataColor(Color c);
				void Draw();

			protected:
				void FullView();
				bool MustProcessData() const;
				
			private:
				DataRenderer _dRenderer;
				Audio _audio;
				DataArray _processedData;
				bool _mustProcessData;
				
				void DrawAxis();

				void ProcessData();

		};
	}
}

#endif

