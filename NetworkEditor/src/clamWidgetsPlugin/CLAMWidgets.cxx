#include <QtDesigner/QDesignerContainerExtension>
#include <QtDesigner/QDesignerCustomWidgetInterface>
#include <QtCore/QtPlugin>
#include "OscilloscopePlugin.hxx"
#include "VumeterPlugin.hxx"
#include "SpectrumViewPlugin.hxx"
#include "PeakViewPlugin.hxx"
#include "TonnetzPlugin.hxx"
#include "KeySpacePlugin.hxx"
#include "SpectrogramPlugin.hxx"
#include "PolarChromaPeaksPlugin.hxx"
#include "ChordRankingPlugin.hxx"
#include "LPModelViewPlugin.hxx"
#include "MelSpectrumViewPlugin.hxx"
#include "MelCepstrumViewPlugin.hxx"
#include "SegmentationViewPlugin.hxx"
#include "PixmapWidgetsPlugin.hxx"
#include "QFirstPersonPlugin.hxx"
#include "QSynthKnobPlugin.hxx"
#include "ControlSurfaceWidgetPlugin.hxx"
#include "ProgressControlWidgetPlugin.hxx"
#include <iostream>
#include <CLAM/CLAMVersion.hxx>

#include "CLAMWidgets.hxx"

CLAMWidgets::CLAMWidgets(QObject* parent)
	: QObject(parent)
{
	std::cout << "Loading CLAM widgets " << CLAM::GetFullVersion() << "..." << std::endl;
	_widgetPlugins
		<< new VumeterPlugin(this)
		<< new OscilloscopePlugin(this)
		<< new SpectrumViewPlugin(this)
		<< new PeakViewPlugin(this)
		<< new TonnetzPlugin(this)
		<< new KeySpacePlugin(this)
		<< new SpectrogramPlugin(this)
		<< new PolarChromaPeaksPlugin(this)
		<< new ChordRankingPlugin(this)
		<< new LPModelViewPlugin(this)
		<< new MelCepstrumViewPlugin(this)
		<< new MelSpectrumViewPlugin(this)
		<< new SegmentationViewPlugin(this)
		<< new PixmapSliderPlugin(this)
		<< new ThinPixmapButtonPlugin(this)
		<< new QSynthKnobPlugin(this)
		<< new ControlSurfaceWidgetPlugin(this)
		<< new ProgressControlWidgetPlugin(this)
		<< new QFirstPersonPlugin(this)
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



