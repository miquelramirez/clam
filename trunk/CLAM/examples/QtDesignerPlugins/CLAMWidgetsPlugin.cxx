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
	        list << "CLAM::VM::NetAudioPlot";
	        list << "CLAM::VM::NetSpectrumPlot";
	        list << "CLAM::VM::NetPeaksPlot";
	        list << "CLAM::VM::NetFundPlot";
	        return list;
	}
	QWidget* create( const QString &key, QWidget* parent = 0, const char* name = 0 )
	{
		if (key=="CLAM::VM::NetAudioPlot") return new CLAM::VM::NetAudioPlot(parent);
		if (key=="CLAM::VM::NetSpectrumPlot") return new CLAM::VM::NetSpectrumPlot(parent);
		if (key=="CLAM::VM::NetPeaksPlot") return new CLAM::VM::NetPeaksPlot(parent);
		if (key=="CLAM::VM::NetFundPlot") return new CLAM::VM::NetFundPlot(parent);
		return 0;
	}
	QString includeFile( const QString& key) const
	{
		if (key=="CLAM::VM::NetAudioPlot") return "NetAudioPlot.hxx";
		if (key=="CLAM::VM::NetSpectrumPlot") return "NetSpectrumPlot.hxx";
		if (key=="CLAM::VM::NetPeaksPlot") return "NetPeaksPlot.hxx";
		if (key=="CLAM::VM::NetFundPlot") return "NetFundPlot.hxx";
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
		return QString::null;
	}
	QString toolTip( const QString& key) const
	{
		if (key=="CLAM::VM::NetAudioPlot") return "CLAM Network Audio Port Monitor";
		if (key=="CLAM::VM::NetSpectrumPlot") return "CLAM Network Spectrum Port Monitor";
		if (key=="CLAM::VM::NetPeaksPlot") return "CLAM Network Spectral Peaks Port Monitor";
		if (key=="CLAM::VM::NetFundPlot") return "CLAM Network Fundamental Port Monitor";
		return QString::null;
	}
	QString whatsThis( const QString& key) const
	{
		if (key=="CLAM::VM::NetAudioPlot") return "Use this widget to visualize Audio content from an outport of a CLAM processing object";
		if (key=="CLAM::VM::NetSpectrumPlot") return "Use this widget to visualize Spectrum content from an outport of a CLAM processing object";
		if (key=="CLAM::VM::NetPeaksPlot") return "Use this widget to visualize Spectral Peaks content from an outport of a CLAM processing object";
		if (key=="CLAM::VM::NetFundPlot") return "Use this widget to visualize Fundamental content from an outport of a CLAM processing object";
		return QString::null;
	}
	bool isContainer( const QString& key) const
	{
		if (key=="CLAM::VM::NetAudioPlot") return false;
		if (key=="CLAM::VM::NetSpectrumPlot") return false;
		if (key=="CLAM::VM::NetPeaksPlot") return false;
		if (key=="CLAM::VM::NetFundPlot") return false;
		return false;
	}
};

Q_EXPORT_PLUGIN( CLAMWidgetsPlugin )



