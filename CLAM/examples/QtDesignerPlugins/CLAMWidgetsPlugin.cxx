#include "qwidgetplugin.h"

#include "NetAudioPlot.hxx"
#include "NetSpectrumPlot.hxx"
#include "NetPeaksPlot.hxx"
#include "NetFundPlot.hxx"

class CLAMWidgetsPlugin : public QWidgetPlugin
{
public:
	CLAMWidgetsPlugin() {}
	QStringList keys() const
	{
		QStringList list;
	        list << "NetAudioPlot";
	        list << "NetSpectrumPlot";
	        list << "NetPeaksPlot";
	        list << "NetFundPlot";
	        return list;
	}
	QWidget* create( const QString &key, QWidget* parent = 0, const char* name = 0 )
	{
		if (key=="NetAudioPlot") return new CLAM::VM::NetAudioPlot(parent);
		if (key=="NetSpectrumPlot") return new CLAM::VM::NetSpectrumPlot(parent);
		if (key=="NetPeaksPlot") return new CLAM::VM::NetPeaksPlot(parent);
		if (key=="NetFundPlot") return new CLAM::VM::NetFundPlot(parent);
		return 0;
	}
	QString includeFile( const QString& key) const
	{
		if (key=="NetAudioPlot") return "NetAudioPlot.hxx";
		if (key=="NetSpectrumPlot") return "NetSpectrumPlot.hxx";
		if (key=="NetPeaksPlot") return "NetPeaksPlot.hxx";
		if (key=="NetFundPlot") return "NetFundPlot.hxx";
		return QString::null;
	}
	QIconSet iconSet( const QString& key) const
	{
		return QIconSet( QPixmap( ) );
	}
	QString group( const QString& key) const
	{
		if (key=="NetAudioPlot") return "Plots";
		if (key=="NetSpectrumPlot") return "Plots";
		if (key=="NetPeaksPlot") return "Plots";
		if (key=="NetFundPlot") return "Plots";
		return QString::null;
	}
	QString toolTip( const QString& key) const
	{
		if (key=="NetAudioPlot") return "Mixer Slider";
		if (key=="NetSpectrumPlot") return "Mixer Slider";
		if (key=="NetPeaksPlot") return "Mixer Slider";
		if (key=="NetFundPlot") return "Mixer Slider";
		return QString::null;
	}
	QString whatsThis( const QString& key) const
	{
		if (key=="NetAudioPlot") return "A Mixer device like slider";
		if (key=="NetSpectrumPlot") return "A Mixer device like slider";
		if (key=="NetPeaksPlot") return "A Mixer device like slider";
		if (key=="NetFundPlot") return "A Mixer device like slider";
		return QString::null;
	}
	bool isContainer( const QString& key) const
	{
		if (key=="NetAudioPlot") return true;
		if (key=="NetSpectrumPlot") return true;
		if (key=="NetPeaksPlot") return true;
		if (key=="NetFundPlot") return true;
		return false;
	}
};

Q_EXPORT_PLUGIN( CLAMWidgetsPlugin )



