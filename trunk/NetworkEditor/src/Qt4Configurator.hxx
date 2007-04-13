/*
 * Copyright (c) 2001-2004 MUSIC TECHNOLOGY GROUP (MTG)
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

#ifndef Qt4Configurator_hxx
#define Qt4Configurator_hxx

#include <CLAM/ConfigurationVisitor.hxx>
#include <map>
#include <string>

#include <CLAM/Assert.hxx>
#include <CLAM/Enum.hxx>
#include <CLAM/BPF.hxx>
#include <CLAM/DataTypes.hxx>
#include <CLAM/DynamicType.hxx>
#include <CLAM/ConfigurableFile.hxx>
#include <CLAM/Filename.hxx>
// #include <CLAM/QtEnvelopeEditor.hxx> // TODO: Still not Qt4

#include <QtGui/QDialog>
#include <QtGui/QPushButton>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QSpinBox>
#include <QtGui/QComboBox>
#include <QtGui/QFileDialog>
#include <QtGui/QFrame>
#include <QtGui/QVBoxLayout>
#include <QtGui/QHBoxLayout>

#include "QFileLineEdit.hxx"


namespace CLAM{
	/**
	 * A generic DynamicType edition dialog for Qt.
	 * @ingroup Configurators
	 * @see DynamicType, FLTKConfigurator
	 */
	class Qt4Configurator : public QDialog {
		Q_OBJECT
		typedef QDialog super;
		typedef std::map<std::string, QWidget*> tWidgets;
	public:
		Qt4Configurator(QWidget * parent = NULL) 
			: super(parent)
			
		{
			mSetter = 0;
			mGetter = 0;
			mLayout = 0;
		}

		virtual ~Qt4Configurator();
		template <class Config>
		void SetConfig(Config & config) {
			CLAM_ASSERT(!mSetter, "Configurator: Configuration assigned twice");
			CLAM_ASSERT(!mGetter, "Configurator: Configuration assigned twice");
			mSetter = new ConfigurationSetter<Config,Qt4Configurator>(&config, this);
			mGetter = new ConfigurationGetter<Config,Qt4Configurator>(&config, this);

			CLAM_ASSERT(!mLayout, "Configurator: Configuration assigned twice");
			mLayout = new QVBoxLayout(this);
			mLayout->setSpacing(3);
			mLayout->setMargin(5);
			//mLayout->setSizePolicy(QSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding));
			setMinimumWidth(400);

			GetInfo();
			AddButtons();
		}
	protected:
		void AddButtons()
		{
			QFrame * frame = new QFrame(this);
			mLayout->addWidget(frame);
			frame->setMinimumHeight(10);

			QHBoxLayout * buttons = new QHBoxLayout;
			mLayout->addLayout(buttons);
			buttons->setMargin(5);
			buttons->setSpacing(3);
			
			QPushButton * discardButton = new QPushButton("Discard", this);
			discardButton->setAutoDefault(false);
			buttons->addWidget(discardButton);
			connect( discardButton, SIGNAL(clicked()), this, SLOT(Discard()) );

			QPushButton * okButton = new QPushButton("Ok", this);
			okButton->setDefault(true);
			buttons->addWidget(okButton);
			connect( okButton, SIGNAL(clicked()), this, SLOT(Ok()) );

			buttons->setStretchFactor(discardButton,2);
			buttons->setStretchFactor(okButton,2);

			adjustSize();
		}
	private:
		void GetInfo() {
			CLAM_ASSERT(mGetter,"Configurator: No config to set");
			mGetter->VisitConfig();
		}
		void SetInfo() {
			CLAM_ASSERT(mSetter,"Configurator: No config to set");
			mSetter->VisitConfig();
		}

		QWidget * GetWidget(const char * name) {
			tWidgets::iterator found=mWidgets.find(name);
			if (found==mWidgets.end()) return NULL;
			return found->second;
		}
		void PushWidget(const char * name, QWidget * widget)
		{
			mWidgets.insert(tWidgets::value_type(name, widget));
		}

	public:

		/** Default implementation, do nothing */
		template <typename T>
		void AddWidget(const char *name, void *foo, T& value) {
			QHBoxLayout * cell = new QHBoxLayout;
			mLayout->addLayout(cell);
			cell->addWidget(new QLabel(name));
			QLabel * mInput = new QLabel("Non editable attribute");
			cell->addWidget(mInput);
			PushWidget(name,mInput);
		}
		/** Default implementation, do nothing */
		template <typename T>
		void RetrieveValue(const char *name, void *foo, T& value) {
		}

		template <typename T>
		void AddWidget(const char *name, std::string *foo, T& value) {
			QLineEdit * mInput = new QLineEdit(QString(value.c_str()));

			QHBoxLayout * cell = new QHBoxLayout;
			mLayout->addLayout(cell);
			cell->addWidget(new QLabel(name));
			cell->addWidget(mInput);
			PushWidget(name, mInput);
		}
		template <typename T>
		void RetrieveValue(const char *name, std::string *foo, T& value) {
			QLineEdit * mInput = dynamic_cast<QLineEdit*>(GetWidget(name));
			CLAM_ASSERT(mInput,"Configurator: Retrieving a value/type pair not present");
			value=mInput->text().toStdString();
		}

		template <typename T>
		void AddWidget(const char *name, float *foo, T& value) {
			QDoubleSpinBox * mInput = new QDoubleSpinBox;
			mInput->setRange(-1e16,1e16);
			mInput->setValue(value);

			QHBoxLayout * cell = new QHBoxLayout;
			mLayout->addLayout(cell);
			cell->addWidget(new QLabel(name));
			cell->addWidget(mInput);
			PushWidget(name, mInput);
		}
		template <typename T>
		void RetrieveValue(const char *name, float *foo, T& value) {
			QDoubleSpinBox * mInput = dynamic_cast<QDoubleSpinBox*>(GetWidget(name));
			CLAM_ASSERT(mInput,"Configurator: Retrieving a value/type pair not present");
			value = mInput->value();
		}

		template <typename T>
		void AddWidget(const char *name, double *foo, T& value) {
			QDoubleSpinBox * mInput = new QDoubleSpinBox;
			mInput->setRange(-1e16,1e16);
			mInput->setValue(value);

			QHBoxLayout * cell = new QHBoxLayout;
			mLayout->addLayout(cell);
			cell->addWidget(new QLabel(name));
			cell->addWidget(mInput);
			PushWidget(name, mInput);
		}
		template <typename T>
		void RetrieveValue(const char *name, double *foo, T& value) {
			QDoubleSpinBox * mInput = dynamic_cast<QDoubleSpinBox*>(GetWidget(name));
			CLAM_ASSERT(mInput,"Configurator: Retrieving a value/type pair not present");
			value = mInput->value();
		}

		template <typename T>
		void AddWidget(const char *name, TSize *foo, T& value) {
			QSpinBox * mInput = new QSpinBox;
			mInput->setRange(0,0xffffff);
			mInput->setValue(value);

			QHBoxLayout * cell = new QHBoxLayout;
			mLayout->addLayout(cell);
			cell->addWidget(new QLabel(name));
			cell->addWidget(mInput);
			PushWidget(name, mInput);
		}
		template <typename T>
		void RetrieveValue(const char *name, TSize *foo, T& value) {
			QSpinBox * mInput = dynamic_cast<QSpinBox*>(GetWidget(name));
			CLAM_ASSERT(mInput,"Configurator: Retrieving a value/type pair not present");
			value = mInput->value();
		}

		template <typename T>
		void AddWidget(const char *name, unsigned *foo, T& value) {
			QSpinBox * mInput = new QSpinBox;
			mInput->setRange(0,0xffffff);
			mInput->setValue(value);

			QHBoxLayout * cell = new QHBoxLayout;
			mLayout->addLayout(cell);
			cell->addWidget(new QLabel(name));
			cell->addWidget(mInput);
			PushWidget(name, mInput);
		}
		template <typename T>
		void RetrieveValue(const char *name, unsigned *foo, T& value) {
			QSpinBox * mInput = dynamic_cast<QSpinBox*>(GetWidget(name));
			CLAM_ASSERT(mInput,"Configurator: Retrieving a value/type pair not present");
			value = mInput->value();
		}

		template <typename T>
		void AddWidget(const char *name, bool *foo, T& value) {
			QPushButton * mInput = new QPushButton(name, this);
			mLayout->addWidget(mInput);
			mInput->setCheckable(true);
			mInput->setChecked(value);
			mInput->setAutoDefault(false);
			PushWidget(name, mInput);
		}
		template <typename T>
		void RetrieveValue(const char *name, bool *foo, T& value) {
			QPushButton * mInput = dynamic_cast<QPushButton*>(GetWidget(name));
			CLAM_ASSERT(mInput,"Configurator: Retrieving a value/type pair not present");
			value=mInput->isChecked();
		}

		template <typename T>
		void AddWidget(const char *name, Enum *foo, T& value) {
			QComboBox * mInput = new QComboBox(/*editable*/false);

			const Enum::tEnumValue * mapping = value.GetSymbolMap();
			for (unsigned i = 0; mapping[i].name; i++) {
				mInput->addItem( mapping[i].name );
				if (mapping[i].value==value.GetValue()) mInput->setCurrentIndex(i);
			}
			QHBoxLayout * cell = new QHBoxLayout;
			mLayout->addLayout(cell);
			cell->addWidget(new QLabel(name));
			cell->addWidget(mInput);
			PushWidget(name, mInput);
		}
		template <typename T>
		void RetrieveValue(const char *name, Enum *foo, T& value) {
			QComboBox * mInput = dynamic_cast<QComboBox*>(GetWidget(name));
			CLAM_ASSERT(mInput,"Configurator: Retrieving a value/type pair not present");
			const Enum::tEnumValue * mapping = value.GetSymbolMap();
			CLAM_BEGIN_CHECK
				int i;
				for (i = 0; mapping[i].name; i++);
				CLAM_ASSERT(mInput->currentIndex()<i, "Configurator: Choice value out of range");
			CLAM_END_CHECK
			value=mapping[mInput->currentIndex()].value;
		}

		template <typename T>
		void AddWidget(const char *name, DynamicType *foo, T&value) {
			QPushButton * mInput = new QPushButton("Details...");
			mInput->setAutoDefault(false);
			Qt4Configurator * subConfigurator = new Qt4Configurator(this);
			subConfigurator->setWindowTitle(windowTitle()+"."+name);
			subConfigurator->SetConfig(value);
			connect( mInput, SIGNAL(clicked()), subConfigurator, SLOT(show()) );

			QHBoxLayout * cell = new QHBoxLayout;
			mLayout->addLayout(cell);
			cell->addWidget(new QLabel(name));
			cell->addWidget(mInput);
			PushWidget(name, mInput);
		}
		template <typename T>
		void RetrieveValue(const char *name, DynamicType *foo, T&value) {
		}
/*
		// TODO: The QtEnvelopeEditor must be ported
		template <typename T>
		void AddWidget(const char *name, BPF *foo, T& value) {
			QtEnvelopeEditor* mInput = new QtEnvelopeEditor;
			// Those ones should be BPF properties
			mInput->BoundMinX( 0 );
			mInput->BoundMaxX( 22050 );
			mInput->BoundMinY( 0 );
			mInput->BoundMaxY( 1000 );
			mInput->SetValue(value);
			mInput->setMinimumSize( QSize( 350, 350) );

			QHBoxLayout * cell = new QHBoxLayout;
			mLayout->addLayout(cell);
			cell->addWidget(new QLabel(name));
			cell->addWidget(mInput);
			PushWidget(name, mInput);
		}

		template <typename T>
		void RetrieveValue(const char *name, BPF *foo, T& value) {
			QtEnvelopeEditor * mInput = dynamic_cast<QtEnvelopeEditor*>(GetWidget(name));
			CLAM_ASSERT(mInput,"Configurator: Retrieving a value/type pair not present");
			value = mInput->GetValue();
		}
*/
		template <typename T>
		void AddWidget(const char *name, CLAM::Filename *foo, T& value) {

			QLineEdit * mInput = new QLineEdit(value.c_str());
			mInput->setMinimumWidth(300);

			QPushButton * fileBrowserLauncher = new QPushButton("...");
			fileBrowserLauncher->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
			QFileDialog * fd = new QFileDialog(this, "file dialog", FALSE );
			fd->setFileMode( QFileDialog::ExistingFile );
			connect( fileBrowserLauncher, SIGNAL(clicked()), fd, SLOT(exec()) );
			connect( fd, SIGNAL(currentChanged( const QString & )), mInput, SLOT( setText( const QString & )));
			fd->selectFile(value.c_str());
			std::cout << "Filename: selectFile : " << value.c_str() << std::endl;

			QHBoxLayout * cell = new QHBoxLayout;
			mLayout->addLayout(cell);
			cell->addWidget(new QLabel(name));
			cell->addWidget(mInput);
			cell->addWidget(fileBrowserLauncher);
			cell->setSpacing(5);
			PushWidget(name, mInput);
		}

		template <typename T>
		void RetrieveValue(const char *name, CLAM::Filename *foo, T& value) {
			QLineEdit * mInput = dynamic_cast<QLineEdit*>(GetWidget(name));
			CLAM_ASSERT(mInput,"Configurator: Retrieving a value/type pair not present");
			value = mInput->text().toStdString();
		}

		QString filterString(const char *group,
				const CLAM::FileFormatFilterList &filters,
				bool inWrite)
		{
			if (!group[0])
				group = "visible";
			if (filters.empty())
				return tr("All %1 files (* *.*)").arg(group);
			QString qtfilter, allfilter;
			char *separator = "";
			CLAM::FileFormatFilterList::const_iterator it;
			for (it = filters.begin(); it != filters.end(); it++)
			{
				qtfilter += QString("%1%2 (%3)")
					.arg(separator)
					.arg(it->first.c_str())
					.arg(it->second.c_str())
					;
				separator = ";;";
				allfilter += (it->second+" ").c_str();
			}
			QString allfiles(tr("All %1 files (%2)")
					.arg(group)
					.arg(allfilter) );
			if (inWrite)
				allfiles = qtfilter + separator + allfiles;
			return allfiles;
		}

		template <typename T>
		void AddWidget(const char *name, CLAM::ConfigurableFile *foo, T& value)
		{
			const char *groupStr = value.GetGroupName();
			bool writeMode = value.GetWriteMode();
			QString filters(filterString(groupStr,
				  value.GetFormatFilterList(), writeMode) );
			
			QFileLineEdit * mInput = new QFileLineEdit(this);
			mInput->setWriteMode(writeMode);
			mInput->setFilters(filters);
			mInput->setLocation(value.GetLocation().c_str());

			const char *modeStr = writeMode ? "output" : "input";
			const char *sep = groupStr[0] ? " " : "";
			mInput->setDialogCaption(
				tr("Select an %1%2%3 file")
					.arg(modeStr)
					.arg(sep)
					.arg(groupStr) );

			QHBoxLayout * cell = new QHBoxLayout;
			mLayout->addLayout(cell);
			cell->addWidget(new QLabel(name));
			cell->addWidget(mInput);
			cell->setSpacing(5);
			PushWidget(name, mInput);
		}

		template <typename T>
		void RetrieveValue(const char *name, CLAM::ConfigurableFile *foo, T& value) {
			QFileLineEdit * mInput = dynamic_cast<QFileLineEdit*>(GetWidget(name));
			CLAM_ASSERT(mInput,"Configurator: Retrieving a value/type pair not present");
			value.SetLocation(mInput->location().toStdString());
		}
	public slots:

		void Apply() {
			SetInfo();
		}
		void Discard() {
			reject();
		}
		void Ok() {
			Apply();
			accept();
		}
	public:
	
		void show() {
			super::show();
		}

	private:
		QVBoxLayout * mLayout;
		ConfigurationVisitor * mGetter;
		ConfigurationVisitor * mSetter;
		tWidgets mWidgets;

	};
}
#endif//Qt4Configurator_hxx
