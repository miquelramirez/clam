#include <qwidgetplugin.h>

#include "NetAudioPlot.hxx"
#include "NetSpectrumPlot.hxx"
#include "NetPeaksPlot.hxx"
#include "NetFundPlot.hxx"
#include "NetAudioBuffPlot.hxx"
#include "NetSpecgramPlot.hxx"
#include "NetFundTrackPlot.hxx"
#include "NetSinTracksPlot.hxx"
#include <iostream>

class CLAMWidgetsPlugin : public QWidgetPlugin
{
public:
	CLAMWidgetsPlugin()
	{
		std::cout << "Loading CLAM qt widgets plugins" << std::endl;
	}
	QStringList keys() const
	{
		QStringList list;
	        list << "CLAM::VM::NetAudioPlot";
	        list << "CLAM::VM::NetSpectrumPlot";
	        list << "CLAM::VM::NetPeaksPlot";
	        list << "CLAM::VM::NetFundPlot";

	        list << "CLAM::VM::NetAudioBuffPlot";
	        list << "CLAM::VM::NetSpecgramPlot";
	        list << "CLAM::VM::NetFundTrackPlot";
	        list << "CLAM::VM::NetSinTracksPlot";
//	        list << "CLAM::VM::NetXPlot";
	        return list;
	}
	QWidget* create( const QString &key, QWidget* parent = 0, const char* name = 0 )
	{
		if (key=="CLAM::VM::NetAudioPlot") return new CLAM::VM::NetAudioPlot(parent);
		if (key=="CLAM::VM::NetSpectrumPlot") return new CLAM::VM::NetSpectrumPlot(parent);
		if (key=="CLAM::VM::NetPeaksPlot") return new CLAM::VM::NetPeaksPlot(parent);
		if (key=="CLAM::VM::NetFundPlot") return new CLAM::VM::NetFundPlot(parent);
		if (key=="CLAM::VM::NetAudioBuffPlot") return new CLAM::VM::NetAudioBuffPlot(parent);
		if (key=="CLAM::VM::NetSpecgramPlot") return new CLAM::VM::NetSpecgramPlot(parent);
		if (key=="CLAM::VM::NetFundTrackPlot") return new CLAM::VM::NetFundTrackPlot(parent);
		if (key=="CLAM::VM::NetSinTracksPlot") return new CLAM::VM::NetSinTracksPlot(parent);
//		if (key=="CLAM::VM::NetXPlot") return new CLAM::VM::NetXPlot(parent);
		return 0;
	}
	QString includeFile( const QString& key) const
	{
		if (key=="CLAM::VM::NetAudioPlot") return "NetAudioPlot.hxx";
		if (key=="CLAM::VM::NetSpectrumPlot") return "NetSpectrumPlot.hxx";
		if (key=="CLAM::VM::NetPeaksPlot") return "NetPeaksPlot.hxx";
		if (key=="CLAM::VM::NetFundPlot") return "NetFundPlot.hxx";
		if (key=="CLAM::VM::NetAudioBuffPlot") return "NetAudioBuffPlot.hxx";
		if (key=="CLAM::VM::NetSpecgramPlot") return "NetSpecgramPlot.hxx";
		if (key=="CLAM::VM::NetFundTrackPlot") return "NetFundTrackPlot.hxx";
		if (key=="CLAM::VM::NetSinTracksPlot") return "NetSinTracksPlot.hxx";
//		if (key=="CLAM::VM::NetXPlot") return "NetXPlot.hxx";
		return QString::null;
	}
	QIconSet iconSet( const QString& key) const
	{
		return QIconSet( QPixmap( ) );
	}
	QString group( const QString& key) const
	{
		if (key=="CLAM::VM::NetAudioPlot") return "CLAM Plots";
		if (key=="CLAM::VM::NetSpectrumPlot") return "CLAM Plots";
		if (key=="CLAM::VM::NetPeaksPlot") return "CLAM Plots";
		if (key=="CLAM::VM::NetFundPlot") return "CLAM Plots";
		if (key=="CLAM::VM::NetAudioBuffPlot") return "CLAM Plots";
		if (key=="CLAM::VM::NetSpecgramPlot") return "CLAM Plots";
		if (key=="CLAM::VM::NetFundTrackPlot") return "CLAM Plots";
		if (key=="CLAM::VM::NetSinTracksPlot") return "CLAM Plots";
//		if (key=="CLAM::VM::NetXPlot") return "CLAM Plots";
		return QString::null;
	}
	QString toolTip( const QString& key) const
	{
		if (key=="CLAM::VM::NetAudioPlot") return "CLAM Network Audio Port Monitor";
		if (key=="CLAM::VM::NetSpectrumPlot") return "CLAM Network Spectrum Port Monitor";
		if (key=="CLAM::VM::NetPeaksPlot") return "CLAM Network Spectral Peaks Port Monitor";
		if (key=="CLAM::VM::NetFundPlot") return "CLAM Network Fundamental Port Monitor";

		if (key=="CLAM::VM::NetAudioBuffPlot") return "CLAM Network Audio Port Log Monitor";
		if (key=="CLAM::VM::NetSpecgramPlot") return "CLAM Network Spectrum Port Spectrogram Monitor";
		if (key=="CLAM::VM::NetFundTrackPlot") return "CLAM Network Fundamental Tracker";
		if (key=="CLAM::VM::NetSinTracksPlot") return "CLAM Network Sinusoidal Peaks Tracker";
//		if (key=="CLAM::VM::NetXPlot") return "CLAM Network X Port Monitor";
		return QString::null;
	}
	QString whatsThis( const QString& key) const
	{
		if (key=="CLAM::VM::NetAudioPlot") return "Use this widget to visualize Audio content from an outport of a CLAM processing object";
		if (key=="CLAM::VM::NetSpectrumPlot") return "Use this widget to visualize Spectrum content from an outport of a CLAM processing object";
		if (key=="CLAM::VM::NetPeaksPlot") return "Use this widget to visualize Spectral Peaks content from an outport of a CLAM processing object";
		if (key=="CLAM::VM::NetFundPlot") return "Use this widget to visualize Fundamental content from an outport of a CLAM processing object";
		if (key=="CLAM::VM::NetAudioBuffPlot") return "Use this widget to track the Audio content from an outport of a CLAM processing object with a wider memory";
		if (key=="CLAM::VM::NetSpecgramPlot") return "Use this widget to track the Spectrums from an outport of a CLAM processing object as an spectrogram";
		if (key=="CLAM::VM::NetFundTrackPlot") return "Use this widget to track the Fundamental value from an outport of a CLAM processing object";
		if (key=="CLAM::VM::NetSinTracksPlot") return "Use this widget to visualize the sinusoidal tracks from an SpectralPeaks outport of a CLAM processing object";
//		if (key=="CLAM::VM::NetXPlot") return "Use this widget to visualize X content from an outport of a CLAM processing object";
		return QString::null;
	}
	bool isContainer( const QString& key) const
	{
		if (key=="CLAM::VM::NetAudioPlot") return false;
		if (key=="CLAM::VM::NetSpectrumPlot") return false;
		if (key=="CLAM::VM::NetPeaksPlot") return false;
		if (key=="CLAM::VM::NetFundPlot") return false;
		if (key=="CLAM::VM::NetAudioBuffPlot") return false;
		if (key=="CLAM::VM::NetSpecgramPlot") return false;
		if (key=="CLAM::VM::NetFundTrackPlot") return false;
		if (key=="CLAM::VM::NetXPlot") return false;
//		if (key=="CLAM::VM::NetXPlot") return false;
		return false;
	}
};

Q_EXPORT_PLUGIN( CLAMWidgetsPlugin )



