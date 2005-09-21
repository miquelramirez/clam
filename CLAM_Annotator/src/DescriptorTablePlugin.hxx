#include "Project.hxx"
#include "RangeSelectionTableItem.hxx"
#include "DescriptorTableController.hxx"
#include <qtable.h>
#include "ComboTableItem.hxx"

namespace CLAM_Annotator
{
	class DescriptorTablePlugin
	{
	public:
		DescriptorTablePlugin(QTable * parent, unsigned row, const SchemaAttribute & scheme);
		virtual ~DescriptorTablePlugin();
		void refreshData(int element, CLAM::DescriptionDataPool & dataPool);
		virtual void refreshData(CLAM::DescriptionDataPool & dataPool) = 0;
		virtual void updateData(CLAM::DescriptionDataPool & dataPool) = 0;
		void clearData();
	protected:
		QTable * mTable;
		const unsigned mRow;
		const std::string mScope;
		const std::string mName;
		int mElement;
	};

	DescriptorTablePlugin * createItemController(
			QTable * parent, 
			unsigned row, 
			const SchemaAttribute & scheme);

}

