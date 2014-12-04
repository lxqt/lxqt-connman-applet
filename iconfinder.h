#ifndef ICONFINDER_H
#define ICONFINDER_H
#include <QIcon>

class IconFinder : public QObject
{
    Q_OBJECT

public:
    static IconFinder* instance();

    QIcon& icon();

private:
    IconFinder();
    QIcon mIcon;
};



#endif // ICONFINDER_H
