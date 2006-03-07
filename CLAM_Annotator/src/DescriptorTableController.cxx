#include "DescriptorTableController.hxx"
#include "Project.hxx"
#include "DescriptorTablePlugin.hxx"
#include <QtGui/QTableWidget>

namespace CLAM_Annotator
{

	DescriptorTableController::DescriptorTableController(QTableWidget * table, const CLAM_Annotator::Project & project)
		: mTable(table)
		, mProject(project)
		, mScope("")
		, mElement(-1)
	{
//		mTable->setLeftMargin(0); //QTPORT
		mTable->setColumnCount( 2 );
		mTable->setHorizontalHeaderLabels(QStringList() 
				<< tr("Descriptor" )
				<< tr("Value")
				);
		mTable->setRowCount( 0 );
		mTable->setSelectionMode( QAbstractItemView::NoSelection );
//		mTable->setColumnStretchable(1, true); // QTPORT
	}
	DescriptorTableController::~DescriptorTableController()
	{
		for (unsigned i = 0; i < mPlugins.size(); i++)
			delete mPlugins[i];
	}
	void DescriptorTableController::refreshSchema(const std::string & scope)
	{
		mTable->hide();
		mTable->setRowCount(0);

		for (unsigned i = 0; i < mPlugins.size(); i++)
			delete mPlugins[i];
		mPlugins.clear();

		mScope = scope;
		mElement = -1;

		CLAM_Annotator::Project::ScopeSchema attributes = mProject.GetScopeSchema(mScope);
		mTable->setRowCount(attributes.size());
		std::list<SchemaAttribute>::iterator attribute = attributes.begin();
		for(unsigned row = 0 ; attribute != attributes.end(); attribute++)
		{
			DescriptorTablePlugin * itemController 
				= createItemController(mTable, row, *attribute);
			if (!itemController) continue; // No plugin available for that type
			mPlugins.push_back(itemController);
			row++;
		}
		mTable->setRowCount(mPlugins.size()); // Some attributes were filtered
//		mTable->adjustColumn(0); //QTPORT
		mTable->show();
	}
	void DescriptorTableController::refreshData(int element, CLAM::DescriptionDataPool * dataPool)
	{
		if (mScope=="") return;
		mElement=element;
//		mTable->setColumnReadOnly(1,mElement==-1); // QTPORT
		for (unsigned i = 0; i<mPlugins.size(); i++)
			mPlugins[i]->refreshData(mElement, *dataPool);
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

