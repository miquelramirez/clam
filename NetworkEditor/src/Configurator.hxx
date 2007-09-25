#ifndef Configurator_hxx
#define Configurator_hxx
#include <QtGui/QDialog>
#include <QtGui/QFrame>
#include <QtGui/QPushButton>
#include <QtGui/QHBoxLayout>
#include <QtGui/QVBoxLayout>
#include <QtGui/QLabel>
#include <QtGui/QDialogButtonBox>
#include "ConfiguratorPlugin.hxx"
#include <map>

class Configurator : public QDialog
{
	Q_OBJECT
	CLAM::DynamicType * _data;
	QVBoxLayout * mLayout;
	typedef std::map<std::string, QWidget*> tWidgets;
	tWidgets mWidgets;
public:
	Configurator(CLAM::DynamicType & data, QWidget * parent = 0)
		: QDialog(parent) 
		, _data(&data)
	{
		mLayout = new QVBoxLayout(this);
		mLayout->setSpacing(3);
		mLayout->setMargin(5);
		setMinimumWidth(400);
		GenerateWidgets();
		AddButtons();
	}
private:
	QWidget * GetWidget(const char * name) {
		tWidgets::iterator found=mWidgets.find(name);
		if (found==mWidgets.end()) return NULL;
		return found->second;
	}
	void PushWidget(const char * name, QWidget * widget)
	{
		mWidgets.insert(tWidgets::value_type(name, widget));
	}

	void AddButtons()
	{
		QDialogButtonBox * buttonBox = new QDialogButtonBox(
			QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
			Qt::Horizontal, this);
		buttonBox->setCenterButtons(true);
		connect( buttonBox, SIGNAL(rejected()), this, SLOT(Discard()) );
		connect( buttonBox, SIGNAL(accepted()), this, SLOT(Ok()) );
		mLayout->addStretch();
		mLayout->addSpacing(10);
		mLayout->addWidget(buttonBox);
		mLayout->addSpacing(5);
		adjustSize();
	}
	void GenerateWidgets()
	{
		if (!_data->GetNDynamicAttributes())
		{
			mLayout->addWidget(new QLabel(tr("No parameters to configure")));
		}
		for (unsigned i=0; i<_data->GetNDynamicAttributes(); i++)
		{
			if (!_data->IsAttributeInstantiated(i)) continue;
			const char * name = _data->GetDynamicAttributeName(i);
			ConfiguratorPlugin & plugin = ConfiguratorPlugin::GetPlugin(*_data, i);
			QHBoxLayout * cell = new QHBoxLayout;
			mLayout->addLayout(cell);
			cell->addWidget(new QLabel(name));
			QWidget * editorWidget = plugin.editorWidget(*_data,i);
			cell->addWidget(editorWidget);
			PushWidget(name, editorWidget);
		}
	}
	void RecoverData()
	{
		for (unsigned i=0; i<_data->GetNDynamicAttributes(); i++)
		{
			if (!_data->IsAttributeInstantiated(i)) continue;
			ConfiguratorPlugin & plugin = ConfiguratorPlugin::GetPlugin(*_data, i);
			const char * name = _data->GetDynamicAttributeName(i);
			QWidget * editorWidget = GetWidget(name);
			plugin.takeFromWidget(*_data,i,editorWidget);
		}
	}
public slots:

	void Apply() {
		RecoverData();
	}
	void Discard() {
		reject();
	}
	void Ok() {
		Apply();
		accept();
	}
	
};

#endif//Configurator_hxx

