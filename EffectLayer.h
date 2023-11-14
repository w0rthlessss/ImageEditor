#ifndef EFFECTLAYER_H
#define EFFECTLAYER_H

#include "Effects.h"

class EffectLayer
{
private:
    //cv::Mat layer;
    int effectIndex;
    int value;
    bool isVisible;
    void (*Effect)(cv::Mat&, cv::Mat, int);
public:
    EffectLayer() = default;
    EffectLayer(int index, int value, void (*effect)(cv::Mat&, cv::Mat, int));
    void SetVisible(bool);
    void SetValue(int);
    int GetIndex();
    int GetValue();
    void ApplyEffect(cv::Mat &imgAfter, cv::Mat imgBefore, int value);
    //cv::Mat GetLayer();
    ~EffectLayer() = default;
};

#endif // EFFECTLAYER_H
