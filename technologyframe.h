#ifndef TECHNOLOGYFRAME_H
#define TECHNOLOGYFRAME_H

#include <QVariantMap>

#include "clickableframe.h"
#include "net.connman.Technology.h"

namespace Ui {
class TechnologyFrame;
}

class TechnologyFrame : public ClickableFrame
{
    Q_OBJECT

public:
    explicit TechnologyFrame(const QString path, const QVariantMap &properties, QWidget *parent = 0);
    ~TechnologyFrame();

    virtual void click();

private slots:
    void onPropertyChanged(QString name, QDBusVariant value);

private:
    void update();

    Ui::TechnologyFrame *ui;
    NetConnmanTechnologyInterface technologyInterface;
    QVariantMap properties;
};

#endif // TECHNOLOGYFRAME_H
