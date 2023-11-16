#include "EffectLayer.h"

EffectLayer::EffectLayer(int index, int value, void (*effect)(cv::Mat&, cv::Mat, int))
{
    this->effectIndex = index;
    this->value = value;
    isVisible = true;
    Effect = effect;
}
void EffectLayer::SetVisible(bool checked){
    isVisible = checked;
}

void EffectLayer::SetValue(int value){
    this->value = value;
}

int EffectLayer::GetIndex(){
    return effectIndex;
}

int EffectLayer::GetValue(){
    return value;
}

void EffectLayer::ApplyEffect(cv::Mat &imgAfter, cv::Mat imgBefore, int value){
    Effect(imgAfter, imgBefore, value);
}

bool EffectLayer::operator!=(const EffectLayer &other){
    return this->value != other.value;
}

