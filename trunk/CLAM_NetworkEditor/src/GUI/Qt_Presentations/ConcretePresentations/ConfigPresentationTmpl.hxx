
#ifndef __CONFIGPRESENTATIONTMPL_HXX__
#define __CONFIGPRESENTATIONTMPL_HXX__

#include "Qt_ProcessingConfigPresentation.hxx"
#include <qwidget.h>

#include <map>
#include<string>
#include "ConfigurationVisitor.hxx"
#include "Assert.hxx"
#include "Enum.hxx"
#include "DataTypes.hxx"
#include "DynamicType.hxx"

#include <qdialog.h>
#include <qvbox.h>
#include <qgrid.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qspinbox.h>
#include <qvalidator.h>
#include <qcombobox.h>
#include <qpushbutton.h>



namespace CLAM
{
	class CLAM::ProcessingConfig;
}

namespace NetworkGUI
{

template<class ConcreteConfig>
class ConfigPresentationTmpl : public Qt_ProcessingConfigPresentation
{
	typedef std::map<std::string, QWidget*> tWidgets;
protected:


	
	QVBox * mLayout;
	CLAM::ConfigurationVisitor * mGetter;
	CLAM::ConfigurationVisitor * mSetter;
	tWidgets mWidgets;




	ConcreteConfig * mConfig;
	virtual void OnNewConfig( CLAM::ProcessingConfig* );
	virtual void ApplyChangesToConfig();
	void GetInfo();
	void SetInfo();
	QWidget * GetWidget(const char * name);

public:
	ConfigPresentationTmpl( QWidget * parent = 0 );
	virtual ~ConfigPresentationTmpl();
	virtual void Show();
	virtual void Hide();	

	template<typename T>
	void AddWidget(const char *name, void *foo, T& value);
	template<typename T>
	void RetrieveValue(const char *name, void *foo, T& value);

	template<typename T>
	void AddWidget(const char *name, std::string *foo, T& value);
	template<typename T>
	void RetrieveValue(const char *name, std::string *foo, T& value);


	template<typename T>
	void AddWidget(const char *name, CLAM::TData *foo, T& value);
	template<typename T>
	void RetrieveValue(const char *name, CLAM::TData *foo, T& value);

	template<typename T>
	void AddWidget(const char *name, CLAM::TSize *foo, T& value);
	template<typename T>
	void RetrieveValue(const char *name, CLAM::TSize *foo, T& value);

	template<typename T>
	void AddWidget(const char *name, bool *foo, T& value);
	template<typename T>
	void RetrieveValue(const char *name, bool *foo, T& value);

	template<typename T>
	void AddWidget(const char *name, CLAM::Enum *foo, T& value);
	template<typename T>
	void RetrieveValue(const char *name, CLAM::Enum *foo, T& value);
};


template<class ConcreteConfig>
ConfigPresentationTmpl<ConcreteConfig>::ConfigPresentationTmpl( QWidget * parent )
	: Qt_ProcessingConfigPresentation( parent ),
	  mConfig(0)
{
	mSetter = 0;
	mGetter = 0;
	mLayout = 0;
}

template<class ConcreteConfig>
ConfigPresentationTmpl<ConcreteConfig>::~ConfigPresentationTmpl()
{
	if (mSetter) 
		delete mSetter;
	if (mGetter) 
		delete mGetter;
}

template<class ConcreteConfig>
void ConfigPresentationTmpl<ConcreteConfig>::Show()
{
	show();
}

template<class ConcreteConfig>
void ConfigPresentationTmpl<ConcreteConfig>::Hide()
{
	hide();
}

template<class ConcreteConfig>
void ConfigPresentationTmpl<ConcreteConfig>::OnNewConfig( CLAM::ProcessingConfig* cfg)
{
	mConfig = (ConcreteConfig*)cfg;


	CLAM_ASSERT(!mSetter, "Configurator: Configuration assigned twice");
	CLAM_ASSERT(!mGetter, "Configurator: Configuration assigned twice");
	typedef ConfigPresentationTmpl<ConcreteConfig> ConcreteConfigPresentation;
	mSetter = new CLAM::ConfigurationSetter<ConcreteConfig,ConcreteConfigPresentation>(mConfig, this);
	mGetter = new CLAM::ConfigurationGetter<ConcreteConfig,ConcreteConfigPresentation>(mConfig, this);
	CLAM_ASSERT(!mLayout, "Configurator: Configuration assigned twice");


	mLayout = new QVBox(this);
	mLayout->setSpacing(3);
	mLayout->setMargin(5);
	mLayout->setMinimumWidth(120);
	GetInfo();

	QFrame * frame = new QFrame(mLayout);
	frame->setMinimumHeight(10);
	mLayout->adjustSize();	


	

}

template<class ConcreteConfig>
void ConfigPresentationTmpl<ConcreteConfig>::ApplyChangesToConfig()
{
	SetInfo();
	ApplyConfig.Emit(mConfig);
}

template<class ConcreteConfig>
void ConfigPresentationTmpl<ConcreteConfig>::GetInfo()
{
	CLAM_ASSERT(mGetter,"Configurator: No config to set");
	mGetter->VisitConfig();
}

template<class ConcreteConfig>
void ConfigPresentationTmpl<ConcreteConfig>::SetInfo()
{
	CLAM_ASSERT(mSetter,"Configurator: No config to set");
	mSetter->VisitConfig();
}

template<class ConcreteConfig>
QWidget * ConfigPresentationTmpl<ConcreteConfig>::GetWidget(const char * name)
{
	tWidgets::iterator found=mWidgets.find(name);
	if (found==mWidgets.end()) return 0;
	return found->second;
}

template<class ConcreteConfig>
template< typename T>
void ConfigPresentationTmpl<ConcreteConfig>::AddWidget(const char *name, void *foo, T& value) {
}

template<class ConcreteConfig>
template< typename T>
void ConfigPresentationTmpl<ConcreteConfig>::RetrieveValue(const char *name, void *foo, T& value) {
}

template <class ConcreteConfig>
template< typename T>
void ConfigPresentationTmpl<ConcreteConfig>::AddWidget(const char *name, std::string *foo, T& value) {
	QHBox * cell = new QHBox(mLayout);
	new QLabel(QString(name), cell);
	QLineEdit * mInput = new QLineEdit(QString(value.c_str()), cell);
	mInput->resize( 120, 25 );
	mWidgets.insert(tWidgets::value_type(name, mInput));
}

template <class ConcreteConfig>
template< typename T>
void ConfigPresentationTmpl<ConcreteConfig>::RetrieveValue(const char *name, std::string *foo, T& value) {
	QLineEdit * mInput = dynamic_cast<QLineEdit*>(GetWidget(name));
	CLAM_ASSERT(mInput,"Configurator: Retrieving a value/type pair not present");
	value=mInput->text().latin1();
}

template <class ConcreteConfig>
template< typename T>
void ConfigPresentationTmpl<ConcreteConfig>::AddWidget(const char *name, CLAM::TData *foo, T& value) {
	QHBox * cell = new QHBox(mLayout);
	new QLabel(QString(name), cell);
	std::stringstream val;
	val << value << std::ends;
	QLineEdit * mInput = new QLineEdit(QString(val.str().c_str()), cell);
	mInput->setValidator(new QDoubleValidator(mInput));
	mWidgets.insert(tWidgets::value_type(name, mInput));
}

template <class ConcreteConfig>
template< typename T>
void ConfigPresentationTmpl<ConcreteConfig>::RetrieveValue(const char *name, CLAM::TData *foo, T& value) {
	QLineEdit * mInput = dynamic_cast<QLineEdit*>(GetWidget(name));
	CLAM_ASSERT(mInput,"Configurator: Retrieving a value/type pair not present");
	const char * readValue=mInput->text().latin1();
	std::stringstream s(readValue);
	s >> value;
}

template <class ConcreteConfig>
template< typename T>
void ConfigPresentationTmpl<ConcreteConfig>::AddWidget(const char *name, CLAM::TSize *foo, T& value) {
	QHBox * cell = new QHBox(mLayout);
	new QLabel(QString(name), cell);
	QSpinBox * mInput = new QSpinBox(cell);
	QIntValidator * validator = new QIntValidator(mInput);
	validator->setBottom(0);
	mInput->setValidator(validator);
	mInput->setValue(value);
	mWidgets.insert(tWidgets::value_type(name, mInput));
}

template <class ConcreteConfig>
template< typename T>
void ConfigPresentationTmpl<ConcreteConfig>::RetrieveValue(const char *name, CLAM::TSize *foo, T& value) {
	QSpinBox * mInput = dynamic_cast<QSpinBox*>(GetWidget(name));
	CLAM_ASSERT(mInput,"Configurator: Retrieving a value/type pair not present");
	value = mInput->value();
}

template <class ConcreteConfig>
template< typename T>
void ConfigPresentationTmpl<ConcreteConfig>::AddWidget(const char *name, bool *foo, T& value) {
	QPushButton * mInput = new QPushButton(name, mLayout);
	mInput->setToggleButton(true);
	mInput->setOn(value);
	mInput->setAutoDefault(false);
	mWidgets.insert(tWidgets::value_type(name, mInput));
}

template <class ConcreteConfig>
template< typename T>
void ConfigPresentationTmpl<ConcreteConfig>::RetrieveValue(const char *name, bool *foo, T& value) {
	QPushButton * mInput = dynamic_cast<QPushButton*>(GetWidget(name));
	CLAM_ASSERT(mInput,"Configurator: Retrieving a value/type pair not present");
	value=mInput->isOn();
}

template <class ConcreteConfig>
template< typename T>
void ConfigPresentationTmpl<ConcreteConfig>::AddWidget(const char *name, CLAM::Enum *foo, T& value) {
	QHBox * cell = new QHBox(mLayout);
	new QLabel(QString(name), cell);
	QComboBox * mChoice = new QComboBox(false, cell); // false editable
	
	const CLAM::Enum::tEnumValue * mapping = value.GetSymbolMap();
	for (unsigned i = 0; mapping[i].name; i++) {
		mChoice->insertItem( mapping[i].name );
		if (mapping[i].value==value.GetValue()) mChoice->setCurrentItem(i);
	}
	mWidgets.insert(tWidgets::value_type(name, mChoice));
}
template <class ConcreteConfig>
template< typename T>
void ConfigPresentationTmpl<ConcreteConfig>::RetrieveValue(const char *name, CLAM::Enum *foo, T& value) {
	QComboBox * mInput = dynamic_cast<QComboBox*>(GetWidget(name));
	CLAM_ASSERT(mInput,"Configurator: Retrieving a value/type pair not present");
	const CLAM::Enum::tEnumValue * mapping = value.GetSymbolMap();
	CLAM_BEGIN_CHECK
		int i;
	for (i = 0; mapping[i].name; i++);
	CLAM_ASSERT(mInput->currentItem()<i, "Configurator: Choice value out of range");
	CLAM_END_CHECK
		value=mapping[mInput->currentItem()].value;
}

/*	
  template <class ConcreteConfig>
	void AddWidget(const char *name, DynamicType *foo, T&value) {
		QHBox * cell = new QHBox(mLayout);
		new QLabel(QString(name), cell);
		QPushButton * mInput = new QPushButton("Details...", cell);
		mInput->setAutoDefault(false);
		//QTConfigurator * subConfigurator = new QTConfigurator(this;)
		ConfigPresentationTmpl * subConfigurator = new ConfigPresentationTmpl(this);

		subConfigurator->SetConfig(value);
		connect( mInput, SIGNAL(clicked()), subConfigurator, SLOT(show()) );
		mWidgets.insert(tWidgets::value_type(name, mInput));
	}
*/
/*
	template <class ConcreteConfig>
	void RetrieveValue(const char *name, DynamicType *foo, T&value) {
	}	
*/



} // namespace NetworkGUI

#endif // __CONFIGPRESENTATIONTMPL_HXX__
