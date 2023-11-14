#ifndef EFFECTS_H
#define EFFECTS_H

#include "ConvertImage.h"

class Effects
{

public:
    Effects() = default;

    static cv::Mat MakeNoiseMap(cv::Mat img);

    static void MakeBrightnessEffect(cv::Mat &imgAfter, cv::Mat imgBefore, int value);

    static void MakeContrastEffect(cv::Mat &imgAfter, cv::Mat imgBefore, int value);

    static void MakeExposureEffect(cv::Mat &imgAfter, cv::Mat imgBefore, int value);

    static void MakeHueEffect(cv::Mat &imgAfter, cv::Mat imgBefore, int value);

    static void MakeSaturationEffect(cv::Mat &imgAfter, cv::Mat imgBefore, int value);

    static void MakeLightnessEffect(cv::Mat &imgAfter, cv::Mat imgBefore, int value);

    static void MakeGrayscaleEffect(cv::Mat &imgAfter, cv::Mat imgBefore, int value);

    static void MakeBlurEffect(cv::Mat &imgAfter, cv::Mat imgBefore, int value);

    static void MakeNoiseEffect(cv::Mat &imgAfter, cv::Mat imgBefore, int value);
};

#endif // EFFECTS_H
