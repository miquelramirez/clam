#include <QtDesigner/QDesignerContainerExtension>
#include <QtDesigner/QDesignerCustomWidgetInterface>
#include <QtDesigner/QDesignerCustomWidgetCollectionInterface>
#include <QtCore/QtPlugin>

class CLAMWidgets : public QObject,
	public QDesignerCustomWidgetCollectionInterface
{
	Q_OBJECT
	Q_INTERFACES(QDesignerCustomWidgetCollectionInterface)
public:
	CLAMWidgets(QObject* parent=0);
	virtual ~CLAMWidgets();
	QList<QDesignerCustomWidgetInterface*> customWidgets() const;

private:
	QList<QDesignerCustomWidgetInterface*> _widgetPlugins;
};


