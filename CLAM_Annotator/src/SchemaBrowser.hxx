#ifndef SchemaBrowser_hxx
#define SchemaBrowser_hxx

#include <qvariant.h>
#include <qpixmap.h>
#include <qwidget.h>
//Added by qt3to4:
#include <Q3VBoxLayout>
#include <Q3HBoxLayout>
#include <QLabel>
#include <Q3GridLayout>
#include <Q3Frame>

#include <string>

class Q3VBoxLayout;
class Q3HBoxLayout;
class Q3GridLayout;
class QSpacerItem;
class QSplitter;
class Q3ListView;
class Q3ListViewItem;
class Q3Frame;
class QSpinBox;
class QLabel;
class Q3TextBrowser;
class QLineEdit;
namespace CLAM_Annotator { class Schema; }

class SchemaBrowser : public QWidget
{
    Q_OBJECT

public:
    SchemaBrowser( QWidget* parent = 0, const char* name = 0, Qt::WFlags fl = 0 );
    ~SchemaBrowser();
    void setSchema(CLAM_Annotator::Schema & schema);
    void addAttribute(const std::string & scope, const std::string & name, const std::string & type);
private:
    QSplitter* splitter1;
    Q3ListView* attributeList;
    QSplitter* splitter2;
    Q3Frame* attributeProperties;
    QSpinBox* minSpin;
    QSpinBox* maxSpin;
    QLineEdit* childEdit;
    QLabel* minLabel;
    QLabel* maxLabel;
    QLabel* childLabel;
    Q3TextBrowser* attributeDocumentation;

protected:
    Q3HBoxLayout* schemaBrowserLayout;
    Q3GridLayout* attributePropertiesLayout;

protected slots:
    virtual void languageChange();
    virtual void updateCurrentAttribute();

private:
    QPixmap scopeIcon;
    QPixmap attributeIcon;

    CLAM_Annotator::Schema * mSchema;
};

#endif // SchemaBrowser_hxx
