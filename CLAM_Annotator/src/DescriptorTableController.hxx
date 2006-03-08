#ifndef _DescriptorTableController_hxx_
#define _DescriptorTableController_hxx_

#include <string>
#include <vector>
#include <QtGui/QItemDelegate>

class QTableWidget;

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

class DescriptorTableController : public QItemDelegate
{
	Q_OBJECT
private:
	QTableWidget * mTable;
	const CLAM_Annotator::Project & mProject;
	std::string mScope;
	int mElement;
	std::vector<DescriptorTablePlugin*> mPlugins;
	CLAM::DescriptionDataPool * mData;
public:
	DescriptorTableController(QTableWidget * table, const CLAM_Annotator::Project & project);
	~DescriptorTableController();
	void refreshSchema(const std::string & scope);
	void refreshData(int element, CLAM::DescriptionDataPool * dataPool);
	void updateData(int row, CLAM::DescriptionDataPool * dataPool);

	// Delegate interface
	virtual QWidget * createEditor (QWidget * parent, const QStyleOptionViewItem & option, const QModelIndex & index) const;
	virtual void setEditorData(QWidget *editor, const QModelIndex &index) const;
	virtual void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
private:
	int descriptorIndexInTable(const std::string& name);
};
} // namespace CLAM_Annotator


#endif//_DescriptorTableController_hxx_
