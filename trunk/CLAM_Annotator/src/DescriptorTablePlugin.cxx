#include "DescriptorTablePlugin.hxx"
#include "SchemaAttribute.hxx"
#include <qtable.h>
#include "ComboTableItem.hxx"
#include "RangeSelectionTableItem.hxx"
#include "Pool.hxx"

namespace CLAM_Annotator
{
	DescriptorTablePlugin::DescriptorTablePlugin(QTable * parent, unsigned row, const SchemaAttribute & scheme)
		: mTable(parent)
		, mRow(row)
		, mScope(scheme.GetScope())
		, mName(scheme.GetName())
		, mElement(-1)
	{
		std::cout << "Creating '" << mScope << ":" << mName << "' of type " << scheme.GetType() << std::endl;
		TableItem * item = new TableItem(mTable, TableItem::Never, mName.c_str());
		mTable->setItem(mRow, 0, item);
		clearData();
	}
	DescriptorTablePlugin::~DescriptorTablePlugin()
	{
	}
	void DescriptorTablePlugin::clearData()
	{
		mTable->clearCell(mRow,1);
	}
	void DescriptorTablePlugin::refreshData(int element, CLAM::DescriptionDataPool & dataPool)
	{
		mElement = element;
		std::cout << "Refreshing '" << mScope << ":" << mName << "' pos " << mElement << std::endl;
		if (mElement==-1)
			clearData();
		else
			refreshData(dataPool);
	}

	class DescriptorsTableItemControllerString : public DescriptorTablePlugin
	{
	public:
		DescriptorsTableItemControllerString(QTable * parent, unsigned row, const SchemaAttribute & scheme)
			: DescriptorTablePlugin(parent, row, scheme)
		{
		}
		void refreshData(CLAM::DescriptionDataPool & dataPool)
		{
			const CLAM::Text & value = dataPool.GetReadPool<CLAM::Text>(mScope,mName)[mElement];
			QString qvalue = QString(value.c_str());
			mTable->setItem(mRow,1, new TableItem(mTable, TableItem::WhenCurrent, qvalue));
		}
		void updateData(CLAM::DescriptionDataPool & dataPool)
		{
			if (mElement==-1) return;
			CLAM_ASSERT(dataPool.GetNumberOfContexts(mScope)>mElement,"Fuera!!");
			QString qValue = mTable->text(mRow, 1);
			const std::string & value = qValue.ascii();
			dataPool.GetWritePool<CLAM::Text>(mScope,mName)[mElement] = value;
		}
	};
	
	class DescriptorsTableItemControllerEnum : public DescriptorTablePlugin
	{
	public:
		DescriptorsTableItemControllerEnum(QTable * parent, unsigned row, const SchemaAttribute & scheme)
			: DescriptorTablePlugin(parent, row, scheme)
			, mOptions(scheme.GetRestrictionValues())
		{
		}
		void refreshData(CLAM::DescriptionDataPool & dataPool)
		{
			const CLAM_Annotator::RestrictedString& value =
				dataPool.GetReadPool<RestrictedString>(mScope,mName)[mElement];
			QString qvalue = value.GetString().c_str();
			QStringList qrestrictionStrings;

			std::list<std::string>::const_iterator it;
			for(it = mOptions.begin();it != mOptions.end(); it++)
				qrestrictionStrings << QString(it->c_str());

			std::vector<QStringList> qrestrictionStringslist;
			qrestrictionStringslist.push_back( qrestrictionStrings );
			ComboTableItem * item = new ComboTableItem(mTable,qrestrictionStringslist,false);
			item->setCurrentItem(qvalue);
			mTable->setItem(mRow,1,item);
		}
		void updateData(CLAM::DescriptionDataPool & dataPool)
		{
			if (mElement==-1) return;
			QString qValue = mTable->text(mRow, 1);
			const std::string & value = qValue.ascii();
			dataPool.GetWritePool<RestrictedString>(mScope,mName)[mElement].SetString(value);
		}
	private:
		const std::list<std::string> mOptions;
	};

	class DescriptorsTableItemControllerFloat : public DescriptorTablePlugin
	{
	public:
		DescriptorsTableItemControllerFloat(QTable * parent, unsigned row, const SchemaAttribute & scheme)
			: DescriptorTablePlugin(parent, row, scheme)
			, mRange(scheme.GetfRange())
		{
		}
		void refreshData(CLAM::DescriptionDataPool & dataPool)
		{
			float value = dataPool.GetReadPool<float>(mScope,mName)[mElement];
			mTable->setItem(mRow,1,
				new RangeSelectionTableItem(mTable,
					TableItem::WhenCurrent,
					QString::number(value),mRange));
		}
		void updateData(CLAM::DescriptionDataPool & dataPool)
		{
			if (mElement==-1) return;
			CLAM_ASSERT(dataPool.GetNumberOfContexts(mScope)>mElement,"Fuera!!");
			QString qValue = mTable->text(mRow, 1);
			dataPool.GetWritePool<float>(mScope,mName)[mElement] = qValue.toFloat();
		}
	private:
		const Range<float> mRange;
	};

	class DescriptorsTableItemControllerInt : public DescriptorTablePlugin
	{
	public:
		DescriptorsTableItemControllerInt(QTable * parent, unsigned row, const SchemaAttribute & scheme)
			: DescriptorTablePlugin(parent, row, scheme)
			, mRange(scheme.GetiRange())
		{
		}
		void refreshData(CLAM::DescriptionDataPool & dataPool)
		{
			int value = dataPool.GetReadPool<int>(mScope,mName)[mElement];
			mTable->setItem(mRow,1,
				new RangeSelectionTableItem(mTable,
					TableItem::WhenCurrent,
					QString::number(value),mRange));
		}
		void updateData(CLAM::DescriptionDataPool & dataPool)
		{
			if (mElement==-1) return;
			CLAM_ASSERT(dataPool.GetNumberOfContexts(mScope)>mElement,"Fuera!!");
			QString qValue = mTable->text(mRow, 1);
			dataPool.GetWritePool<int>(mScope,mName)[mElement] = qValue.toInt();
		}
	private:
		const Range<int> mRange;
	};



	DescriptorTablePlugin * createItemController(
			QTable * parent, 
			unsigned row, 
			const SchemaAttribute & scheme)
	{
		const std::string & type = scheme.GetType();

		if (type == "String")
			return new DescriptorsTableItemControllerString(parent,row,scheme);
		if (type == "RestrictedString")
			return new DescriptorsTableItemControllerEnum(parent,row,scheme);
		if (type == "Float")
			return new DescriptorsTableItemControllerFloat(parent,row,scheme);
		if (type == "Int")
			return new DescriptorsTableItemControllerInt(parent,row,scheme);
		return 0;
	}

}

