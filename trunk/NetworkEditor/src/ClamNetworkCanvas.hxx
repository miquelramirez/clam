#ifndef ClamNetworkCanvas_hxx
#define ClamNetworkCanvas_hxx


#include <typeinfo>
#include <CLAM/XMLStorage.hxx>
#include <CLAM/Network.hxx>
#include <CLAM/ProcessingDataPlugin.hxx>
#include <CLAM/ProcessingFactory.hxx>
#include "OutControlSender.hxx"
#include "Configurator.hxx"
#include "NetworkCanvas.hxx"


class ClamNetworkCanvas : public NetworkCanvas
{
	Q_OBJECT
public:
	ClamNetworkCanvas(QWidget * parent=0)
		: NetworkCanvas(parent)
		, _network(0)
	{
		setWindowState(windowState() ^ Qt::WindowFullScreen);

		_newProcessingAction = new QAction(tr("&New Processing"), this);
		_newProcessingAction->setShortcut(QKeySequence(tr("Ctrl+Space")));
		addAction(_newProcessingAction);
		connect(_newProcessingAction, SIGNAL(triggered()), this, SLOT(onNewProcessing()));

		_newTextBoxAction = new QAction(tr("&New Text Box"), this);
		_newTextBoxAction->setShortcut(QKeySequence(tr("Ctrl+t")));
		addAction(_newTextBoxAction);
		connect(_newTextBoxAction, SIGNAL(triggered()), this, SLOT(onNewTextBox()));

		_copySelectionAction = new QAction(QIcon(":/icons/images/editcopy.png"), tr("&Copy"), this);
		_copySelectionAction->setShortcut(QKeySequence(tr("Ctrl+C")));
		addAction(_copySelectionAction);
		connect(_copySelectionAction, SIGNAL(triggered()), this, SLOT (onCopyProcessingsToClipboard()));

		_cutSelectionAction = new QAction(QIcon(":/icons/images/editcut.png"), tr("Cu&t"), this);
		_cutSelectionAction->setShortcut(QKeySequence(tr("Ctrl+X")));
		addAction(_cutSelectionAction);
		connect(_cutSelectionAction, SIGNAL(triggered()), this, SLOT (onCutProcessingsToClipboard()));

		_pasteSelectionAction = new QAction(QIcon(":/icons/images/editcopy.png"), tr("&Paste"), this);
		_pasteSelectionAction->setShortcut(QKeySequence(tr("Ctrl+V")));
		addAction(_pasteSelectionAction);
		connect(_pasteSelectionAction, SIGNAL(triggered()), this, SLOT (onPasteProcessingsFromClipboard()));
	}
	CLAM::Network & network()
	{
		return *_network;
	}

public: // Actions
	bool editConfiguration(ProcessingBox * box)
	{
		CLAM::Processing * processing = (CLAM::Processing *) box->model();
		CLAM::ProcessingConfig * config = (CLAM::ProcessingConfig*) processing->GetConfig().DeepCopy();
		Configurator configurator(*config);
		if (!configurator.exec())
		{
			delete config;
			return false;
		}
		network().ConfigureProcessing( box->getName().toStdString(), *config);	
		box->setProcessing(processing);
		refreshWires();
		delete config;
		return true;
	}

	virtual void * networkProcessing(const QString & name)
	{
		if (networkIsDummy()) return 0;
		return &_network->GetProcessing(name.toStdString());
	}

	virtual QString networkAddProcessing(const QString & type)
	{
		if (networkIsDummy()) return type;
		try
		{
			return _network->AddProcessing(type.toStdString()).c_str();
		}
		catch (CLAM::Err & e)
		{
			return "";
		}
	}

	virtual void createAndLinkToInControl( ProcessingBox * processing, QPoint point )
	{
		if (networkIsDummy()) return;

		unsigned controlIndex = processing->controlIndexByXPos(point);
		QString inControlName = processing->getIncontrolName(controlIndex);
		CLAM::Processing * model = (CLAM::Processing*) processing->model();
		CLAM::InControlBase& inControl = model->GetInControl(controlIndex);
		float defaultValue = inControl.DefaultValue();
		float lower = inControl.LowerBound();
		float upper = inControl.UpperBound();

		std::string controlSenderName  =  inControlName.toStdString();
		// add control-sender processing to network
		if (_network->HasProcessing(controlSenderName) )
		{
			controlSenderName = _network->GetUnusedName(controlSenderName);
		}
		_network->AddProcessing( controlSenderName, "OutControlSender");
		CLAM::Processing & controlSender = _network->GetProcessing( controlSenderName );
		// configure 
		CLAM::OutControlSenderConfig config;
		config.SetMin(lower);
		config.SetMax(upper);
		config.SetStep( std::max( (upper-lower)/200, CLAM::TControlData(0.01)) ); 
		config.SetDefault( defaultValue );
		controlSender.Configure( config );
		// add box to canvas and connect
		addProcessingBox( controlSenderName.c_str(), &controlSender, point+QPoint(0,-100));
		addControlConnection( getBox(controlSenderName.c_str()), 0, processing, controlIndex );

		markAsChanged();
	}

	virtual void createAndLinkToOutControl( ProcessingBox * processing, QPoint point )
	{
		if (networkIsDummy()) return;

		unsigned controlIndex = processing->controlIndexByXPos(point);
		QString outControlName = processing->getOutcontrolName(controlIndex);
		std::string controlPrinterName  =  outControlName.toStdString();
		// add Control Printer processing to network
		if (_network->HasProcessing(controlPrinterName) )
		{
			controlPrinterName = _network->GetUnusedName(controlPrinterName);
		}
		_network->AddProcessing( controlPrinterName, "ControlPrinter");
		CLAM::Processing & controlPrinter = _network->GetProcessing( controlPrinterName );
		// add box to canvas and connect
		addProcessingBox( controlPrinterName.c_str(), &controlPrinter, point+QPoint(0,100));
		addControlConnection( processing, controlIndex, getBox(controlPrinterName.c_str()), 0 );
		markAsChanged();
	}
	
	void createAndLinkToOutPort( ProcessingBox * processing, QPoint point, const QString & processingType)
	{
		if (networkIsDummy()) return;

		std::string type = processingType.toStdString();
		unsigned portIndex = processing->portIndexByYPos(point);
		QString outPortName = processing->getOutportName(portIndex);

		CLAM::Processing * model = (CLAM::Processing*)processing->model();
		if (type.empty())
		{
			// Choose default if any
			if (model->GetOutPort(portIndex).GetTypeId()==typeid(CLAM::TData))
				type = "AudioSink";
			else return;
		}

		std::string processingId = _network->AddProcessing(type);
		CLAM::Processing & portProcessing = _network->GetProcessing( processingId );
		// add box to canvas and connect
		addProcessingBox( processingId.c_str(), &portProcessing, point+QPoint(100,0));
		addPortConnection(processing, portIndex, getBox(processingId.c_str()), 0);
		markAsChanged();
	}

	void createAndLinkToInPort( ProcessingBox * processing, QPoint point, const QString & processingType)
	{
		if (networkIsDummy()) return;

		std::string type = processingType.toStdString();
		unsigned portIndex = processing->portIndexByYPos(point);
		QString inPortName = processing->getInportName(portIndex);
		CLAM::Processing * model = (CLAM::Processing*)processing->model();
		if (type.empty())
		{
			// Choose default if any
			if (model->GetInPort(portIndex).GetTypeId()==typeid(CLAM::TData))
				type = "AudioSource";
			else return;
		}

		std::string processingId = _network->AddProcessing(type);
		CLAM::Processing & portProcessing = _network->GetProcessing( processingId );
		// add box to canvas and connect
		addProcessingBox( processingId.c_str(), &portProcessing, point+QPoint(-200,0));
		addPortConnection(getBox(processingId.c_str()), 0, processing, portIndex);
		markAsChanged();
	}


	virtual QWidget * embededWidgetFor(void * processing);
	virtual unsigned nInports(void * processing) { return ((CLAM::Processing*)processing)->GetNInPorts();}
	virtual unsigned nOutports(void * processing) { return ((CLAM::Processing*)processing)->GetNOutPorts();}
	virtual unsigned nIncontrols(void * processing) { return ((CLAM::Processing*)processing)->GetNInControls();}
	virtual unsigned nOutcontrols(void * processing) { return ((CLAM::Processing*)processing)->GetNOutControls();}
	virtual QColor inportColor(void * element, unsigned index) const
	{
		if (!element) return colorPort();
		CLAM::Processing * processing = (CLAM::Processing*) element;
		const std::type_info& porttype = processing->GetInPort(index).GetTypeId();
		return getConnectorColorByType(porttype);
	}
	virtual QColor outportColor(void * element, unsigned index) const
	{
		if (!element) return colorPort();
		CLAM::Processing * processing = (CLAM::Processing*) element;
		const std::type_info& porttype = processing->GetOutPort(index).GetTypeId();
		return getConnectorColorByType(porttype);
	}
	virtual QColor incontrolColor(void * element, unsigned index) const
	{
		if (!element) return colorControl();
		CLAM::Processing * processing = (CLAM::Processing*) element;
		const std::type_info& controltype = processing->GetInControl(index).GetTypeId();
		return getConnectorColorByType(controltype);
	}
	virtual QColor outcontrolColor(void * element, unsigned index) const
	{
		if (!element) return colorControl();
		CLAM::Processing * processing = (CLAM::Processing*) element;
		const std::type_info& controltype = processing->GetOutControl(index).GetTypeId();
		return getConnectorColorByType(controltype);
	}
	virtual QString inportName(void * processing, unsigned index) const
	{
		if (!processing) return QString("Inport_%1").arg(index);
		CLAM::InPortBase & port = ((CLAM::Processing*)processing)->GetInPort(index);
		return port.GetName().c_str();
	}
	virtual QString outportName(void * processing, unsigned index) const
	{
		if (!processing) return QString("Outport_%1").arg(index);
		CLAM::OutPortBase & port = ((CLAM::Processing*)processing)->GetOutPort(index);
		return port.GetName().c_str();
	}
	virtual QString incontrolName(void * processing, unsigned index) const
	{
		if (!processing) return QString("Incontrol_%1").arg(index);
		CLAM::InControlBase & control = ((CLAM::Processing*)processing)->GetInControl(index);
		return control.GetName().c_str();
	}
	virtual QString outcontrolName(void * processing, unsigned index) const
	{
		if (!processing) return QString("Outcontrol_%1").arg(index);
		CLAM::OutControlBase & control = ((CLAM::Processing*)processing)->GetOutControl(index);
		return control.GetName().c_str();
	}
	template <typename Connector>
	QString connectorTooltip(Connector & connector) const
	{
		const char * typeString = CLAM::ProcessingDataPlugin::displayNameFor(connector.GetTypeId()).c_str();
		return QObject::tr("%1\nType: %2", "Connector tooltip")
			.arg(connector.GetName().c_str())
			.arg(typeString)
			;
	}
	QString outportTooltip(void * processing, unsigned index) const
	{
		if (!processing) return outportName(processing, index);
		CLAM::OutPortBase & port = ((CLAM::Processing*)processing)->GetOutPort(index);
		return connectorTooltip(port);
	}
	QString inportTooltip(void * processing, unsigned index) const
	{
		if (!processing) return inportName(processing, index);
		CLAM::InPortBase & port = ((CLAM::Processing*)processing)->GetInPort(index);
		return connectorTooltip(port);
	}
	QString outcontrolTooltip(void * processing, unsigned index) const
	{
		if (!processing) return outcontrolName(processing, index);
		CLAM::OutControlBase & control = ((CLAM::Processing*)processing)->GetOutControl(index);
		return connectorTooltip(control);
	}
	QString incontrolTooltip(void * processing, unsigned index) const
	{
		CLAM::InControlBase& inControl = ((CLAM::Processing*)processing)->GetInControl(index);
		// TODO: Bound info composing should be moved to the connector
		QString boundInfo = inControl.IsBounded() ? 
			QString("\n(bounds: [%1, %2] )")
				.arg(inControl.LowerBound())
				.arg(inControl.UpperBound()) :
			"\n(not bounded)";
		return connectorTooltip(inControl) + boundInfo;
	}
	virtual bool isOk(void * processing)
	{
		if (!processing) return true;
		return ((CLAM::Processing*)processing)->IsConfigured();
	}
	virtual QString errorMessage(void * processing)
	{
		if (!processing) return "";
		return ((CLAM::Processing*)processing)->GetConfigErrorMessage().c_str();
	}
	virtual QString infoMessage(void * processing)
	{
		if (!processing) return "";
		CLAM::Processing * proc = (CLAM::Processing*)processing;
		std::string key= proc->GetClassName();
		std::string info = "Type: "+key;
		CLAM::ProcessingFactory & factory = CLAM::ProcessingFactory::GetInstance();
		CLAM::ProcessingFactory::Pairs pairsFromKey=factory.GetPairsFromKey(key);
		CLAM::ProcessingFactory::Pairs::const_iterator itPairs;
		for(itPairs = pairsFromKey.begin();itPairs!=pairsFromKey.end();itPairs++)
		{
			std::string attribute = itPairs->attribute;
			std::string value = itPairs->value;
			if (attribute=="icon") continue;
			if (attribute=="embedded_svg") continue;
			if (value=="") continue;
			info+="\n"+attribute+": "+value;
		}
		return info.c_str();
	}
	virtual bool networkRenameProcessing(QString oldName, QString newName)
	{
		if (networkIsDummy()) return true;
		return _network->RenameProcessing(oldName.toStdString(), newName.toStdString());
	}


private:
	QColor getConnectorColorByType(const std::type_info & type) const
	{
		const char * colorstring = CLAM::ProcessingDataPlugin::colorFor(type).c_str();
		QColor color(colorstring);
		if (color.isValid()) return color;
		return colorPort();
	}

protected:
	bool canConnectPorts(ProcessingBox * source, unsigned outlet, ProcessingBox * target, unsigned inlet)
	{
		if (networkIsDummy()) return true;
		QString outName = source->getName() + "." + source->getOutportName(outlet);
		QString inName = target->getName() + "." + inportName(target->model(),inlet);
		CLAM::OutPortBase & out = _network->GetOutPortByCompleteName(outName.toStdString());
		CLAM::InPortBase & in = _network->GetInPortByCompleteName(inName.toStdString());
		return out.IsConnectableTo(in);
	}
	bool canConnectControls(ProcessingBox * source, unsigned outlet, ProcessingBox * target, unsigned inlet)
	{
		if (networkIsDummy()) return true;
		QString outName = source->getName() + "." + source->getOutcontrolName(outlet);
		QString inName = target->getName() + "." + incontrolName(target->model(),inlet);
		CLAM::OutControlBase & out = _network->GetOutControlByCompleteName(outName.toStdString());
		CLAM::InControlBase & in = _network->GetInControlByCompleteName(inName.toStdString());
		return out.IsLinkable(in);
	}
	virtual bool networkAddPortConnection(const QString & outlet, const QString & inlet)
	{
		if (networkIsDummy()) return true;
		return _network->ConnectPorts(outlet.toStdString(), inlet.toStdString());
	}
	virtual bool networkAddControlConnection(const QString & outlet, const QString & inlet)
	{
		if (networkIsDummy()) return true;
		return _network->ConnectControls(outlet.toStdString(), inlet.toStdString());
	}
	virtual bool networkRemovePortConnection(const QString & outlet, const QString & inlet)
	{
		if (networkIsDummy()) return true;
		return _network->DisconnectPorts(outlet.toStdString(), inlet.toStdString());
	}
	virtual bool networkRemoveControlConnection(const QString & outlet, const QString & inlet)
	{
		if (networkIsDummy()) return true;
		return _network->DisconnectControls(outlet.toStdString(), inlet.toStdString());
	}
	virtual void networkRemoveProcessing(const QString & name)
	{
		if (networkIsDummy()) return;
		_network->RemoveProcessing(name.toStdString());
	}
	virtual void networkRemoveTextBox(void * informationText)
	{
		if (networkIsDummy()) return;
		_network->removeInformationText((CLAM::InformationText*) informationText);
	}

public:
	bool networkIsDummy() const
	{
		return _network == 0;
	}
	void loadNetwork(CLAM::Network * network)
	{
		clear();
		_network = network;
		clearChanges();
		if (networkIsDummy()) return;
		typedef CLAM::Network::ProcessingsMap::const_iterator ProcIterator;
		for (ProcIterator it=_network->BeginProcessings(); it!=_network->EndProcessings(); it++)
		{
			const std::string & name = it->first;
			CLAM::Processing * processing = it->second;
			addProcessingBox( name.c_str(),  processing );
		}
		typedef CLAM::Network::InformationTexts::iterator TextIterator;
		for (TextIterator it=_network->BeginInformationTexts(); it!=_network->EndInformationTexts(); it++)
		{
			TextBox *textBox=new TextBox(this);
			textBox->setText(QString::fromLocal8Bit((*it)->text.c_str()));
			textBox->setPos(QPoint((*it)->x, (*it)->y));
			textBox->setInformationText(*it); // TODO: Too sensible to the order
			scene()->addItem(textBox);
			_textBoxes.push_back(textBox);
		}
		refreshWires();
	}


	void reloadNetwork(bool selectAllNew=false)
	{
		if (networkIsDummy()) return;
		CLAM::Network::ProcessingsMap::const_iterator it;
		for (it=_network->BeginProcessings(); it!=_network->EndProcessings(); it++)
		{
			const std::string & name = it->first;
			if (getBox(QString(name.c_str())))
				continue; // if the processing exists in canvas, skip it
			CLAM::Processing * processing = it->second;
			addProcessingBox( name.c_str(),  processing );
			if (selectAllNew)
				getBox(name.c_str())->select();
		}
		refreshWires();
	}
	void refreshWires()
	{
		if (not _network) return;
		clearWires();
		typedef CLAM::Network::NamesList Names;
		// TODO: Refactor this code please!!!
		for (unsigned p = 0; p<_processings.size(); p++)
		{
			std::string producerName = _processings[p]->getName().toStdString();
			CLAM::Processing & producer = _network->GetProcessing(producerName);
			unsigned nOutPorts = producer.GetNOutPorts();
			for (unsigned op = 0; op<nOutPorts; op++)
			{
				CLAM::OutPortBase & outPort = producer.GetOutPort(op);
				std::string completeOutName = producerName + "." + outPort.GetName();
				Names connected = _network->GetInPortsConnectedTo( completeOutName );
				for(Names::iterator inName=connected.begin(); inName!=connected.end(); inName++)
				{
					std::string consumerName = _network->GetProcessingIdentifier(*inName);
					ProcessingBox * consumerBox = getBox(consumerName.c_str());
					if (!consumerBox) continue; // TODO: Error?

					std::string peerConnection = _network->GetConnectorIdentifier(*inName);
					CLAM::Processing & consumer = _network->GetProcessing(consumerName);
					unsigned nInPorts = consumer.GetNInPorts();
					for (unsigned ip=0; ip<nInPorts; ip++)
					{
						CLAM::InPortBase & inPort = consumer.GetInPort(ip);
						if (inPort.GetName()!=peerConnection) continue;
						addPortWire(_processings[p], op, consumerBox, ip);
						break;
					}
				}
			}
			unsigned nOutControls = producer.GetNOutControls();
			for (unsigned op = 0; op<nOutControls; op++)
			{
				CLAM::OutControlBase & outControl = producer.GetOutControl(op);
				std::string completeOutName = producerName + "." + outControl.GetName();
				Names connected = _network->GetInControlsConnectedTo( completeOutName );
				for(Names::iterator inName=connected.begin(); inName!=connected.end(); inName++)
				{
					std::string consumerName = _network->GetProcessingIdentifier(*inName);
					ProcessingBox * consumerBox = getBox(consumerName.c_str());
					if (!consumerBox) continue; // TODO: Error?

					std::string peerConnection = _network->GetConnectorIdentifier(*inName);
					CLAM::Processing & consumer = _network->GetProcessing(consumerName);
					unsigned nInControls = consumer.GetNInControls();
					for (unsigned ip=0; ip<nInControls; ip++)
					{
						CLAM::InControlBase & inControl = consumer.GetInControl(ip);
						if (inControl.GetName()!=peerConnection) continue;
						addControlWire(_processings[p], op, consumerBox, ip);
						break;
					}
				}
			}
		}
	}

	bool updateGeometriesOnXML(QPoint offsetPoint=QPoint(0,0))
	{
		CLAM::Network::ProcessingsGeometriesMap processingsGeometriesMap;
		for (unsigned i=0; i<_processings.size(); i++)
		{
			CLAM::Network::Geometry processingGeometry;
			QPoint position = _processings[i]->position()-offsetPoint;
			QSize size = _processings[i]->size();
			const std::string name=_processings[i]->getName().toStdString();
			processingGeometry.x=position.x();
			processingGeometry.y=position.y();
			processingGeometry.width=size.width();
			processingGeometry.height=size.height();
			processingsGeometriesMap.insert(CLAM::Network::ProcessingsGeometriesMap::value_type(name,processingGeometry));
		}
		return (_network->SetProcessingsGeometries(processingsGeometriesMap));
	}
	bool loadGeometriesFromXML(QPoint offsetPoint = QPoint(0,0))
	{
		const CLAM::Network::ProcessingsGeometriesMap & processingsGeometriesMap=_network->GetAndClearGeometries();
		if (processingsGeometriesMap.empty())
			return 0;
		CLAM::Network::ProcessingsGeometriesMap::const_iterator it;
		for(it=processingsGeometriesMap.begin();it!=processingsGeometriesMap.end();it++)
		{
			QString name=QString(it->first.c_str());
			const CLAM::Network::Geometry & geometry=it->second;
			QPoint position=offsetPoint+QPoint(geometry.x,geometry.y);
			QSize size=QSize(geometry.width,geometry.height);
			ProcessingBox * box=getBox(name);
			box->move(position);
			box->resize(size);
		}
		return 1;
	}

	void setEmbedSVGDiagramsOption(bool state)
	{
		_embedSVGDiagramsOption=state;
	}

	bool embedSVGDiagramsOption()
	{
		return _embedSVGDiagramsOption;
	}

private slots:
	void onCopyConnection()
	{
		QPoint point = ((QAction*)sender())->data().toPoint();
		ProcessingBox * processing = processingUnder(point);
		ProcessingBox::Region region = processing->getRegion(point);
		QString toCopy;
		switch (region)
		{
			case ProcessingBox::outportsRegion: 
				toCopy = processing->getOutportPrototyperName(point);
				break;
			case ProcessingBox::inportsRegion: 
				toCopy = processing->getInportPrototyperName(point);
				break;
			case ProcessingBox::incontrolsRegion: 
				toCopy = processing->getIncontrolPrototyperName(point);
				break;
			case ProcessingBox::outcontrolsRegion: 
				toCopy = processing->getOutcontrolPrototyperName(point);
				break;
			default:
				return; // Matches a region but not a connector one, weird
		}
		QApplication::clipboard()->setText(toCopy);
	}

	void onCopyProcessingsToClipboard(bool cut=false)
	{
		typedef CLAM::Network::NamesList Names;
		Names processingNames;
		// Copy selected processings on networkToCopy
		for (unsigned i=0; i<_processings.size();i++)
		{
			if (!_processings[i]->isSelected())
				continue;
			const std::string name=(_processings[i]->getName()).toStdString();
			processingNames.push_back(name);
		}

		if (_network->UpdateSelections(processingNames))
			return;

		std::ostringstream streamXMLBuffer;
		updateGeometriesOnXML(getSelectionBoundingRect().topLeft());
		CLAM::XmlStorage::Dump(*_network,"network",streamXMLBuffer);
		QApplication::clipboard()->setText(QString(streamXMLBuffer.str().c_str()));

		if (not cut) return;

		for(Names::iterator it = processingNames.begin();
			it!=processingNames.end();
			it++)
		{
			removeProcessing(getBox( it->c_str() ));
		}
	}

	void onCutProcessingsToClipboard()
	{
		onCopyProcessingsToClipboard(true);
	}

	void onPasteProcessingsFromClipboard()
	{
		QPoint point = ((QAction*)sender())->data().toPoint();
		QString text=QApplication::clipboard()->text();
		std::stringstream streamXMLBuffer;
		streamXMLBuffer << text.toStdString();

		try
		{
			_network->setPasteMode();
			CLAM::XMLStorage::Restore(*_network, streamXMLBuffer);
		}
		catch(CLAM::XmlStorageErr &e)
		{
			QMessageBox::critical(this, tr("Error pasting from clipboard"), 
					tr("<p>An error occurred while pasting clipboard content.<p>"
						"<p><b>%1</b></p>").arg(e.what()));
			return;
		}
		clearSelections();
		bool selectAllNew=true;
		reloadNetwork(selectAllNew);
		loadGeometriesFromXML(point);
	}

	void onProcessingsConnectTo()
	{
		QMap<QString, QVariant> receiveMap=((QAction*)sender())->data().toMap();
		ProcessingBox * sourceBox = getBox(receiveMap["outprocessing"].toString());
		ProcessingBox * targetBox = getBox(receiveMap["inprocessing"].toString());
		unsigned sourceConnector = receiveMap["outlet"].toUInt();
		unsigned targetConnector = receiveMap["inlet"].toUInt();
		bool isPort = receiveMap["isPort"].toBool();
		if (isPort)
			addPortConnection(sourceBox,sourceConnector,targetBox,targetConnector);
		else
			addControlConnection(sourceBox,sourceConnector,targetBox,targetConnector);
		update();
	}

	virtual QString askProcessingType()
	{
		if (!_network)
			return QInputDialog::getText(
				this, tr("Type"), tr("Type"), QLineEdit::Normal, tr("A Processing"));

		QStringList completionList;
		typedef CLAM::ProcessingFactory::Keys FactoryKeys;
		FactoryKeys keys = CLAM::ProcessingFactory::GetInstance().GetKeys();
		for (FactoryKeys::const_iterator it=keys.begin(); it!=keys.end(); it++)
			completionList << QString::fromStdString(*it);
		QDialog dialog;
		dialog.setWindowTitle(tr("Adding a new processing"));
		QVBoxLayout * layout = new QVBoxLayout(&dialog);
		QLineEdit * lineEdit = new QLineEdit(&dialog);
		layout->addWidget(new QLabel(tr("Type"), &dialog));
		QCompleter *completer = new QCompleter(completionList, &dialog);
		completer->setCaseSensitivity(Qt::CaseInsensitive);
		lineEdit->setCompleter(completer);
		layout->addWidget(lineEdit);
		QDialogButtonBox * buttons = new QDialogButtonBox(QDialogButtonBox::Ok|QDialogButtonBox::Cancel, Qt::Horizontal, &dialog);
		connect(buttons, SIGNAL(accepted()), &dialog, SLOT(accept()));
		connect(buttons, SIGNAL(rejected()), &dialog, SLOT(reject()));
		layout->addWidget(buttons);
		if (dialog.exec()==QDialog::Rejected) return QString();
		return lineEdit->text();
	}
	void onNewTextBox()
	{
		QPoint point = ((QAction*)sender())->data().toPoint();
		addTextBox(point);
	}

	void addTextBox(const QPoint& point)
	{
		QString newText = askText(tr("New text box"));
		if (newText.isNull()) return;

		CLAM::InformationText * informationText= new CLAM::InformationText();
		_network->addInformationText(informationText);

		TextBox * textbox = new TextBox(this);
		textbox->setInformationText(informationText);
		textbox->setText(newText);
		textbox->setPos(point);
		scene()->addItem(textbox);
		_textBoxes.push_back(textbox);
		markAsChanged();
	}
	void editTextBox(TextBox * textbox)
	{
		QString newText = askText(tr("Editing text box"), textbox->toPlainText());
		if (newText.isNull()) return;
		textbox->setText(newText);
		markAsChanged();
	}
	QString askText(const QString & title, const QString & initialValue="")
	{
		QDialog dialog;
		dialog.setWindowTitle(title);
		QVBoxLayout * layout = new QVBoxLayout(&dialog);
		QPlainTextEdit * textEdit = new QPlainTextEdit(&dialog);
		layout->addWidget(textEdit);
		textEdit->setPlainText(initialValue);

		QDialogButtonBox * buttons = new QDialogButtonBox(QDialogButtonBox::Ok|QDialogButtonBox::Cancel, Qt::Horizontal, &dialog);
		connect(buttons, SIGNAL(accepted()), &dialog, SLOT(accept()));
		connect(buttons, SIGNAL(rejected()), &dialog, SLOT(reject()));
		layout->addWidget(buttons);
		int result = dialog.exec();
		if (result==QDialog::Rejected) return QString(); // Null
		return textEdit->toPlainText();
	}
	virtual void networkUpdateTextBox(void * modelText, const QString & text, const QPointF & pos)
	{
		if (not modelText) return;
		CLAM::InformationText * clamText = (CLAM::InformationText *) modelText;
		clamText->text=text.toStdString();
		clamText->x=pos.x();
		clamText->y=pos.y();
	}
	void onOpenFileWithExternalApplication()
	{
		const QString fileName = ((QAction*)sender())->data().toString();
		QDesktopServices::openUrl(QUrl::fromLocalFile(fileName));
	}

	void onBrowseUrl()
	{
		const QString fileName = ((QAction*)sender())->data().toString();
		emit browseUrlRequest(fileName);
	}

signals:
	void browseUrlRequest(const QString & fileName);

private:
	std::string outportTypeId(void * processing, unsigned index) const
	{
		if (!processing) return "";
		return ((CLAM::Processing*)processing)->GetOutPort(index).GetTypeId().name();
	}

	std::string inportTypeId(void * processing, unsigned index) const
	{
		if (!processing) return "";
		return ((CLAM::Processing*)processing)->GetInPort(index).GetTypeId().name();
	}


	virtual void connectionContextMenu(QMenu * menu, const QPoint& cursorPosition, ProcessingBox * processing, ProcessingBox::Region region)
	{
		menu->addAction(QIcon(":/icons/images/remove.png"), tr("Disconnect"),
			this, SLOT(onDisconnect()))->setData(cursorPosition);
		menu->addAction(QIcon(":/icons/images/editcopy.png"), tr("Copy connection name"),
			this, SLOT(onCopyConnection()))->setData(cursorPosition);

		if (not processing->model()) return;
		if (region==ProcessingBox::incontrolsRegion)
		{
			menu->addAction(QIcon(":/icons/images/hslider.png"), tr("Add slider"),
				this, SLOT(onAddLinkedProcessing()))->setData(cursorPosition);
		}
		if (region==ProcessingBox::outcontrolsRegion)
		{
			menu->addAction(QIcon(":/icons/images/processing.png"), tr("Add control printer"),
				this, SLOT(onAddLinkedProcessing()))->setData(cursorPosition);
		}
		if (region==ProcessingBox::outportsRegion)
		{
			typedef CLAM::ProcessingFactory::Keys Keys;
			menu->addSeparator();
			CLAM::ProcessingFactory & factory = CLAM::ProcessingFactory::GetInstance();
			unsigned portindex = processing->portIndexByYPos(cursorPosition);
			std::string outportType = outportTypeId(processing->model(),portindex);
			Keys keys = factory.GetKeys("port_monitor_type", outportType);
			for (Keys::const_iterator it=keys.begin(); it!=keys.end(); it++)
			{
				const char* key = it->c_str();
				menu->addAction( clamProcessingIcon(key), key,
					this, SLOT(onAddLinkedProcessing()))->setData(cursorPosition);
			}
			if (outportType==typeid(CLAM::TData).name()) // Check for audio port
			{
				const char* key="AudioSink";
				menu->addAction( clamProcessingIcon(key), key,
					this, SLOT(onAddLinkedProcessing()))->setData(cursorPosition);
			}
		}
		if (region==ProcessingBox::inportsRegion)
		{
			unsigned portindex = processing->portIndexByYPos(cursorPosition);
			std::string inportType=inportTypeId(processing->model(),portindex);

			if (inportType==typeid(CLAM::TData).name()) // Check for audio port
			{
				const char* key="AudioSource";
				menu->addAction( clamProcessingIcon(key), key,
					this ,SLOT(onAddLinkedProcessing()))->setData(cursorPosition);
			}
		}
		// Check for compatible connectors and add "Connect To" submenus
		{
			bool isPortConnection = region == ProcessingBox::outportsRegion || region == ProcessingBox::inportsRegion;
			bool selfIsInput = region == ProcessingBox::incontrolsRegion || region == ProcessingBox::inportsRegion;

			// Self reference connector
			ProcessingBox * selfProcessing = processing; // Just an alias to be symmetric to 'peer'
			unsigned selfConnection = isPortConnection ?
					selfProcessing->portIndexByYPos(cursorPosition):
					selfProcessing->controlIndexByXPos(cursorPosition);

			// Peer: To be given values along the bucle
			ProcessingBox * peerProcessing;
			unsigned peerConnection;

			// Choosing in/out in respect of self/peer
			ProcessingBox ** inProcessing  = selfIsInput? &selfProcessing : &peerProcessing;
			ProcessingBox ** outProcessing = selfIsInput? &peerProcessing : &selfProcessing;
			unsigned * inlet  = selfIsInput ? &selfConnection : &peerConnection;
			unsigned * outlet = selfIsInput ? &peerConnection : &selfConnection;
			
			menu->addSeparator();
			for (unsigned i = 0; i<_processings.size(); i++ )
			{
				peerProcessing =_processings[i];
				if (peerProcessing==selfProcessing) continue; // TODO: Loops not allowed by now

				typedef QMap<QString, QVariant> ConnectionMap;
				QList<ConnectionMap> listConnectionsMap;
				unsigned nPeerConnectors = nConnectors(peerProcessing->model(), peerRegion(region));
				for (peerConnection=0; peerConnection<nPeerConnectors; peerConnection++)
				{
					// Filter out incompatible connections
					if (isPortConnection and not canConnectPorts(*outProcessing, *outlet, *inProcessing, *inlet))
						continue;
					if (not isPortConnection and not canConnectControls(*outProcessing, *outlet, *inProcessing, *inlet))
						continue;

					QString peerConnectionName=connectorName(peerProcessing->model(), peerRegion(region), peerConnection);
					ConnectionMap connectionMap;
					connectionMap["outprocessing"]=(*outProcessing)->getName();	// QString
					connectionMap["inprocessing"]=(*inProcessing)->getName();	// QString
					connectionMap["outlet"]=*outlet;			//unsigned
					connectionMap["inlet"]=*inlet;			//unsigned
					connectionMap["isPort"]=isPortConnection;			//bool
					connectionMap["connectionName"]=peerConnectionName;		//QString
					listConnectionsMap.push_back(connectionMap);
				}

				if (listConnectionsMap.empty())
					continue; // no compatible connections, skip
				QIcon icon = processingIcon(peerProcessing);
				if (listConnectionsMap.size()==1) // one compatible connection
				{
					ConnectionMap connectionMap = listConnectionsMap.front();
					QString connectionName=connectionMap["connectionName"].toString();
					menu->addAction(icon,
						tr("Connect to %1.%2")
							.arg(peerProcessing->getName())
							.arg(connectionName),
						this, SLOT(onProcessingsConnectTo()))->setData(connectionMap);
					continue;
				}
				//more than one compatible connection:
				QMenu * submenu=menu->addMenu(icon,
					tr("Connect to %1").arg(peerProcessing->getName()));
				QList<ConnectionMap>::const_iterator itConnectionsMaps=listConnectionsMap.constBegin();
				while (itConnectionsMaps!=listConnectionsMap.constEnd())
				{
					ConnectionMap connectionMap= *itConnectionsMaps;
					QString connectionName=connectionMap["connectionName"].toString();
					submenu->addAction(connectionName,
						this, SLOT(onProcessingsConnectTo()))->setData(connectionMap);
					itConnectionsMaps++;
				}
			}
		}
	}

	virtual void processingContextMenu(QMenu * menu, const QPoint& point, ProcessingBox * processing)
	{
		menu->addAction(QIcon(":/icons/images/configure.png"), tr("Configure"),
			this, SLOT(onConfigure()))->setData(point);
		menu->addAction(QIcon(":/icons/images/editclear.png"), tr("Rename"),
			this, SLOT(onRename()))->setData(point);
		menu->addAction(_deleteSelectedAction);
		menu->addAction(_copySelectionAction);
		menu->addAction(_cutSelectionAction);

		if (not processing->model()) return;

		CLAM::ProcessingFactory & factory = CLAM::ProcessingFactory::GetInstance();
		std::string className=((CLAM::Processing*)processing->model())->GetClassName();
		if (factory.AttributeExists(className,"faust_diagram"))
		{
			QString fileName=QString(factory.GetValueFromAttribute(className,"faust_diagram").c_str());
			menu->addSeparator();
			//menu->addAction(clamProcessingIcon(className),tr("Open diagram with browser"),this,SLOT(onOpenFileWithExternalApplication()))->setData(fileName);
			menu->addAction(
				clamProcessingIcon(className),
				tr("Navigate Faust diagram"),
				this,SLOT(onBrowseUrl())
				)->setData(fileName);
		}
		if (factory.AttributeExists(className,"faust_source_file"))
		{
			QString fileName=QString(factory.GetValueFromAttribute(className,"faust_source_file").c_str());
			menu->addAction(
				clamProcessingIcon(className),
				tr("Open source with editor"),
				this,SLOT(onOpenFileWithExternalApplication())
				)->setData(fileName);
			//menu->addAction(clamProcessingIcon(className),"Recompile plugin");
		}
	}

	virtual void canvasContextMenu(QMenu * menu, const QPoint& point)
	{
		_pasteSelectionAction->setData(point);
		menu->addAction(_pasteSelectionAction);
		_newProcessingAction->setData(point);
		menu->addAction(_newProcessingAction);
		_newTextBoxAction->setData(point);
		menu->addAction(_newTextBoxAction);
	}

	virtual QIcon processingIcon(ProcessingBox * processingBox)
	{
		if (not processingBox->model()) return  QIcon(":/icons/images/processing.png");
		const char* className=((CLAM::Processing*)processingBox->model())->GetClassName();
		return clamProcessingIcon(className);
	}
private:
	QIcon clamProcessingIcon(const std::string & className)
	{
		CLAM::ProcessingFactory & factory = CLAM::ProcessingFactory::GetInstance();
		if (not factory.AttributeExists(className,"icon"))
			return  QIcon(":/icons/images/processing.png");
		std::string iconPath=factory.GetValueFromAttribute(className,"icon");
		return QIcon( QString(":/icons/images/%1").arg(iconPath.c_str()));
	}

protected:
	QAction * _newTextBoxAction;
	QAction * _newProcessingAction;
	QAction * _copySelectionAction;
	QAction * _cutSelectionAction;
	QAction * _pasteSelectionAction;
private:
	CLAM::Network * _network;
	bool _embedSVGDiagramsOption;
};

#endif//ClamNetworkCanvas_hxx

