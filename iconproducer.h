#ifndef ICONPRODUCER_H
#define ICONPRODUCER_H
#include <QIcon>

class IconFinder : public QObject
{
    Q_OBJECT

public:
    static IconFinder* instance();

    QIcon& wired_connected() { return mWired_connected; }
    QIcon& wired_disconnected() { return mWired_disconnected; }
    QIcon& wireless(int strength);

private slots:
    void onIconThemeChanged();

private:
    IconFinder();
    ~IconFinder();
    void initWirelessIcons();

    QIcon mWired_connected;
    QIcon mWired_disconnected;
    QList<QIcon> mWireless;

};



#endif // ICONPRODUCER_H
