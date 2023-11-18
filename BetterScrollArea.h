#ifndef BETTERSCROLLAREA_H
#define BETTERSCROLLAREA_H

#include <QScrollArea>
#include <QWheelEvent>
#include <QMouseEvent>
#include <QScrollBar>

class BetterScrollArea : public QScrollArea
{
    Q_OBJECT
public:
    BetterScrollArea(QWidget *parent = nullptr);

protected:
    void wheelEvent(QWheelEvent *event) override;

};

#endif // BETTERSCROLLAREA_H
