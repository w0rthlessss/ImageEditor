#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QMainWindow>
#include<QPushButton>
#include <QLabel>
#include <QScrollArea>
#include <QScrollBar>
#include <QUndoStack>
#include <vector>
#include "EffectLayer.h"
//#include "ConvertImage.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


class MainWindow : public QMainWindow
{
    Q_OBJECT

public slots:
    //Main options
    void OpenFile();
    bool SaveImage();
    void SetPropertiesPage();
    void SetDefaultValues();
    void ZoomIn();
    void ZoomOut();
    void UndoAction();
    void RedoAction();
    void CheckDifferences();

    //Apply effects
    void ShowGrayscaleEffect();
    void ShowBrightnessEffect();
    void ShowContrastEffect();
    void ShowExposureEffect();
    void ShowHueEffect();
    void ShowSaturationEffect();
    void ShowLightnessEffect();
    void ShowBlurEffect();
    void ShowNoiseEffect();

    //Rotation and flipping
    void RotateClockwise();
    void RotateCounterClockwise();
    void RotateUpsideDown();
    void FlipVertical();
    void FlipHorizontal();
    void Crop();

public:
    MainWindow(QWidget *parent = nullptr);

    ~MainWindow();

    void AddScale();
    void SetConnections();
    void EnableInterface(bool);
    void SetupEffectLayers();

    void DisplayImage(QImage);
    void UpdateImage();
    void ScaleImage(double);
    void UpdateScaleLabel();
    void UpdateResolutionLabel();
    void UpdateSliderValues();



private:
    void closeEvent(QCloseEvent *event);
    QLabel* scaleLabel=nullptr;
    QLabel* resolutionLabel = nullptr;
    Ui::MainWindow *ui;
    bool isSaved;
    QUndoStack* undoStack = nullptr;
    cv::Mat imgForEditing, imgForDisplay, originalImage, noiseMat;
    QImage image;
    Effects effect;
    std::vector<EffectLayer> previousState;
    std::vector<EffectLayer> effectLayers;
    double scaleValue = 1.0;
};
#endif // MAINWINDOW_H
