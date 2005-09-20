#include "Project.hxx"
#include "RangeSelectionTableItem.hxx"
#include "DescriptorTableController.hxx"
#include <qtable.h>
#include "ComboTableItem.hxx"
#include "DescriptorTablePlugin.hxx"

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
		CLAM_Annotator::Project::ScopeSchema hlds = mProject.GetScopeSchema(scope);
		mTable->setNumRows(hlds.size());
		std::list<SchemaAttribute>::iterator it = hlds.begin();
		for(int i = 0 ; it != hlds.end(); it++, i++)
		{
			TableItem * item = new TableItem(mTable, TableItem::Never, it->GetName().c_str());
			mTable->setItem(i, 0, item);
		}
		mTable->adjustColumn(0);
		if (hlds.size()) mTable->show();
	}
	void DescriptorTableController::refreshData(int element, CLAM::DescriptionDataPool * dataPool)
	{
		if (mScope=="") return;
		mElement=element;
		CLAM_Annotator::Project::ScopeSchema hlds = mProject.GetScopeSchema(mScope);
		std::list<SchemaAttribute>::iterator attribute;
		for(attribute = hlds.begin() ; attribute != hlds.end(); attribute++)
		{
			const std::string & type = attribute->GetType();
			const std::string & name = attribute->GetName();
			unsigned row = descriptorIndexInTable(name);
			if (mElement==-1)
			{
				mTable->clearCell(row,1);
				continue;
			}
			DescriptorTablePlugin * itemController 
				= createItemController(mTable, row, *attribute, *dataPool);

			if (!itemController) continue;
			itemController->refreshData(element);
			delete itemController;
		}
		mTable->adjustColumn(1);
	}
	void DescriptorTableController::updateData(int row, CLAM::DescriptionDataPool * dataPool)
	{
		std::string name = mTable->text(row,0).ascii();
		const SchemaAttribute & attribute = 
			mProject.GetAttributeScheme(mScope,name);

		const std::string & type = attribute.GetType();
		QString qValue = mTable->text(row, 1);
		const std::string & value = qValue.ascii();

		DescriptorTablePlugin * itemController 
			= createItemController(mTable, row, attribute, *dataPool);
		if (!itemController) return;
		itemController->updateData(mElement);
		delete itemController;
	}

	int DescriptorTableController::descriptorIndexInTable(const std::string& name)
	{
		//TODO: should find a more efficient search algorithm

		CLAM_Annotator::Project::ScopeSchema hlds = mProject.GetScopeSchema(mScope);
		std::list<SchemaAttribute>::iterator it = hlds.begin();
		for(int i = 0 ; it != hlds.end(); it++, i++)
		{
			if (it->GetName() == name) return i;
		}
		return -1;
	}

}

