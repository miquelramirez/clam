#include <qt/qobject.h>

class YourClass : public QObject {
Q_OBJECT
public:
YourClass( QObject * parent=0, const char * name=0 )
{
}

~YourClass()
{
}

signals:

public slots:
};
