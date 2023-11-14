#include "ConvertImage.h"

cv::Mat ConvertImage::QImageToCVMat(const QImage &imageToConvert){
    switch ( imageToConvert.format() )
    {
    case QImage::Format_ARGB32:
    case QImage::Format_ARGB32_Premultiplied:
    {
        cv::Mat  mat( imageToConvert.height(), imageToConvert.width(),
                    CV_8UC4,
                    const_cast<uchar*>(imageToConvert.bits()),
                    static_cast<size_t>(imageToConvert.bytesPerLine())
                    );

        return mat;
    }

    case QImage::Format_RGB32:
    {

        cv::Mat  mat( imageToConvert.height(), imageToConvert.width(),
                    CV_8UC4,
                    const_cast<uchar*>(imageToConvert.bits()),
                    static_cast<size_t>(imageToConvert.bytesPerLine())
                    );

        cv::Mat  matNoAlpha;

        cv::cvtColor( mat, matNoAlpha, cv::COLOR_BGRA2BGR );   // drop the all-white alpha channel

        return matNoAlpha;
    }

    case QImage::Format_RGB888:
    {


        QImage   swapped = imageToConvert.rgbSwapped();

        return cv::Mat( swapped.height(), swapped.width(),
                       CV_8UC3,
                       const_cast<uchar*>(swapped.bits()),
                       static_cast<size_t>(swapped.bytesPerLine())
                       ).clone();
    }

    case QImage::Format_Indexed8:
    {
        cv::Mat  mat( imageToConvert.height(), imageToConvert.width(),
                    CV_8UC1,
                    const_cast<uchar*>(imageToConvert.bits()),
                    static_cast<size_t>(imageToConvert.bytesPerLine())
                    );

        return mat;
    }

    default:
        //qWarning() << "ASM::QImageToCvMat() - QImage format not handled in switch:" << imageToConvert.format();
        throw("QImage format not handled in switch");
        break;
    }

    return cv::Mat();
}

QImage ConvertImage::CVMatToQImage(const cv::Mat &imageToConvert){
    switch ( imageToConvert.type() )
    {
    case CV_8UC4:
    {
        QImage image( imageToConvert.data,
                     imageToConvert.cols, imageToConvert.rows,
                     static_cast<int>(imageToConvert.step),
                     QImage::Format_ARGB32 );

        return image;
    }

    case CV_8UC3:
    {
        QImage image( imageToConvert.data,
                     imageToConvert.cols, imageToConvert.rows,
                     static_cast<int>(imageToConvert.step),
                     QImage::Format_RGB888 );

        return image.rgbSwapped();
    }
    case CV_8UC1:
    {

        QImage image( imageToConvert.data,
                     imageToConvert.cols, imageToConvert.rows,
                     static_cast<int>(imageToConvert.step),
                     QImage::Format_Grayscale8 );


        return image;
    }

    default:
        //qWarning() << "ASM::cvMatToQImage() - cv::Mat image type not handled in switch:" << imageToConvert.type();
        throw("cv::Mat image format not handled in switch");
        break;
    }
}
