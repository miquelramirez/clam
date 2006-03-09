#include "DescriptorTablePlugin.hxx"
#include "SchemaAttribute.hxx"
#include <QtGui/QTableWidget>
#include <QtGui/QLineEdit>
#include <QtGui/QComboBox>
#include <QtGui/QSpinBox>
#include <QtGui/QDoubleSpinBox>
#include <CLAM/Pool.hxx>

namespace CLAM_Annotator
{
	DescriptorTablePlugin::DescriptorTablePlugin(QTableWidget * parent, unsigned row, const SchemaAttribute & scheme)
		: mTable(parent)
		, mRow(row)
		, mScope(scheme.GetScope())
		, mName(scheme.GetName())
		, mElement(-1)
	{
		std::cout << "Creating '" << mScope << ":" << mName << "' of type " << scheme.GetType() << std::endl;
		mTable->setItem(mRow, 0, new QTableWidgetItem(mName.c_str()));
		clearData();
	}
	DescriptorTablePlugin::~DescriptorTablePlugin()
	{
	}
	void DescriptorTablePlugin::clearData()
	{
		if (mTable->item(mRow,1))
			mTable->item(mRow,1)->setText("");
	}
	void DescriptorTablePlugin::refreshData(int element, CLAM::DescriptionDataPool & dataPool)
	{
		mElement = element;
		std::cout << "Refreshing '" << mScope << ":" << mName << "' pos " << mElement << std::endl;
		if (mElement<0 || mElement>=dataPool.GetNumberOfContexts(mScope))
			clearData();
		else
			refreshData(dataPool);
	}
	QWidget * DescriptorTablePlugin::createEditor(QWidget * parent, const QStyleOptionViewItem & option)
	{
		return 0;
	}
	void DescriptorTablePlugin::takeEditorContent(QWidget * editor, CLAM::DescriptionDataPool & dataPool)
	{
	}

	void DescriptorTablePlugin::fillEditor(QWidget * editor, CLAM::DescriptionDataPool & dataPool)
	{
	}

	class DescriptorsTableItemControllerString : public DescriptorTablePlugin
	{
	public:
		DescriptorsTableItemControllerString(QTableWidget * parent, unsigned row, const SchemaAttribute & scheme)
			: DescriptorTablePlugin(parent, row, scheme)
		{
		}
		void refreshData(CLAM::DescriptionDataPool & dataPool)
		{
			const CLAM::Text & value = dataPool.GetReadPool<CLAM::Text>(mScope,mName)[mElement];
			QString qvalue = QString(value.c_str());
			mTable->setItem(mRow,1, new QTableWidgetItem(qvalue));
		}
		void updateData(CLAM::DescriptionDataPool & dataPool)
		{
			if (mElement==-1) return;
			CLAM_ASSERT(dataPool.GetNumberOfContexts(mScope)>mElement,"Fuera!!");
			QString qValue = mTable->item(mRow, 1)->text();
			const std::string & value = qValue.toStdString();
			dataPool.GetWritePool<CLAM::Text>(mScope,mName)[mElement] = value;
		}
		void fillEditor(QWidget * editor, CLAM::DescriptionDataPool & dataPool)
		{
			QLineEdit * lineEdit = static_cast<QLineEdit*>(editor);
			const CLAM::Text& value =
				dataPool.GetReadPool<CLAM::Text>(mScope,mName)[mElement];
			lineEdit->setText(value.c_str());
		}
		void takeEditorContent(QWidget * editor, CLAM::DescriptionDataPool & dataPool)
		{
			QLineEdit * lineEdit = static_cast<QLineEdit*>(editor);
			dataPool.GetWritePool<CLAM::Text>(mScope,mName)[mElement] = lineEdit->text().toStdString();
			refreshData(dataPool);
		}
	};
	
	class DescriptorsTableItemControllerEnum : public DescriptorTablePlugin
	{
	public:
		DescriptorsTableItemControllerEnum(QTableWidget * parent, unsigned row, const SchemaAttribute & scheme)
			: DescriptorTablePlugin(parent, row, scheme)
			, mOptions(scheme.GetEnumerationValues())
		{
		}
		void refreshData(CLAM::DescriptionDataPool & dataPool)
		{
			const CLAM_Annotator::Enumerated& value =
				dataPool.GetReadPool<Enumerated>(mScope,mName)[mElement];
			QString qvalue = value.GetString().c_str();
			QStringList qrestrictionStrings;

			std::list<std::string>::const_iterator it;
			for(it = mOptions.begin();it != mOptions.end(); it++)
				qrestrictionStrings << QString(it->c_str());

			std::vector<QStringList> qrestrictionStringslist;
			qrestrictionStringslist.push_back( qrestrictionStrings );
			mTable->setItem(mRow,1, new QTableWidgetItem(qvalue));
		}
		void updateData(CLAM::DescriptionDataPool & dataPool)
		{
			if (mElement==-1) return;
			QString qValue = mTable->item(mRow, 1)->text();
			const std::string & value = qValue.toStdString();
			dataPool.GetWritePool<Enumerated>(mScope,mName)[mElement].SetString(value);
		}

		virtual QWidget * createEditor(QWidget * parent, const QStyleOptionViewItem & option)
		{
			QComboBox * editor = new QComboBox(parent);
			std::list<std::string>::const_iterator it;
			for(it = mOptions.begin();it != mOptions.end(); it++)
				editor->addItem(QString(it->c_str()));
			return editor;
		}
		void fillEditor(QWidget * editor, CLAM::DescriptionDataPool & dataPool)
		{
			QComboBox * combo = static_cast<QComboBox*>(editor);
			const CLAM_Annotator::Enumerated& value =
				dataPool.GetReadPool<Enumerated>(mScope,mName)[mElement];
			combo->setCurrentIndex(combo->findText(value.GetString().c_str()));
		}
		void takeEditorContent(QWidget * editor, CLAM::DescriptionDataPool & dataPool)
		{
			QComboBox * combo = static_cast<QComboBox*>(editor);
			dataPool.GetWritePool<Enumerated>(mScope,mName)[mElement] = combo->currentText().toStdString();
			refreshData(dataPool);
		}

	private:
		const std::list<std::string> mOptions;
	};

	class DescriptorsTableItemControllerFloat : public DescriptorTablePlugin
	{
		const Range<CLAM::TData> mRange;
	public:
		DescriptorsTableItemControllerFloat(QTableWidget * parent, unsigned row, const SchemaAttribute & scheme)
			: DescriptorTablePlugin(parent, row, scheme)
			, mRange(scheme.GetfRange())
		{
		}
		void refreshData(CLAM::DescriptionDataPool & dataPool)
		{
			CLAM::TData value = dataPool.GetReadPool<CLAM::TData>(mScope,mName)[mElement];
			mTable->setItem(mRow,1, new QTableWidgetItem(QString::number(value)));
		}
		void updateData(CLAM::DescriptionDataPool & dataPool)
		{
			if (mElement==-1) return;
			CLAM_ASSERT(dataPool.GetNumberOfContexts(mScope)>mElement,"Fuera!!");
			QString qValue = mTable->item(mRow, 1)->text();
			dataPool.GetWritePool<CLAM::TData>(mScope,mName)[mElement] = qValue.toFloat();
		}
		virtual QWidget * createEditor(QWidget * parent, const QStyleOptionViewItem & option)
		{
			QDoubleSpinBox *editor = new QDoubleSpinBox(parent);
			editor->setMinimum(mRange.GetMin());
			editor->setMaximum(mRange.GetMax());
			editor->setSingleStep((editor->maximum()-editor->minimum())/100);
			editor->setDecimals(6);
			return editor;
		}
		void fillEditor(QWidget * editor, CLAM::DescriptionDataPool & dataPool)
		{
			QDoubleSpinBox * spin = static_cast<QDoubleSpinBox*>(editor);
			const CLAM::TData& value =
				dataPool.GetReadPool<CLAM::TData>(mScope,mName)[mElement];
			spin->setValue(value);
		}
		void takeEditorContent(QWidget * editor, CLAM::DescriptionDataPool & dataPool)
		{
			QDoubleSpinBox * spin = static_cast<QDoubleSpinBox*>(editor);
			dataPool.GetWritePool<CLAM::TData>(mScope,mName)[mElement] = spin->value();
			refreshData(dataPool);
		}

	};

	class DescriptorsTableItemControllerInt : public DescriptorTablePlugin
	{
		const Range<int> mRange;
	public:
		DescriptorsTableItemControllerInt(QTableWidget * parent, unsigned row, const SchemaAttribute & scheme)
			: DescriptorTablePlugin(parent, row, scheme)
			, mRange(scheme.GetiRange())
		{
		}
		void refreshData(CLAM::DescriptionDataPool & dataPool)
		{
			int value = dataPool.GetReadPool<int>(mScope,mName)[mElement];
			mTable->setItem(mRow,1, new QTableWidgetItem(QString::number(value)));
		}
		void updateData(CLAM::DescriptionDataPool & dataPool)
		{
			if (mElement==-1) return;
			CLAM_ASSERT(dataPool.GetNumberOfContexts(mScope)>mElement,"Fuera!!");
			QString qValue = mTable->item(mRow, 1)->text();
			dataPool.GetWritePool<int>(mScope,mName)[mElement] = qValue.toInt();
		}
		virtual QWidget * createEditor(QWidget * parent, const QStyleOptionViewItem & option)
		{
			QSpinBox *editor = new QSpinBox(parent);
			editor->setMinimum(mRange.GetMin());
			editor->setMaximum(mRange.GetMax());
			return editor;
		}
		void fillEditor(QWidget * editor, CLAM::DescriptionDataPool & dataPool)
		{
			QSpinBox * spin = static_cast<QSpinBox*>(editor);
			const int & value =
				dataPool.GetReadPool<int>(mScope,mName)[mElement];
			spin->setValue(value);
		}
		void takeEditorContent(QWidget * editor, CLAM::DescriptionDataPool & dataPool)
		{
			QSpinBox * spin = static_cast<QSpinBox*>(editor);
			dataPool.GetWritePool<int>(mScope,mName)[mElement] = spin->value();
			refreshData(dataPool);
		}
	};



	DescriptorTablePlugin * createItemController(
			QTableWidget * parent, 
			unsigned row, 
			const SchemaAttribute & scheme)
	{
		const std::string & type = scheme.GetType();

		if (type == "String")
			return new DescriptorsTableItemControllerString(parent,row,scheme);
		if (type == "Enumerated")
			return new DescriptorsTableItemControllerEnum(parent,row,scheme);
		if (type == "Float")
			return new DescriptorsTableItemControllerFloat(parent,row,scheme);
		if (type == "Int")
			return new DescriptorsTableItemControllerInt(parent,row,scheme);
		return 0;
	}

}

