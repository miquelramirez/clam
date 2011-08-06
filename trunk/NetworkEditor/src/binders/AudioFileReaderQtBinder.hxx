#ifndef CLAM_ConfigurationQtBinder_hxx
#define CLAM_ConfigurationQtBinder_hxx

#include "QtBinder.hxx"

namespace CLAM { class AudioFileReaderQtBind; }
namespace CLAM { class AudioFileReaderQtBinder; }


class CLAM::AudioFileReaderQtBind : public QObject
{
Q_OBJECT
private:
	CLAM::Network & _network;
	std::string _processingName;
public:
	AudioFileReaderQtBind(CLAM::Network & network, const std::string & processingName)
		: _network(network)
		, _processingName(processingName)
	{
	}
public slots:
	void launch();
};


/**
	Binds a clickable or triggerable widget or action to a dialog
	which asks for an audio file to configure an AudioFileReader processing
	in the underlying network.

	To use this binder the widget or action should be named with a prefix
	'AudioFile__' followed by the mangled name of the processing (turning
	space into three undelines '___'.

	@ingroup UiBinders
*/
class CLAM::AudioFileReaderQtBinder : public CLAM::QtBinder
{
	QString _prefix;
public:

	AudioFileReaderQtBinder()
		: _prefix("AudioFile__")
	{}
	bool handles(QObject * uiElement);
	bool bind(QObject * uiElement, CLAM::Network & network, QStringList & errors);
};

#endif//CLAM_ConfigurationQtBinder_hxx

