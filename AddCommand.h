#ifndef ADDCOMMAND_H
#define ADDCOMMAND_H

#include <QUndoCommand>
#include "EffectLayer.h"

class AddCommand : public QUndoCommand
{
private:
    cv::Mat* targetImage;
    cv::Mat imageBefore;
    cv::Mat imageAfter;
    std::vector<EffectLayer>* targetState;
    std::vector<EffectLayer> previousStates;
    std::vector<EffectLayer> currentStates;

public:

    AddCommand(cv::Mat *targetImage, cv::Mat imageBefore, cv::Mat imageAfter,
               std::vector<EffectLayer> *targetState, std::vector<EffectLayer> previousStates,
               std::vector<EffectLayer> currentStates, QString text, QUndoCommand *parent = nullptr);

    void undo() override;

    void redo() override;
};

#endif // ADDCOMMAND_H
