#include "Project.hxx"
#include "DescriptorTableController.hxx"
#include <qtable.h>
#include "ComboTableItem.hxx"

namespace CLAM_Annotator
{

	DescriptorTableController::DescriptorTableController(QTable * table, const CLAM_Annotator::Project & project)
		: mTable(table)
		, mProject(project)
		, mScope("")
		, mElement(-1)
	{
		mTable->setLeftMargin(0);
		mTable->setNumCols( 2 );
		mTable->horizontalHeader()->setLabel( 0, mTable->tr( "Descriptor" ) );
		mTable->horizontalHeader()->setLabel( 1, mTable->tr( "Value" ) );
		mTable->setNumRows( 0 );
		mTable->setNumCols( 2 );
		mTable->setSelectionMode( QTable::NoSelection );
	}
	void DescriptorTableController::refreshSchema(const std::string & scope)
	{
		mTable->hide();
		mScope = scope;
		mElement = -1;
		mTable->setNumRows(0);
		CLAM_Annotator::Project::SongScopeSchema hlds = mProject.GetScopeSchema(scope);
		mTable->setNumRows(hlds.size());
		std::list<CLAM_Annotator::SchemaAttribute>::iterator it = hlds.begin();
		for(int i = 0 ; it != hlds.end(); it++, i++)
		{
			TableItem * item = new TableItem(mTable, TableItem::Never, it->GetName().c_str());
			mTable->setItem(i, 0, item);
		}
		if (hlds.size()) mTable->show();
	}
	void DescriptorTableController::refreshData(int element, const CLAM::DescriptionDataPool * dataPool)
	{
		if (mScope=="") return;
		mElement=element;
		CLAM_Annotator::Project::SongScopeSchema hlds = mProject.GetScopeSchema(mScope);
		std::list<CLAM_Annotator::SchemaAttribute>::iterator it;
		for(it = hlds.begin() ; it != hlds.end(); it++)
		{
			const std::string & type = it->GetType();
			const std::string & name = it->GetName();
			unsigned row = descriptorIndexInTable(name);
			if (mElement==-1)
			{
				mTable->clearCell(row,1);
				continue;
			}
			else if (type == "String")
			{
				insertValue(row,
					dataPool->GetReadPool<CLAM::Text>(mScope,name)[element]
					);
			}
			else if (type == "RestrictedString")
			{
				const std::list<std::string> & options = it->GetRestrictionValues();
				insertValue(row,
					dataPool->GetReadPool<CLAM_Annotator::RestrictedString>(mScope,name)[element],
					options);
			}
			else if (type == "Float")
			{
				insertValue(row,
					dataPool->GetReadPool<float>(mScope,name)[element],
					it->GetfRange());
			}
			else if (type == "Int")
			{
				insertValue(row,
					dataPool->GetReadPool<int>(mScope,name)[element],
					it->GetiRange());
			}
		}
		mTable->adjustColumn(0);
		mTable->adjustColumn(1);
	}
	void DescriptorTableController::updateData(int row, CLAM::DescriptionDataPool * dataPool)
	{
		std::string name = mTable->text(row,0).ascii();
		const CLAM_Annotator::SchemaAttribute & hldSchemaElement = 
			mProject.GetAttributeScheme(mScope,name);

		const std::string & type = hldSchemaElement.GetType();
		QString qValue = mTable->text(row, 1);
		const std::string & value = qValue.ascii();

		if (type == "String")
		{
			dataPool->GetWritePool<CLAM::Text>(mScope,name)[mElement] = value;
		}
		if (type == "RestrictedString")
		{
			dataPool->GetWritePool<CLAM_Annotator::RestrictedString>(mScope,name)[mElement].SetString(value);
		}
		if (type == "Float")
		{
			dataPool->GetWritePool<float>(mScope,name)[mElement] = qValue.toFloat();
		}
		if (type == "Int")
		{
			dataPool->GetWritePool<int>(mScope,name)[mElement] = qValue.toInt();
		}
	}

	int DescriptorTableController::descriptorIndexInTable(const std::string& name)
	{
		//TODO: should find a more efficient search algorithm

		CLAM_Annotator::Project::SongScopeSchema hlds = mProject.GetScopeSchema(mScope);
		std::list<CLAM_Annotator::SchemaAttribute>::iterator it = hlds.begin();
		for(int i = 0 ; it != hlds.end(); it++, i++)
		{
			if (it->GetName() == name) return i;
		}
		return -1;
	}

	void DescriptorTableController::insertValue(int row, const CLAM::Text & value)
	{
		QString qvalue = QString(value.c_str());
		mTable->setItem(row,1,
			new TableItem(mTable,TableItem::WhenCurrent,qvalue));
	}

	void DescriptorTableController::insertValue(int row, 
		const CLAM_Annotator::RestrictedString& value, 
		const std::list<std::string> & options)
	{
		QString qvalue = value.GetString().c_str();
		QStringList qrestrictionStrings;
		std::list<std::string>::const_iterator it;
		for(it = options.begin();it != options.end(); it++)
		{
			qrestrictionStrings << QString(it->c_str());
		}

		std::vector<QStringList> qrestrictionStringslist;
		qrestrictionStringslist.push_back( qrestrictionStrings );
		ComboTableItem * item = new ComboTableItem(mTable,qrestrictionStringslist,false);
		item->setCurrentItem(qvalue);
		mTable->setItem(row,1,item);
	}

	void DescriptorTableController::insertValue(int row, float value, Range<float> range)
	{
		std::ostringstream s;
		s<<value;
		QString qvalue = QString(s.str().c_str());
		mTable->setItem(row,1,
			new RangeSelectionTableItem(mTable,
				TableItem::WhenCurrent,qvalue,range));
	}

	void DescriptorTableController::insertValue(int row, int value, Range<int> range)
	{
		std::ostringstream s;
		s<<value;
		QString qvalue = QString(s.str().c_str());
		mTable->setItem(row,1,
			new RangeSelectionTableItem(mTable,
				TableItem::WhenCurrent,qvalue,range));

	}

}

