#include "DescriptorTableController.hxx"
#include "Project.hxx"
#include "DescriptorTablePlugin.hxx"
#include <qtable.h>

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

		CLAM_Annotator::Project::ScopeSchema attributes = mProject.GetScopeSchema(mScope);
		mTable->setNumRows(attributes.size());
		std::list<SchemaAttribute>::iterator attribute = attributes.begin();
		for(unsigned row = 0 ; attribute != attributes.end(); attribute++)
		{
			DescriptorTablePlugin * itemController 
				= createItemController(mTable, row, *attribute);
			if (!itemController) continue; // No plugin available for that type
			mPlugins.push_back(itemController);
			row++;
		}
		mTable->setNumRows(mPlugins.size()); // Some attributes were filtered
		mTable->adjustColumn(0);
		if (attributes.size()) mTable->show();
	}
	void DescriptorTableController::refreshData(int element, CLAM::DescriptionDataPool * dataPool)
	{
		if (mScope=="") return;
		mElement=element;
		mTable->setColumnReadOnly(1,mElement==-1);
		for (unsigned i = 0; i<mPlugins.size(); i++)
			mPlugins[i]->refreshData(mElement, *dataPool);
		mTable->adjustColumn(1);
	}
	void DescriptorTableController::updateData(int row, CLAM::DescriptionDataPool * dataPool)
	{
		if (mElement==-1) return;

		DescriptorTablePlugin * itemController = mPlugins[row];
		itemController->updateData(*dataPool);
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

