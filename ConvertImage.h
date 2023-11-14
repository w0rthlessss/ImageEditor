#ifndef CONVERTIMAGE_H
#define CONVERTIMAGE_H


#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgproc/types_c.h>

#include <QImage>

class ConvertImage{
public:
    ConvertImage()=default;

    static cv::Mat QImageToCVMat(const QImage &imageToConvert);

    static QImage CVMatToQImage(const cv::Mat &imageToConvert);
};

#endif // CONVERTIMAGE_H
