#ifndef SchemaBrowser_hxx
#define SchemaBrowser_hxx

#include <QtCore/QVariant>
#include <QtGui/QPixmap>
#include <QtGui/QWidget>

#include <string>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QSplitter;
class QTreeWidget;
class QFrame;
class QSpinBox;
class QLabel;
class QTextBrowser;
class QLineEdit;
namespace CLAM_Annotator { class Schema; }

class SchemaBrowser : public QWidget
{
    Q_OBJECT

public:
    SchemaBrowser( QWidget* parent = 0, Qt::WFlags fl = 0 );
    ~SchemaBrowser();
    void setSchema(CLAM_Annotator::Schema & schema);
    void addAttribute(const std::string & scope, const std::string & name, const std::string & type);
private:
    QSplitter* splitter1;
    QTreeWidget* attributeList;
    QSplitter* splitter2;
    QFrame* attributeProperties;
    QSpinBox* minSpin;
    QSpinBox* maxSpin;
    QLineEdit* childEdit;
    QLabel* minLabel;
    QLabel* maxLabel;
    QLabel* childLabel;
    QTextBrowser* attributeDocumentation;

protected:
    QHBoxLayout* schemaBrowserLayout;
    QGridLayout* attributePropertiesLayout;

protected slots:
    virtual void languageChange();
    virtual void updateCurrentAttribute();

private:
    QPixmap scopeIcon;
    QPixmap attributeIcon;

    CLAM_Annotator::Schema * mSchema;
};

#endif // SchemaBrowser_hxx
