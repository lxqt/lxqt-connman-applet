#ifndef CLICKABLEFRAME_H
#define CLICKABLEFRAME_H

#include <QFrame>
#include <QDebug>
#include <QMouseEvent>

class ClickableFrame : public QFrame
{
    Q_OBJECT

public:
    ClickableFrame(QWidget* parent = 0) : QFrame(parent) {}
    virtual void click() = 0;

signals:
    void entered();
    void left();
    void pressed();
    void released();

protected:
    virtual void enterEvent(QEvent* event) { emit entered(); }
    virtual void leaveEvent(QEvent* event) { emit left(); }
    virtual void mousePressEvent(QMouseEvent *event) { emit pressed(); }
    virtual void mouseReleaseEvent(QMouseEvent *event) { emit released(); }
};

#endif // CLICKABLEFRAME_H
