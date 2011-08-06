#include "AudioFileReaderQtBinder.hxx"
#include <CLAM/Network.hxx>
#include <CLAM/MonoAudioFileReaderConfig.hxx>
#include <QtGui/QFileDialog>
#include <QtGui/QAbstractButton>
#include <QtGui/QAction>

static CLAM::AudioFileReaderQtBinder audioFileReaderBinder;

void CLAM::AudioFileReaderQtBind::launch()
{
	QObject * loadButton = sender();
	std::string processingName = loadButton->objectName().mid(12).toStdString();
	std::cout << "Loading audio for " << _processingName << std::endl;
	CLAM::Processing & processing = _network.GetProcessing(_processingName);
	CLAM::MonoAudioFileReaderConfig config =
		dynamic_cast<const CLAM::MonoAudioFileReaderConfig &> (processing.GetConfig());
	QString filename = 
		QFileDialog::getOpenFileName(0, 
			tr("Choose an audio file"),
			QString::fromLocal8Bit(config.GetSourceFile().c_str()),
			tr("Audio files (*.wav *.ogg *.mp3)")
			);
	if (filename.isEmpty()) return;
	config.SetSourceFile(filename.toLocal8Bit().constData());
	_network.Stop();
	processing.Configure(config);
	_network.Start();
}

bool CLAM::AudioFileReaderQtBinder::handles(QObject * uiElement)
{
	if (not uiElement->objectName().startsWith(_prefix)) return false;
	if (isSubClassOf<QAction>(uiElement)) return true;
	if (isSubClassOf<QAbstractButton>(uiElement)) return true;
	return false;
}

bool CLAM::AudioFileReaderQtBinder::bind(QObject * uiElement, CLAM::Network & network, QStringList & errors)
{
	// Just to be sure, no guaranties the user has used handles before to check
	if (not isSubClassOf<QAction>(uiElement)) 
		if (not isSubClassOf<QAbstractButton>(uiElement)) return error(errors,
			QString("AudioFileReaderQtBinder: Expected a QAction or a QAbstractButton."));

	if (not uiElement->objectName().startsWith(_prefix)) return error(errors,
		QString("CLAM::AudioFileReaderQtBinder: Action name '%1' should start by with '%2'.")
			.arg(uiElement->objectName())
			.arg(_prefix)
			);

	std::string processingName = widget2NetworkName(_prefix, uiElement->objectName());
	std::cout << "* Load Audio File Button/Action connected to Audio file reader '" << processingName << "'" << std::endl;

	if (missingProcessing(processingName,network,errors)) return false;
	AudioFileReaderQtBind * launcher = new AudioFileReaderQtBind(network, processingName);
	launcher->setParent(uiElement);

	if (isSubClassOf<QAction>(uiElement)) 
		QObject::connect(uiElement, SIGNAL(triggered()), launcher, SLOT(launch()));
	else
		QObject::connect(uiElement, SIGNAL(clicked()), launcher, SLOT(launch()));

	return true;
}

