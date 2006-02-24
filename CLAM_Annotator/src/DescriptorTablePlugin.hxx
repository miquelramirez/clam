#ifndef _DescriptorTablePlugin_hxx_
#define _DescriptorTablePlugin_hxx_

#include <string>

class Q3Table;
namespace CLAM { class DescriptionDataPool; }
namespace CLAM_Annotator { class SchemaAttribute; }

namespace CLAM_Annotator
{
	class DescriptorTablePlugin
	{
	public:
		DescriptorTablePlugin(Q3Table * parent, unsigned row, const SchemaAttribute & scheme);
		virtual ~DescriptorTablePlugin();
		void refreshData(int element, CLAM::DescriptionDataPool & dataPool);
		virtual void refreshData(CLAM::DescriptionDataPool & dataPool) = 0;
		virtual void updateData(CLAM::DescriptionDataPool & dataPool) = 0;
		void clearData();
	protected:
		Q3Table * mTable;
		const unsigned mRow;
		const std::string mScope;
		const std::string mName;
		int mElement;
	};

	DescriptorTablePlugin * createItemController(
			Q3Table * parent, 
			unsigned row, 
			const SchemaAttribute & scheme);

}

#endif//_DescriptorTablePlugin_hxx_
