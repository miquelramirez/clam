#include "ConfiguratorPlugin.hxx"
#include <cstdlib>
#include <limits>

/**
 * Abstract class for plugins to add new editable types to configuration dialogs
 */


class NullConfiguratorPlugin : public ConfiguratorPlugin
{
public:
	virtual bool accepts(const CLAM::DynamicType & object, unsigned attribute)
	{
		return true;
	}
	virtual QWidget * editorWidget(const CLAM::DynamicType & object, unsigned attribute)
	{
		return new QLabel("Non editable type");
	}
	virtual void takeFromWidget(const CLAM::DynamicType & object, unsigned attribute, QWidget * editorWidget)
	{
	}
};

ConfiguratorPlugin & ConfiguratorPlugin::GetPlugin(const CLAM::DynamicType & object, unsigned attribute)
{
	static NullConfiguratorPlugin nullPlugin;
	for (Map::iterator it = GetList().begin(); it!=GetList().end(); it++)
	{
		if ((*it)->accepts(object,attribute)) return **it;
	}
	return nullPlugin;
}

// TODO: It has more sense to use UTF-8 as internal representation but actual XML converts to local 8 bits
#include <CLAM/Text.hxx>
#include <QtGui/QLineEdit>
template <typename ConcreteString>
class StringConfiguratorPlugin : public ConfiguratorPlugin
{
public:
	virtual bool accepts(const CLAM::DynamicType & object, unsigned attribute)
	{
		return object.GetTypeId(attribute) == typeid(ConcreteString);
	}
	virtual QWidget * editorWidget(const CLAM::DynamicType & object, unsigned attribute)
	{
		const ConcreteString & value = *(ConcreteString *)object.GetAttributeAsVoidPtr(attribute);
		return new QLineEdit(QString::fromLocal8Bit(value.c_str()));
	}
	virtual void takeFromWidget(const CLAM::DynamicType & object, unsigned attribute, QWidget * editorWidget)
	{
		QLineEdit * input = dynamic_cast<QLineEdit*>(editorWidget);
		ConcreteString & value = *(ConcreteString *)object.GetAttributeAsVoidPtr(attribute);
		value = input->text().toLocal8Bit().constData();
	}
};
static StringConfiguratorPlugin<std::string> stringRegistrator;
static StringConfiguratorPlugin<CLAM::Text> textRegistrator;


#include <QtGui/QDoubleSpinBox>
template <typename ConcreteFloat>
class FloatConfiguratorPlugin : public ConfiguratorPlugin
{
public:
	virtual bool accepts(const CLAM::DynamicType & object, unsigned attribute)
	{
		return object.GetTypeId(attribute) == typeid(ConcreteFloat);
	}
	virtual QWidget * editorWidget(const CLAM::DynamicType & object, unsigned attribute)
	{
		const ConcreteFloat & value = *(ConcreteFloat *)object.GetAttributeAsVoidPtr(attribute);
		QDoubleSpinBox * input = new QDoubleSpinBox;
		input->setDecimals(5);
		input->setRange(-std::numeric_limits<ConcreteFloat>::max(),std::numeric_limits<ConcreteFloat>::max());
		input->setValue(value);
		return input;
	}
	virtual void takeFromWidget(const CLAM::DynamicType & object, unsigned attribute, QWidget * editorWidget)
	{
		QDoubleSpinBox * input = dynamic_cast<QDoubleSpinBox*>(editorWidget);
		ConcreteFloat & value = *(ConcreteFloat *)object.GetAttributeAsVoidPtr(attribute);
		value = input->value();
	}
};
static FloatConfiguratorPlugin<float> floatRegistrator;
static FloatConfiguratorPlugin<double> doubleRegistrator;
static FloatConfiguratorPlugin<long double> longDoubleRegistrator;


#include <QtGui/QSpinBox>
template <typename ConcreteInt>
class IntConfiguratorPlugin : public ConfiguratorPlugin
{
public:
	virtual bool accepts(const CLAM::DynamicType & object, unsigned attribute)
	{
		return object.GetTypeId(attribute) == typeid(ConcreteInt);
	}
	virtual QWidget * editorWidget(const CLAM::DynamicType & object, unsigned attribute)
	{
		const ConcreteInt & value = *(ConcreteInt *)object.GetAttributeAsVoidPtr(attribute);
		QSpinBox * input = new QSpinBox;
		input->setRange(
			std::numeric_limits<ConcreteInt>::min(),
			std::min(
				(unsigned long) std::numeric_limits<ConcreteInt>::max(),
				(unsigned long) std::numeric_limits<int>::max()));
		input->setValue(value);
		return input;
	}
	virtual void takeFromWidget(const CLAM::DynamicType & object, unsigned attribute, QWidget * editorWidget)
	{
		QSpinBox * input = dynamic_cast<QSpinBox*>(editorWidget);
		ConcreteInt & value = *(ConcreteInt *)object.GetAttributeAsVoidPtr(attribute);
		value = input->value();
	}
};
static IntConfiguratorPlugin<int> IntRegistrator;
static IntConfiguratorPlugin<unsigned int> unsignedIntRegistrator;
static IntConfiguratorPlugin<unsigned> unsignedRegistrator;



#include <QtGui/QCheckBox>

class BoolConfiguratorPlugin : public ConfiguratorPlugin
{
public:
	virtual bool accepts(const CLAM::DynamicType & object, unsigned attribute)
	{
		return object.GetTypeId(attribute) == typeid(bool);
	}
	virtual QWidget * editorWidget(const CLAM::DynamicType & object, unsigned attribute)
	{
		const bool * value = (bool *)object.GetAttributeAsVoidPtr(attribute);
		QCheckBox * input = new QCheckBox;
		input->setChecked(value && *value);
		return input;
	}
	virtual void takeFromWidget(const CLAM::DynamicType & object, unsigned attribute, QWidget * editorWidget)
	{
		QCheckBox * input = dynamic_cast<QCheckBox*>(editorWidget);
		bool & value = *(bool *)object.GetAttributeAsVoidPtr(attribute);
		value = input->isChecked();
	}
};
static BoolConfiguratorPlugin boolRegistrator;



#include <QtGui/QComboBox>
#include <CLAM/Enum.hxx>

class EnumConfiguratorPlugin : public ConfiguratorPlugin
{
public:
	virtual bool accepts(const CLAM::DynamicType & object, unsigned attribute)
	{
		const CLAM::Component * component = object.GetAttributeAsComponent(attribute);
		if (!component) return false;
		return dynamic_cast<const CLAM::Enum*>(component);
	}
	virtual QWidget * editorWidget(const CLAM::DynamicType & object, unsigned attribute)
	{
		const CLAM::Enum & value = *(CLAM::Enum *)object.GetAttributeAsVoidPtr(attribute);
		QComboBox * input = new QComboBox(/*editable*/ false);
		const CLAM::Enum::tEnumValue * mapping = value.GetSymbolMap();
		for (unsigned i = 0; mapping[i].name; i++) {
			input->addItem( mapping[i].name );
			if (mapping[i].value==value.GetValue()) input->setCurrentIndex(i);
		}
		return input;
	}
	virtual void takeFromWidget(const CLAM::DynamicType & object, unsigned attribute, QWidget * editorWidget)
	{
		QComboBox * input = dynamic_cast<QComboBox*>(editorWidget);
		CLAM::Enum & value = *(CLAM::Enum *)object.GetAttributeAsVoidPtr(attribute);
		const CLAM::Enum::tEnumValue * mapping = value.GetSymbolMap();
		CLAM_BEGIN_CHECK
			int i;
			for (i = 0; mapping[i].name; i++);
			CLAM_ASSERT(input->currentIndex()<i, "Configurator: Choice value out of range");
		CLAM_END_CHECK
		value=mapping[input->currentIndex()].value;
	}
};
static EnumConfiguratorPlugin enumRegistrator;



#include "Configurator.hxx"

class DynamicTypeConfiguratorPlugin : public ConfiguratorPlugin
{
public:
	virtual bool accepts(const CLAM::DynamicType & object, unsigned attribute)
	{
		return object.AttributeIsDynamictype(attribute);
	}
	virtual QWidget * editorWidget(const CLAM::DynamicType & object, unsigned attribute)
	{
		CLAM::DynamicType & value = *(CLAM::DynamicType *)object.GetAttributeAsVoidPtr(attribute);
		QPushButton * input = new QPushButton("Details...");
		input->setAutoDefault(false);
		Configurator * subConfigurator = new Configurator(value);
		QObject::connect( input, SIGNAL(clicked()), subConfigurator, SLOT(exec()) );
		return input;
	}
	virtual void takeFromWidget(const CLAM::DynamicType & object, unsigned attribute, QWidget * editorWidget)
	{
	}
};
static DynamicTypeConfiguratorPlugin dtRegistrator;


#include <QtGui/QGraphicsView>
#include <CLAM/BPF.hxx>

class BPFConfiguratorPlugin : public ConfiguratorPlugin
{
public:
	virtual bool accepts(const CLAM::DynamicType & object, unsigned attribute)
	{
		const CLAM::Component * component = object.GetAttributeAsComponent(attribute);
		if (!component) return false;
		return dynamic_cast<const CLAM::BPF*>(component);
	}
	virtual QWidget * editorWidget(const CLAM::DynamicType & object, unsigned attribute)
	{
//		const CLAM::BPF & value = *(CLAM::BPF *)object.GetAttributeAsVoidPtr(attribute);
		QGraphicsView * input = new QGraphicsView;
		return input;
	}
	virtual void takeFromWidget(const CLAM::DynamicType & object, unsigned attribute, QWidget * editorWidget)
	{
//		QGraphicsView * input = dynamic_cast<QGraphicsView*>(editorWidget);
//		CLAM::BPF & value = *(CLAM::BPF *)object.GetAttributeAsVoidPtr(attribute);
	}
};
static BPFConfiguratorPlugin bpfRegistrator;



#include "QFileLineEdit.hxx"
#include <CLAM/AudioInFilename.hxx>
#include <CLAM/AudioOutFilename.hxx>
template <typename ConcreteFilename, bool writeMode>
class FilenameConfiguratorPlugin : public ConfiguratorPlugin
{
public:
	virtual bool accepts(const CLAM::DynamicType & object, unsigned attribute)
	{
		return object.GetTypeId(attribute) == typeid(ConcreteFilename);
	}
	virtual QWidget * editorWidget(const CLAM::DynamicType & object, unsigned attribute)
	{
		ConcreteFilename & value = *(ConcreteFilename *)object.GetAttributeAsVoidPtr(attribute);
		const char *typeFamily = value.TypeFamily();
		QFileLineEdit * input = new QFileLineEdit;
		input->setWriteMode(writeMode);
		input->setFilters(filterString(typeFamily, value.Filters(), writeMode));
		input->setLocation(QString::fromLocal8Bit(value.c_str()));

		const char *modeStr = writeMode ? "output" : "input";
		const char *sep = typeFamily[0] ? " " : "";
		input->setDialogCaption(
			QObject::tr("Select a(n) %1%2%3 file")
				.arg(modeStr)
				.arg(sep)
				.arg(typeFamily) );

		return input;
	}
	virtual void takeFromWidget(const CLAM::DynamicType & object, unsigned attribute, QWidget * editorWidget)
	{
		QFileLineEdit * input = dynamic_cast<QFileLineEdit*>(editorWidget);
		ConcreteFilename & value = *(ConcreteFilename *)object.GetAttributeAsVoidPtr(attribute);
		value = input->location().toLocal8Bit().constData();
	}
private:
	QString filterString(const char *typeFamily,
			const CLAM::Filename::Filter * filters,
			bool inWrite)
	{
		if (!filters[0].description)
			return QObject::tr("All %1 files (*.*)").arg(typeFamily);
		QString qtfilter, allfilter;
		const char *separator = "";
		for (const CLAM::Filename::Filter* filter = filters; filter->description; filter++)
		{
			qtfilter += QString("%1%2 (%3)")
				.arg(separator)
				.arg(filter->description)
				.arg(filter->wildcard)
				;
			separator = ";;";
			allfilter += filter->wildcard;
			allfilter += " ";
		}
		QString allfiles(QObject::tr("All %1 files (%2)")
				.arg(typeFamily)
				.arg(allfilter) );
		if (inWrite)
			allfiles = qtfilter + separator + allfiles;
		return allfiles;
	}

};
static FilenameConfiguratorPlugin<CLAM::InFilename,false> inFileRegistrator;
static FilenameConfiguratorPlugin<CLAM::AudioInFilename,false> audioInFileRegistrator;
static FilenameConfiguratorPlugin<CLAM::OutFilename,true> outFileRegistrator;
static FilenameConfiguratorPlugin<CLAM::AudioOutFilename,true> audioOutFileRegistrator;



#include "QFileLineEdit.hxx"
#include <CLAM/DirectoryName.hxx>
class DirectoryNameConfiguratorPlugin : public ConfiguratorPlugin
{
public:
	virtual bool accepts(const CLAM::DynamicType & object, unsigned attribute)
	{
		return object.GetTypeId(attribute) == typeid(CLAM::DirectoryName);
	}
	virtual QWidget * editorWidget(const CLAM::DynamicType & object, unsigned attribute)
	{
		CLAM::DirectoryName & value = *(CLAM::DirectoryName *)object.GetAttributeAsVoidPtr(attribute);
		QFileLineEdit * input = new QFileLineEdit;
		input->setDirMode(true);
		input->setLocation(QString::fromLocal8Bit(value.c_str()));
		input->setDialogCaption( QObject::tr("Select a directory"));
		return input;
	}
	virtual void takeFromWidget(const CLAM::DynamicType & object, unsigned attribute, QWidget * editorWidget)
	{
		QFileLineEdit * input = dynamic_cast<QFileLineEdit*>(editorWidget);
		CLAM::DirectoryName & value = *(CLAM::DirectoryName *)object.GetAttributeAsVoidPtr(attribute);
		value = input->location().toLocal8Bit().constData();
	}
};
static DirectoryNameConfiguratorPlugin dirnameOutFileRegistrator;


