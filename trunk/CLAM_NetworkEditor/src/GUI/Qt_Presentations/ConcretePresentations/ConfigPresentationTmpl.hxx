/*
 * Copyright (c) 2001-2003 MUSIC TECHNOLOGY GROUP (MTG)
 *                         UNIVERSITAT POMPEU FABRA
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#ifndef __CONFIGPRESENTATIONTMPL_HXX__
#define __CONFIGPRESENTATIONTMPL_HXX__

#include "Qt_ProcessingConfigPresentation.hxx"
#include <qwidget.h>

#include <map>
#include <string>
#include "ConfigurationVisitor.hxx"
#include "Assert.hxx"
#include "Enum.hxx"
#include "DataTypes.hxx"
#include "DynamicType.hxx"
#include "Filename.hxx"
#include "AudioFile.hxx"

#include <limits>
#include <qdialog.h>
#include <qvbox.h>
#include <qgrid.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qspinbox.h>
#include <qvalidator.h>
#include <qcombobox.h>
#include <qpushbutton.h>
#include <qfiledialog.h>

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

	ConcreteConfig mConfig;
	virtual void SetConfig( const CLAM::ProcessingConfig & );
	virtual void ConfigureProcessing();
	void GetInfo();
	void SetInfo();
	QWidget * GetWidget(const char * name);

public:
	ConfigPresentationTmpl( QWidget * parent = 0 );
	virtual ~ConfigPresentationTmpl();

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
	void AddWidget(const char *name, unsigned long *foo, T& value);
	template<typename T>
	void RetrieveValue(const char *name, unsigned long *foo, T& value);

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

	
	template<typename T>
	void AddWidget(const char *name, CLAM::Filename *foo, T& value);
	template<typename T>
	void RetrieveValue(const char *name, CLAM::Filename *foo, T& value);

	template<typename T>
	void AddWidget(const char *name, CLAM::AudioFile *foo, T& value);
	template<typename T>
	void RetrieveValue(const char *name, CLAM::AudioFile *foo, T& value);

};


template<class ConcreteConfig>
ConfigPresentationTmpl<ConcreteConfig>::ConfigPresentationTmpl( QWidget * parent )
	: Qt_ProcessingConfigPresentation( parent )
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
void ConfigPresentationTmpl<ConcreteConfig>::SetConfig( const CLAM::ProcessingConfig & cfg)
{

//	deep copy from abstract processing config to concrete
	mConfig = static_cast<const ConcreteConfig &>(cfg);


	CLAM_ASSERT(!mSetter, "Configurator: Configuration assigned twice");
	CLAM_ASSERT(!mGetter, "Configurator: Configuration assigned twice");
	typedef ConfigPresentationTmpl<ConcreteConfig> ConcreteConfigPresentation;
	mSetter = new CLAM::ConfigurationSetter<ConcreteConfig,ConcreteConfigPresentation>(&mConfig, this);
	mGetter = new CLAM::ConfigurationGetter<ConcreteConfig,ConcreteConfigPresentation>(&mConfig, this);
	CLAM_ASSERT(!mLayout, "Configurator: Configuration assigned twice");


	mLayout = mAttributeContainer;
	mLayout->setSpacing(5);
	mLayout->setMargin(5);
	mLayout->setMinimumWidth(120);
	GetInfo();

	adjustSize();

}

template<class ConcreteConfig>
void ConfigPresentationTmpl<ConcreteConfig>::ConfigureProcessing()
{
	SetInfo();
	SignalConfigureProcessing.Emit(mConfig);
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
	cell->setSpacing(5);
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
	cell->setSpacing(5);
	new QLabel(QString(name), cell);
	std::stringstream val;
	val << value << std::ends;
	QLineEdit * mInput = new QLineEdit(QString(val.str().c_str()), cell);
	mInput->setAlignment(Qt::AlignRight);
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
void ConfigPresentationTmpl<ConcreteConfig>::AddWidget(const char *name, unsigned long *foo, T& value) {
	QHBox * cell = new QHBox(mLayout);
	cell->setSpacing(5);
	new QLabel(QString(name), cell);
	std::stringstream val;
	val << value << std::ends;
	QLineEdit * mInput = new QLineEdit(QString(val.str().c_str()), cell);
	mInput->setAlignment(Qt::AlignRight);
	mInput->setValidator(new QDoubleValidator(mInput));
	mWidgets.insert(tWidgets::value_type(name, mInput));
}

template <class ConcreteConfig>
template< typename T>
void ConfigPresentationTmpl<ConcreteConfig>::RetrieveValue(const char *name, unsigned long *foo, T& value) {
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
	cell->setSpacing(5);
	new QLabel(QString(name), cell);
	QSpinBox * mInput = new QSpinBox(cell);
	mInput->setMaxValue(std::numeric_limits<T>::max());
	mInput->setMinValue(std::numeric_limits<T>::min());
//	mInput->setAlignment(Qt::AlignRight);
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
	cell->setSpacing(5);
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


template <class ConcreteConfig>
template <typename T>
void ConfigPresentationTmpl<ConcreteConfig>::AddWidget(const char *name, CLAM::Filename *foo, T& value) 
{	
	QHBox * cell = new QHBox(mLayout);
	cell->setSpacing(5);
	QLabel * label = new QLabel(QString(name), cell);
	QLineEdit * mInput = new QLineEdit(QString(value.c_str()), cell);
	mInput->setMinimumWidth(300);

	QPushButton * fileBrowserLauncher = new QPushButton("...",cell);
	fileBrowserLauncher->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
	QFileDialog * fd = new QFileDialog(this, "file dialog", FALSE );
	fd->setMode( QFileDialog::ExistingFile );

	mWidgets.insert(tWidgets::value_type(name, mInput));

	connect( fileBrowserLauncher, SIGNAL(clicked()), fd, SLOT(exec()) );
	connect( fd, SIGNAL(fileSelected( const QString & )), mInput, SLOT( setText( const QString & )));
}

template <class ConcreteConfig>
template< typename T>
void ConfigPresentationTmpl<ConcreteConfig>::RetrieveValue(const char *name, CLAM::Filename *foo, T& value) 
{	
	QLineEdit * mInput = dynamic_cast<QLineEdit*>(GetWidget(name));
	CLAM_ASSERT(mInput,"Configurator: Retrieving a value/type pair not present");
	value=mInput->text().latin1();
}

template <class ConcreteConfig>
template <typename T>
void ConfigPresentationTmpl<ConcreteConfig>::AddWidget(const char *name, CLAM::AudioFile *foo, T& value) 
{	
	QHBox * cell = new QHBox(mLayout);
	cell->setSpacing(5);
	QLabel * label = new QLabel(QString(name), cell);
	QLineEdit * mInput = new QLineEdit(QString(value.GetLocation().c_str()), cell);
	mInput->setMinimumWidth(300);

	QPushButton * fileBrowserLauncher = new QPushButton("...",cell);
	fileBrowserLauncher->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
	QFileDialog * fd = new QFileDialog(this, "file dialog", FALSE );
	fd->setMode( QFileDialog::AnyFile );

	mWidgets.insert(tWidgets::value_type(name, mInput));

	connect( fileBrowserLauncher, SIGNAL(clicked()), fd, SLOT(exec()) );
	connect( fd, SIGNAL(fileSelected( const QString & )), mInput, SLOT( setText( const QString & )));
}

template <class ConcreteConfig>
template< typename T>
void ConfigPresentationTmpl<ConcreteConfig>::RetrieveValue(const char *name, CLAM::AudioFile *foo, T& value) 
{	
	QLineEdit * mInput = dynamic_cast<QLineEdit*>(GetWidget(name));
	CLAM_ASSERT(mInput,"Configurator: Retrieving a value/type pair not present");
	value.SetLocation( mInput->text().latin1());
}

} // namespace NetworkGUI

#endif // __CONFIGPRESENTATIONTMPL_HXX__
