#ifndef _DescriptorTableController_hxx_
#define _DescriptorTableController_hxx_

#include <string>
#include <list>
#include <vector>
#include "Range.hxx"

class QTable;

namespace CLAM
{
	class DescriptionDataPool;
	class Text;
}

namespace CLAM_Annotator
{
	class Project;
	class Enumerated;
	class DescriptorTablePlugin;
}

namespace CLAM_Annotator
{

class DescriptorTableController
{
	QTable * mTable;
	const CLAM_Annotator::Project & mProject;
	std::string mScope;
	int mElement;
	std::vector<DescriptorTablePlugin*> mPlugins;
public:
	DescriptorTableController(QTable * table, const CLAM_Annotator::Project & project);
	~DescriptorTableController();
	void refreshSchema(const std::string & scope);
	void refreshData(int element, CLAM::DescriptionDataPool * dataPool);
	void updateData(int row, CLAM::DescriptionDataPool * dataPool);
private:
	int descriptorIndexInTable(const std::string& name);
	void insertValue(int row, const CLAM::Text & value);
	void insertValue(int row, 
		const CLAM_Annotator::Enumerated& value, 
		const std::list<std::string> & options);
	void insertValue(int row, float value, Range<float> range);
	void insertValue(int row, int value, Range<int> range);
};
} // namespace CLAM_Annotator


#endif//_DescriptorTableController_hxx_
