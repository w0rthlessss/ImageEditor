#include "SelectableImage.h"
#include <QColor>
#include <QPainter>


SelectableImage::SelectableImage(QWidget *parent, QAction* selectionAction)
    : QLabel(parent), selectionAction(selectionAction)
{
    setMouseTracking(true);
    SelectableImage::setCursor(Qt::ArrowCursor);
    isSelecting = false;
}

void SelectableImage::IsTriggered(){
    isSelecting = sender()->property("checked").value<bool>();
    if(isSelecting)
    {
        SelectableImage::setCursor(QCursor(QPixmap(":/Icons/crossCursor")));
        selectionRect = QRect();
        update();
        return;
    }

    SelectableImage::setCursor(Qt::ArrowCursor);
}

void SelectableImage::mousePressEvent(QMouseEvent *event){
    if(isSelecting){
        selectionStart = event->pos();
        selectionRect = QRect();
        update();
        return;
    }
}

void SelectableImage::mouseMoveEvent(QMouseEvent *event){
    if(isSelecting && event->buttons() & Qt::LeftButton){
        selectionRect = QRect(selectionStart, event->pos()).normalized();
        update();
        return;
    }
}

void SelectableImage::mouseReleaseEvent(QMouseEvent *){
    if(isSelecting){
        isSelecting = false;
        SelectableImage::setCursor(Qt::ArrowCursor);
        update();
        emit SelectionFinished(true);
        return;
    }
}

void SelectableImage::paintEvent(QPaintEvent*){
    if(!pixmap().isNull()){
        QPainter painter(this);
        painter.drawPixmap(rect(), pixmap());
    }
    if(isSelecting && !selectionRect.isNull()){
        QPainter painter(this);
        painter.setPen(QColor(13, 255, 255));
        painter.drawRect(selectionRect);
    }

}

QRect SelectableImage::SelectionRect(){
    return selectionRect;
}

