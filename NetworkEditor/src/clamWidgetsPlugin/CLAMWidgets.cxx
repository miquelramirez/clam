#include <QtDesigner/QDesignerContainerExtension>
#include <QtDesigner/QDesignerCustomWidgetInterface>
#include <QtCore/QtPlugin>
#include "OscilloscopePlugin.hxx"
#include "VumeterPlugin.hxx"
#include "SpectrumViewPlugin.hxx"
#include "PeakViewPlugin.hxx"
#include "TonnetzPlugin.hxx"
#include "KeySpacePlugin.hxx"
#include "PolarChromaPeaksPlugin.hxx"
#include "ChordRankingPlugin.hxx"
#include "LPModelViewPlugin.hxx"
#include <iostream>
#include <CLAM/CLAMVersion.hxx>

#include "CLAMWidgets.hxx"

CLAMWidgets::CLAMWidgets(QObject* parent)
	: QObject(parent)
{
	std::cout << "Loading CLAM widgets " << CLAM::GetFullVersion() << "..." << std::endl;
	_widgetPlugins
		<< new OscilloscopePlugin(this)
		<< new VumeterPlugin(this)
		<< new SpectrumViewPlugin(this)
		<< new PeakViewPlugin(this)
		<< new TonnetzPlugin(this)
		<< new KeySpacePlugin(this)
		<< new PolarChromaPeaksPlugin(this)
		<< new ChordRankingPlugin(this)
		<< new LPModelViewPlugin(this)
		;
	std::cout << "All CLAM widgets plugins created" << std::endl;
}

CLAMWidgets::~CLAMWidgets()
{
	QList<QDesignerCustomWidgetInterface*>::iterator it;
	for(it = _widgetPlugins.begin(); it != _widgetPlugins.end(); it++)
		delete (*it);
	_widgetPlugins.clear();
}

QList<QDesignerCustomWidgetInterface*> CLAMWidgets::customWidgets() const
{
	return _widgetPlugins;
}

Q_EXPORT_PLUGIN2(clamwidets, CLAMWidgets)



