#ifndef __PLOTFACTORY__
#define __PLOTFACTORY__

#include "QtPlots.hxx"

namespace CLAM
{
	namespace VM
	{
		class PlotFactory
		{
			public:
				static QtAudioPlot* GetAudioPlot(	const Audio& audio,
													const std::string& label="", // title
													int x=100, // left corner x coord
													int y=100, // left corner y coord
													int w=500, // with
													int h=225, // height
													bool sc=true ); // switch display colors automatically 

				static QtStereoAudioPlot* GetStereoAudioPlot(	std::vector<Audio> data,
																const std::string& label="",
																int x=100,
																int y=100,
																int w=660,
																int h=250,
																bool sc=true);

				static QtFundFreqPlot* GetFundFreqPlot(const Segment& segment,
														const std::string& label="",
														int x=100,
														int y=100,
														int w=500,
														int h=225,
														bool sc=true);

				static QtSinTracksPlot* GetSinTracksPlot(	const Segment& segment,
															const std::string& label="",
															int x=100,
															int y=100,
															int w=500,
															int h=225,
															bool sc=true );

				static QtSinTracksPlot* GetSinTracksPlot(	const Array< SpectralPeakArray >& peakMtx,
															const TData& sr,
															const TData& dur,
															const std::string& label="",
															int x=100,
															int y=100,
															int w=500,
															int h=225,
															bool sc=true );

				static QtSpectrumPlot* GetSpectrumPlot(const Spectrum& spec,
														const std::string& label="",
														int x=100,
														int y=100,
														int w=500,
														int h=225,
														bool sc=true);

				static QtSpectrumAndPeaksPlot* GetSpectrumAndPeaksPlot(const Spectrum& spec,
																		const SpectralPeakArray& peaks,
																		const std::string& label="",
																		int x=100,
																		int y=100,
																		int w=500,
																		int h=225,
																		bool sc=true);

				static QtMultiPlot* GetMultiPlot(	const std::string& label="",
													int x=100,
													int y=100,
													int w=500,
													int h=225,
													bool sc=true );

				static QtAudioPlot* GetAudioPlot(	QWidget* parent, // widget parent
													const Audio& audio,
													const std::string& label="",
													int x=100,
													int y=100,
													int w=500,
													int h=225,
													bool sc=true);

				static QtStereoAudioPlot* GetStereoAudioPlot(	QWidget* parent,
																std::vector<Audio> data,
																const std::string& label="",
																int x=100,
																int y=100,
																int w=660,
																int h=250,
																bool sc=true);

				static QtFundFreqPlot* GetFundFreqPlot(QWidget* parent,
														const Segment& segment,
														const std::string& label="",
														int x=100,
														int y=100,
														int w=500,
														int h=225,
														bool sc=true);

				static QtSpectrumPlot* GetSpectrumPlot(QWidget* parent,
														const Spectrum& spec,
														const std::string& label="",
														int x=100,
														int y=100,
														int w=500,
														int h=225,
														bool sc=true);

				static QtSpectrumAndPeaksPlot* GetSpectrumAndPeaksPlot(QWidget* parent,
																		const Spectrum& spec,
																		const SpectralPeakArray& peaks,
																		const std::string& label="",
																		int x=100,
																		int y=100,
																		int w=500,
																		int h=225,
																		bool sc=true);

				static QtSinTracksPlot* GetSinTracksPlot(	QWidget* parent,
															const Segment& segment,
															const std::string& label="",
															int x=100,
															int y=100,
															int w=500,
															int h=225,
															bool sc=true);

				static QtSinTracksPlot* GetSinTracksPlot(	QWidget* parent,
															const Array< SpectralPeakArray >& peakMtx,
															const TData& sr,
															const TData& dur,
															const std::string& label="",
															int x=100,
															int y=100,
															int w=500,
															int h=225,
															bool sc=true);

				static QtMultiPlot* GetMultiPlot(	QWidget* parent,
													const std::string& label="",
													int x=100,
													int y=100,
													int w=500,
													int h=225,
													bool sc=true );

		private:
				static QtPlot* create(const Audio& audio,
									   const std::string& label,
									   int x, int y, int w, int h,
									   bool sc=true,
									   QWidget* parent=0);

				static QtPlot* create( std::vector<Audio> data,
										const std::string& label,
										int x, int y, int w, int h,
										bool sc=true,
										QWidget* parent=0);

				static QtPlot* create( const Segment& segment,
										const std::string& label,
										int x, int y, int w, int h,
										bool sc=true,
										bool st=false,
										QWidget* parent=0 );

				static QtPlot* create( const Spectrum& spec,
										const std::string& label,
										int x, int y, int w, int h,
										bool sc=true,
										QWidget* parent=0);

				static QtPlot* create( const Spectrum& spec,
										const SpectralPeakArray& peaks,
										const std::string& label,
										int x, int y, int w, int h,
										bool sc=true,
									    QWidget* parent=0 );

				static QtPlot* create( const Array< SpectralPeakArray >& peakMtx,
										const TData& sr,
										const TData& dur,
										const std::string& label,
										int x, int y, int w, int h,
										bool sc=true,
									    QWidget* parent=0 );

				static QtPlot* create( const std::string& label,
										int x, int y, int w, int h,
										bool sc=true,
										QWidget* parent=0 );

		};
	}
}

#endif
