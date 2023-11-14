#include "Effects.h"

void Effects::MakeBrightnessEffect(cv::Mat &imgAfter, cv::Mat imgBefore, int value){

    cv::Mat temp;
    cv::cvtColor(imgBefore, temp, cv::COLOR_BGR2RGB);
    imgAfter = temp;
    imgAfter = temp + cv::Scalar::all(value);
    cv::cvtColor(imgAfter, imgAfter, cv::COLOR_RGB2BGR);
}

void Effects::MakeContrastEffect(cv::Mat &imgAfter, cv::Mat imgBefore, int value){

    double beta = 1 + 0.01 * value;

    cv::Mat temp = imgBefore.clone();
    cv::Mat updatedImage = imgAfter.clone();

    for( int y = 0; y < imgBefore.rows; y++ ) {
        for( int x = 0; x < imgBefore.cols; x++ ) {
            for( int c = 0; c < imgBefore.channels(); c++ ) {
                updatedImage.at<cv::Vec3b>(y,x)[c] =
                    cv::saturate_cast<uchar>(temp.at<cv::Vec3b>(y,x)[c] * beta );
            }
        }
    }

    imgAfter = updatedImage;
}

void Effects::MakeExposureEffect(cv::Mat &imgAfter, cv::Mat imgBefore, int value){
    //double beta = 1 + 0.01 * value;
    MakeBrightnessEffect(imgAfter, imgBefore, value);
    MakeContrastEffect(imgAfter, imgAfter, value);
}

void Effects::MakeHueEffect(cv::Mat &imgAfter, cv::Mat imgBefore, int value){

    cv::Mat temp = imgBefore.clone();

    cv::cvtColor(temp, temp, cv::COLOR_BGR2HSV);
    for( int y = 0; y < temp.rows; y++ ) {
        for( int x = 0; x < temp.cols; x++ ) {
            int newValue = (temp.at<cv::Vec3b>(y,x)[0] + value + 180) % 180;
            temp.at<cv::Vec3b>(y,x)[0] = newValue;
        }
    }
    cv::cvtColor(temp, temp, cv::COLOR_HSV2BGR);
    imgAfter = temp;

}

void Effects::MakeSaturationEffect(cv::Mat &imgAfter, cv::Mat imgBefore, int value){

    cv::Mat temp = imgBefore.clone();
    double shift = 1 + 0.01 * value;
    cv::cvtColor(temp, temp, cv::COLOR_BGR2HSV);

    for( int y = 0; y < temp.rows; y++ ) {
        for( int x = 0; x < temp.cols; x++ ) {
            temp.at<cv::Vec3b>(y,x)[1] = cv::saturate_cast<uchar>(temp.at<cv::Vec3b>(y,x)[1] * shift);
        }
    }
    cv::cvtColor(temp, temp, cv::COLOR_HSV2BGR);

    imgAfter = temp;

}

void Effects::MakeLightnessEffect(cv::Mat &imgAfter, cv::Mat imgBefore, int value){

    cv::Mat temp = imgBefore.clone();

    cv::cvtColor(temp, temp, cv::COLOR_BGR2HSV);
    for( int y = 0; y < temp.rows; y++ ) {
        for( int x = 0; x < temp.cols; x++ ) {
            temp.at<cv::Vec3b>(y,x)[2] = cv::saturate_cast<uchar>(temp.at<cv::Vec3b>(y,x)[2] + value);
        }
    }
    cv::cvtColor(temp, temp, cv::COLOR_HSV2BGR);

    imgAfter = temp;

}

void Effects::MakeGrayscaleEffect(cv::Mat &imgAfter, cv::Mat imgBefore, int value){
    cv::Mat updatedImage{};

    cv::cvtColor(imgBefore, updatedImage, cv::COLOR_BGR2GRAY);

    imgAfter = updatedImage;

    cv::cvtColor(imgAfter, imgAfter, cv::COLOR_GRAY2BGR);
}

void Effects::MakeBlurEffect(cv::Mat &imgAfter, cv::Mat imgBefore, int value){
    int kernel = 3 + 18 * value / 50;
    if(kernel % 2 == 0) kernel++;
    cv::Mat temp = imgBefore.clone();

    cv::GaussianBlur(temp, temp, cv::Size(kernel, kernel), 10, 10);
    imgAfter = temp;

}

void Effects::MakeNoiseEffect(cv::Mat &imgAfter, cv::Mat imgBefore, int value){
    std::srand(static_cast<unsigned int>(std::time(NULL)));
    cv::Mat temp = imgBefore.clone();


    cv::Mat img_16SC;
    cv::Mat noise = cv::Mat(imgBefore.size(), CV_16SC3);
    cv::randn(noise, cv::Scalar::all(0.0), cv::Scalar::all(static_cast<double>(value)));

    imgBefore.convertTo(img_16SC, CV_16SC3);
    cv::addWeighted(img_16SC, 1.0, noise, 1.0, 0.0, img_16SC);

    img_16SC.convertTo(temp, imgBefore.type());

    imgAfter = temp;

}


