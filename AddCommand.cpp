#include "AddCommand.h"

    AddCommand::AddCommand(cv::Mat *targetImage, cv::Mat imageBefore, cv::Mat imageAfter,
               std::vector<EffectLayer> *targetState, std::vector<EffectLayer> previousStates,
               std::vector<EffectLayer> currentStates, QString text, QUndoCommand *parent)
    :QUndoCommand(text, parent)
    {
        this->targetImage = targetImage;
        this->imageBefore = imageBefore;
        this->imageAfter = imageAfter;
        this->targetState = targetState;
        this->currentStates = currentStates;
        this->previousStates = previousStates;

    }

void AddCommand::undo(){
    *targetImage = imageBefore;
    *targetState = previousStates;
}

void AddCommand::redo(){
    *targetImage = imageAfter;
    *targetState = currentStates;
}
