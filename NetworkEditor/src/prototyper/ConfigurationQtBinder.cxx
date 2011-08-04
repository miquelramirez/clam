#include "ConfigurationQtBinder.hxx"

static CLAM::ConfigurationQtBinder configuratorBinder;

void ConfigurationLauncher::launch()
{
	if (not _network.HasProcessing(_processingName)) return;
	CLAM::Processing & processing = _network.GetProcessing(_processingName);
	CLAM::ProcessingConfig * config =
		(CLAM::ProcessingConfig*) processing.GetConfig().DeepCopy();

	Configurator configurator(*config);
	if (not configurator.exec())
	{
		delete config;
		return;
	}
	// TODO: Consider the pause case as well
	bool wasPlaying = _network.IsPlaying();

	if (wasPlaying) _network.Stop();
	processing.Configure(*config);
	delete config;
	if (processing.IsConfigured())
	{
		if (wasPlaying) _network.Start();
		return;
	}
	if (not processing.IsConfigured())
	{
		QMessageBox::critical(
			0,
			tr("Error configuring processing"),
			tr(processing.GetConfigErrorMessage().c_str())
			);
	}
}

bool CLAM::ConfigurationQtBinder::handles(QObject * uiElement)
{
	if (isSubClassOf<QAction>(uiElement))
	{
		if (uiElement->objectName().startsWith(_prefix)) return true;
		if (uiElement->objectName().startsWith(_actionPrefix)) return true;
		if (hasProperty(uiElement, _portProperty)) return true;
	}
	else if (isSubClassOf<QAbstractButton>(uiElement))
	{
		if (uiElement->objectName().startsWith(_prefix)) return true;
		if (hasProperty(uiElement, _portProperty)) return true;
	}
	return false;
}

bool CLAM::ConfigurationQtBinder::bind(QObject * uiElement, CLAM::Network & network, QStringList & errors)
{
	// Just to be sure, no guaranties the user has used handles before to check
	if (not isSubClassOf<QAction>(uiElement)) 
		if (not isSubClassOf<QAbstractButton>(uiElement)) return error(errors,
			QString("CLAM::ConfigurationQtBinder: Expected a QAction or a QAbstractButton."));

	std::string processingName;
	if (uiElement->objectName().startsWith(_actionPrefix))
		processingName = widget2NetworkName(_actionPrefix, uiElement->objectName());
	else if (uiElement->objectName().startsWith(_prefix))
		processingName = widget2NetworkName(_prefix, uiElement->objectName());
	else if (hasProperty(uiElement, _portProperty))
		processingName = uiElement->property(_portProperty).toString().toStdString();
	else return error(errors,
		QString("CLAM::ConfigurationQtBinder: Action name '%1' should start by with '%2' or '%3', "
			"or have the '%4' custom property defined")
			.arg(uiElement->objectName())
			.arg(_prefix)
			.arg(_actionPrefix)
			.arg(_portProperty)
			);

	std::cout << "* Configuration binder connecting " 
		<< "QAction " << uiElement->objectName().toStdString() 
		<< " to processing '" << processingName << "'"
		<< std::endl;

	if (missingProcessing(processingName,network,errors)) return false;
	ConfigurationLauncher * launcher = new ConfigurationLauncher(network, processingName);
	launcher->setParent(uiElement);

	if (isSubClassOf<QAction>(uiElement)) 
		QObject::connect(uiElement, SIGNAL(triggered()), launcher, SLOT(launch()));
	else
		QObject::connect(uiElement, SIGNAL(clicked()), launcher, SLOT(launch()));

	return true;
}

