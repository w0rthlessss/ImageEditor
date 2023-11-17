#include "SelectableImage.h"
#include <QColor>
#include <QPainter>

SelectableImage::SelectableImage(QWidget *parent, QAction* selectionAction)
    : QLabel(parent), selectionAction(selectionAction)
{
    setMouseTracking(true);
}

void SelectableImage::IsTriggered(){
    isSelecting = true;
    selectionRect = QRect();
    update();
}

void SelectableImage::mousePressEvent(QMouseEvent *event){
    if(isSelecting){
        selectionStart = event->pos();
        selectionRect = QRect();
        update();
    }
}

void SelectableImage::mouseMoveEvent(QMouseEvent *event){
    if(isSelecting && event->buttons() & Qt::LeftButton){
        selectionRect = QRect(selectionStart, event->pos()).normalized();
        update();
    }
}

void SelectableImage::mouseReleaseEvent(QMouseEvent*){
    if(isSelecting){
        isSelecting = false;
        update();
        emit SelectionFinished(true);
    }
}

void SelectableImage::paintEvent(QPaintEvent*){
    if(!pixmap().isNull()){
        QPainter painter(this);
        painter.drawPixmap(rect(), pixmap());
    }
    if(isSelecting && !selectionRect.isNull()){
        QPainter painter(this);
        painter.setPen(QColor(247, 154, 254));
        painter.drawRect(selectionRect);
    }
}

QRect SelectableImage::SelectionRect(){
    return selectionRect;
}
