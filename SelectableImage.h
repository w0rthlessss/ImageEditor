#ifndef SELECTABLEIMAGE_H
#define SELECTABLEIMAGE_H

#include <QLabel>
#include <QMouseEvent>
#include <QAction>
#include <QMainWindow>
#include <QTimer>

class SelectableImage : public QLabel
{
    Q_OBJECT
public:
    SelectableImage(QWidget *parent = nullptr, QAction* selectionAction = nullptr);
    QRect SelectionRect();
    ~SelectableImage() = default;

Q_SIGNALS:
    void SelectionFinished(bool flag);

protected:
    void SetScrollArea();
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent*) override;
    void paintEvent(QPaintEvent*) override;

private:
    bool isSelecting;
    QPoint selectionStart;
    QRect selectionRect;
    QAction selectionAction;


private slots:
    void IsTriggered();
};

#endif // SELECTABLEIMAGE_H
