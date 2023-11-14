#ifndef ADDCOMMAND_H
#define ADDCOMMAND_H

#include <QUndoCommand>
#include "mainwindow.h"

class AddCommand : public QUndoCommand
{
private:

    cv::Mat image;
    cv::Mat imageBefore;
    MainWindow *window = nullptr;

public:

    AddCommand(cv::Mat &image, cv::Mat &imageBefore, MainWindow *window, QUndoCommand *parent = nullptr);

    void undo() override;

    void redo() override;
};

#endif // ADDCOMMAND_H
