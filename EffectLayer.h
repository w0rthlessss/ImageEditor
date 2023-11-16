#ifndef EFFECTLAYER_H
#define EFFECTLAYER_H

#include "Effects.h"

enum EffectNames{base, brightness, contrast, exposure, hue, saturation, lightness, gray, blur, noise};

class EffectLayer : private Effects
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
    bool operator!=(const EffectLayer& other);
    //cv::Mat GetLayer();
    ~EffectLayer() = default;
};

#endif // EFFECTLAYER_H
