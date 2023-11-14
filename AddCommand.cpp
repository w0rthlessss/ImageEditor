#include "AddCommand.h"

AddCommand::AddCommand(cv::Mat &image, cv::Mat &imageBefore, MainWindow *window, QUndoCommand *parent)
                       : QUndoCommand(parent), window(window)
{
    this->image = image;
    this->imageBefore = imageBefore;
    window->DisplayImage(ConvertImage::CVMatToQImage(image));
}

void AddCommand::undo(){
    window->DisplayImage(ConvertImage::CVMatToQImage(imageBefore));
}

void AddCommand::redo(){
    window->DisplayImage(ConvertImage::CVMatToQImage(image));
}
