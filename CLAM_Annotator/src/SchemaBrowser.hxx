#ifndef SchemaBrowser_hxx
#define SchemaBrowser_hxx

#include <qvariant.h>
#include <qpixmap.h>
#include <qwidget.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QSplitter;
class QListView;
class QListViewItem;
class QFrame;
class QSpinBox;
class QLabel;
class QTextBrowser;
namespace CLAM_Annotator { class Schema; }

class SchemaBrowser : public QWidget
{
    Q_OBJECT

public:
    SchemaBrowser( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
    ~SchemaBrowser();
    void setSchema(CLAM_Annotator::Schema & schema);
    void addAttribute(const std::string & scope, const std::string & name, const std::string & type);
private:
    QSplitter* splitter1;
    QListView* attributeList;
    QSplitter* splitter2;
    QFrame* attributeProperties;
    QSpinBox* minSpin;
    QSpinBox* maxSpin;
    QLabel* minLabel;
    QLabel* maxLabel;
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
