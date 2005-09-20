#include "DescriptorTablePlugin.hxx"

namespace CLAM_Annotator
{
	DescriptorTablePlugin::DescriptorTablePlugin(QTable * parent, unsigned row, const SchemaAttribute & scheme, CLAM::DescriptionDataPool & dataPool)
		: mTable(parent)
		, mRow(row)
		, mScope(scheme.GetScope())
		, mName(scheme.GetName())
		, mPool(dataPool)
		, mElement(-1)
	{
	}
	DescriptorTablePlugin::~DescriptorTablePlugin()
	{
	}


	class DescriptorsTableItemControllerString : public DescriptorTablePlugin
	{
	public:
		DescriptorsTableItemControllerString(QTable * parent, unsigned row, const SchemaAttribute & scheme, CLAM::DescriptionDataPool & dataPool)
			: DescriptorTablePlugin(parent, row, scheme, dataPool)
		{
		}
		void refreshData(int element)
		{
			const CLAM::Text & value = mPool.GetReadPool<CLAM::Text>(mScope,mName)[element];
			QString qvalue = QString(value.c_str());
			mTable->setItem(mRow,1, new TableItem(mTable, TableItem::WhenCurrent, qvalue));
		}
		void updateData(int element)
		{
			QString qValue = mTable->text(mRow, 1);
			const std::string & value = qValue.ascii();
			CLAM_ASSERT(mPool.GetNumberOfContexts(mScope)>element,"Fuera!!");
			mPool.GetWritePool<CLAM::Text>(mScope,mName)[element] = value;
		}
	};
	
	class DescriptorsTableItemControllerEnum : public DescriptorTablePlugin
	{
	public:
		DescriptorsTableItemControllerEnum(QTable * parent, unsigned row, const SchemaAttribute & scheme, CLAM::DescriptionDataPool & dataPool)
			: DescriptorTablePlugin(parent, row, scheme, dataPool)
			, mOptions(scheme.GetRestrictionValues())
		{
		}
		void refreshData(int element)
		{
			const CLAM_Annotator::RestrictedString& value =
				mPool.GetReadPool<RestrictedString>(mScope,mName)[element];
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
		void updateData(int element)
		{
			QString qValue = mTable->text(mRow, 1);
			const std::string & value = qValue.ascii();
			mPool.GetWritePool<RestrictedString>(mScope,mName)[element].SetString(value);
		}
	private:
		const std::list<std::string> & mOptions;
	};

	class DescriptorsTableItemControllerFloat : public DescriptorTablePlugin
	{
	public:
		DescriptorsTableItemControllerFloat(QTable * parent, unsigned row, const SchemaAttribute & scheme, CLAM::DescriptionDataPool & dataPool)
			: DescriptorTablePlugin(parent, row, scheme, dataPool)
			, mRange(scheme.GetfRange())
		{
		}
		void refreshData(int element)
		{
			float value = mPool.GetReadPool<float>(mScope,mName)[element];
			mTable->setItem(mRow,1,
				new RangeSelectionTableItem(mTable,
					TableItem::WhenCurrent,
					QString::number(value),mRange));
		}
		void updateData(int element)
		{
			QString qValue = mTable->text(mRow, 1);
			CLAM_ASSERT(mPool.GetNumberOfContexts(mScope)>element,"Fuera!!");
			mPool.GetWritePool<float>(mScope,mName)[element] = qValue.toFloat();
		}
	private:
		const Range<float> & mRange;
	};

	class DescriptorsTableItemControllerInt : public DescriptorTablePlugin
	{
	public:
		DescriptorsTableItemControllerInt(QTable * parent, unsigned row, const SchemaAttribute & scheme, CLAM::DescriptionDataPool & dataPool)
			: DescriptorTablePlugin(parent, row, scheme, dataPool)
			, mRange(scheme.GetiRange())
		{
		}
		void refreshData(int element)
		{
			int value = mPool.GetReadPool<int>(mScope,mName)[element];
			mTable->setItem(mRow,1,
				new RangeSelectionTableItem(mTable,
					TableItem::WhenCurrent,
					QString::number(value),mRange));
		}
		void updateData(int element)
		{
			QString qValue = mTable->text(mRow, 1);
			CLAM_ASSERT(mPool.GetNumberOfContexts(mScope)>element,"Fuera!!");
			mPool.GetWritePool<int>(mScope,mName)[element] = qValue.toInt();
		}
	private:
		const Range<int> & mRange;
	};



	DescriptorTablePlugin * createItemController(
			QTable * parent, 
			unsigned row, 
			const SchemaAttribute & scheme, 
			CLAM::DescriptionDataPool & dataPool)
	{
		const std::string & type = scheme.GetType();

		if (type == "String")
			return new DescriptorsTableItemControllerString(parent,row,scheme,dataPool);
		if (type == "RestrictedString")
			return new DescriptorsTableItemControllerEnum(parent,row,scheme,dataPool);
		if (type == "Float")
			return new DescriptorsTableItemControllerFloat(parent,row,scheme,dataPool);
		if (type == "Int")
			return new DescriptorsTableItemControllerInt(parent,row,scheme,dataPool);
		return 0;
	}
		
}

