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
		mTable->setColumnStretchable(1, true);
	}
	DescriptorTableController::~DescriptorTableController()
	{
		for (unsigned i = 0; i < mPlugins.size(); i++)
			delete mPlugins[i];
	}
	void DescriptorTableController::refreshSchema(const std::string & scope)
	{
		mTable->hide();
		mTable->setNumRows(0);

		for (unsigned i = 0; i < mPlugins.size(); i++)
			delete mPlugins[i];
		mPlugins.clear();

		mScope = scope;
		mElement = -1;

		CLAM_Annotator::Project::ScopeSchema attributes = mProject.GetScopeSchema(scope);
		mTable->setNumRows(attributes.size());
		std::list<SchemaAttribute>::iterator attribute = attributes.begin();
		for(unsigned row = 0 ; attribute != attributes.end(); attribute++)
		{
			std::cout << "Adding Descriptors table attribute '" 
				<< attribute->GetScope() << "::"
				<< attribute->GetName() << "' type '"
				<< attribute->GetType() << "'"
				<< std::endl;
			DescriptorTablePlugin * itemController 
				= createItemController(mTable, row, *attribute);
			if (!itemController) continue; // No plugin available for that type
			mPlugins.push_back(itemController);
			TableItem * item = new TableItem(mTable, TableItem::Never, attribute->GetName().c_str());
			mTable->setItem(row, 0, item);
			row++;
		}
		mTable->adjustColumn(0);
		if (attributes.size()) mTable->show();
	}
	void DescriptorTableController::refreshData(int element, CLAM::DescriptionDataPool * dataPool)
	{
		if (mScope=="") return;
		mElement=element;
		for (unsigned i = 0; i<mPlugins.size(); i++)
		{
			if (mElement==-1)
				mPlugins[i]->clearData();
			else
				mPlugins[i]->refreshData(mElement, *dataPool);
		}
		mTable->adjustColumn(1);
	}
	void DescriptorTableController::updateData(int row, CLAM::DescriptionDataPool * dataPool)
	{
		std::string name = mTable->text(row,0).ascii();
		const SchemaAttribute & attribute = 
			mProject.GetAttributeScheme(mScope,name);
		if (mElement==-1) return;

		DescriptorTablePlugin * itemController = mPlugins[row];
		itemController->updateData(mElement, *dataPool);
	}

	int DescriptorTableController::descriptorIndexInTable(const std::string& name)
	{
		//TODO: should find a more efficient search algorithm

		CLAM_Annotator::Project::ScopeSchema attributes = mProject.GetScopeSchema(mScope);
		std::list<SchemaAttribute>::iterator attribute = attributes.begin();
		for(int i = 0 ; attribute != attributes.end(); attribute++, i++)
		{
			if (attribute->GetName() == name) return i;
		}
		return -1;
	}

}

