#include <QtDesigner/QDesignerContainerExtension>
#include <QtDesigner/QDesignerCustomWidgetInterface>
#include <QtCore/QtPlugin>
#include "PlotPlugin.hxx"
#include "worldtimeclockplugin.h"
#include <iostream>

#include "CLAMWidgets.hxx"

CLAMWidgets::CLAMWidgets(QObject* parent)
	: QObject(parent)
{
	std::cout << "Loading CLAM widgets..." << std::endl;
	_widgetPlugins
		<< new PlotPlugin(this)
		<< new WorldTimeClockPlugin2(this)
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

Q_EXPORT_PLUGIN2(clamwidgets, CLAMWidgets)



