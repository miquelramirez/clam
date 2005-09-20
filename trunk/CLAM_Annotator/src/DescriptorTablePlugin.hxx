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
		DescriptorTablePlugin(QTable * parent, unsigned row, const SchemaAttribute & scheme, CLAM::DescriptionDataPool & dataPool);
		virtual ~DescriptorTablePlugin();
		virtual void refreshData(int element) = 0;
		virtual void updateData(int element) = 0;
	protected:
		QTable * mTable;
		unsigned mRow;
		const std::string & mScope;
	       	const std::string & mName;
		CLAM::DescriptionDataPool & mPool;
		int mElement;
	};

	DescriptorTablePlugin * createItemController(
			QTable * parent, 
			unsigned row, 
			const SchemaAttribute & scheme, 
			CLAM::DescriptionDataPool & dataPool);

}

