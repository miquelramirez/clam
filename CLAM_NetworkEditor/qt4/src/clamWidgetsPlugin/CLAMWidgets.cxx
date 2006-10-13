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
#include "PlotPlugin.hxx"
#include <iostream>

#include "CLAMWidgets.hxx"

CLAMWidgets::CLAMWidgets(QObject* parent)
	: QObject(parent)
{
	std::cout << "Loading CLAM widgets " << VERSION << "..." << std::endl;
	_widgetPlugins
		<< new OscilloscopePlugin(this)
		<< new VumeterPlugin(this)
		<< new SpectrumViewPlugin(this)
		<< new PeakViewPlugin(this)
		<< new TonnetzPlugin(this)
		<< new KeySpacePlugin(this)
		<< new PolarChromaPeaksPlugin(this)
		<< new PlotPlugin(this)
		;
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



