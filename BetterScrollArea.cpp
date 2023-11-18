#include "BetterScrollArea.h"
#include <QApplication>

BetterScrollArea::BetterScrollArea(QWidget *parent) : QScrollArea(parent){}

void BetterScrollArea::wheelEvent(QWheelEvent *event){
    if(QApplication::keyboardModifiers() == Qt::ControlModifier){
        int delta = event->angleDelta().y() / 2;
        horizontalScrollBar()->setValue(horizontalScrollBar()->value()-delta);
    }
    else if(!QApplication::keyboardModifiers()) QScrollArea::wheelEvent(event);

    else event->ignore();
}

