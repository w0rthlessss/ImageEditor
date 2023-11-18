#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QImageWriter>
#include <QMessageBox>
#include <QIcon>
#include <QStandardPaths>
#include "SelectableImage.h"
#include "AddCommand.h"



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    SetConnections();
    ui->FilterProperties->setCurrentIndex(EffectNames::base);
    AddScale();
    EnableInterface(false);
    isSaved = true;
    undoStack = new QUndoStack(this);
}

void MainWindow::SetPropertiesPage(){
    ui->FilterProperties->setCurrentIndex(ui->FilterList->currentRow()+1);
}

void MainWindow::EnableInterface(bool enable){
    ui->FilterProperties->setEnabled(enable);
    ui->actionCrop->setEnabled(enable);
    ui->actionZoomIn->setEnabled(enable);
    ui->actionZoomOut->setEnabled(enable);
    ui->actionUndo->setEnabled(enable);
    ui->actionRedo->setEnabled(enable);
    ui->actionFlip_Horizontal->setEnabled(enable);
    ui->actionFlip_Vertical->setEnabled(enable);
    ui->actionRotate_180->setEnabled(enable);
    ui->actionRotate_90_clockwise->setEnabled(enable);
    ui->actionRotate_90_counterclockwise->setEnabled(enable);
    ui->actionSave->setEnabled(enable);
}

void MainWindow::ZoomIn(){
    if(!image.isNull())
    {
        if(scaleValue<=4.8)
            ScaleImage(0.2);
    }
}

void MainWindow::ZoomOut(){
    if(!image.isNull())
    {
        if(scaleValue>0.21)
            ScaleImage(-0.2);
    }
}

void MainWindow::ScaleImage(double scaleParam){
    scaleValue+=scaleParam;

    image = ConvertImage::CVMatToQImage(imgForDisplay);

    QImage scaledImage = image.scaled(image.width() * scaleValue, image.height() * scaleValue);

    UpdateScaleLabel();

    DisplayImage(scaledImage);
}

void MainWindow::UpdateScaleLabel(){
    QString scaleString = "Scale: " + QString::number(scaleValue*100) + "%";
    scaleLabel->setText(scaleString);
}

void MainWindow::UpdateResolutionLabel(){
    QString resString = QString::number(image.width()) + "px x " + QString::number(image.height()) + "px";
    resolutionLabel->setText(resString);
}

void MainWindow::AddScale(){
    QWidget* scaleWidget = new QWidget(this);
    QLayout *layout = new QHBoxLayout(scaleWidget);
    scaleLabel = new QLabel;
    scaleLabel->setStyleSheet("QLabel{color: white;padding: 5px;font: 700 8pt \"Arial\";}");
    resolutionLabel = new QLabel;
    resolutionLabel->setStyleSheet("QLabel{color: white;padding: 5px;font: 700 8pt \"Arial\";}");

    layout->addWidget(scaleLabel);   layout->addWidget(resolutionLabel);

    QWidget *spacer = new QWidget(this);
    spacer->setVisible(true);
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);


    ui->toolBar->addWidget(spacer);
    ui->toolBar->addWidget(scaleWidget);
}

void MainWindow::SetDefaultValues(){
    QWidget* currentPage = ui->FilterProperties->currentWidget();

    QList<QSpinBox*> intValues = currentPage->findChildren<QSpinBox*>();
    QList<QSlider*> sliders = currentPage->findChildren<QSlider*>();
    for(auto slider : sliders) slider->setValue(0);
    for(auto spinbox : intValues) spinbox->setValue(0);
    int currentEffectIndex = ui->FilterProperties->currentIndex();

    if(currentEffectIndex==EffectNames::gray) ui->setGrayscaleBtn->setEnabled(true);



    for(auto it = effectLayers.begin(); it != effectLayers.end(); it++){
        if(it->GetIndex() == currentEffectIndex) it->SetValue(0);
    }

    undoStack->push(new AddCommand(&imgForEditing, &effectLayers, previousState, effectLayers, &noiseMat, "Set default values"));

    previousState = effectLayers;

    UpdateImage();

    if(scaleValue != 1.0) ScaleImage(0.0);
    else DisplayImage(ConvertImage::CVMatToQImage(imgForDisplay));

}

void MainWindow::SetConnections(){
    //Open, close and save
    connect(ui->actionOpen, SIGNAL(triggered(bool)), this, SLOT(OpenFile()));
    connect(ui->actionQuit, SIGNAL(triggered(bool)), this, SLOT(ExitApplication()));
    connect(ui->actionSave, SIGNAL(triggered(bool)), this, SLOT(SaveImage()));

    //Brightness
    connect(ui->brightnessSlider, &QSlider::valueChanged, ui->brightnessIntValue, &QSpinBox::setValue);
    connect(ui->brightnessSlider, &QSlider::sliderReleased, this, &MainWindow::CheckDifferences);
    connect(ui->brightnessSlider, SIGNAL(valueChanged(int)), this, SLOT(ShowBrightnessEffect()));

    //Contrast
    connect(ui->contrastSlider, &QSlider::valueChanged, ui->contrastIntValue, &QSpinBox::setValue);
    connect(ui->contrastSlider, &QSlider::sliderReleased, this, &MainWindow::CheckDifferences);
    connect(ui->contrastSlider, SIGNAL(valueChanged(int)), this, SLOT(ShowContrastEffect()));

    //Blur
    connect(ui->blurSlider, &QSlider::valueChanged, ui->blurIntValue, &QSpinBox::setValue);
    connect(ui->blurSlider, &QSlider::sliderReleased, this, &MainWindow::CheckDifferences);
    connect(ui->blurSlider, SIGNAL(valueChanged(int)), this, SLOT(ShowBlurEffect()));

    //Noise
    connect(ui->noiseSlider, &QSlider::valueChanged, ui->noiseIntValue, &QSpinBox::setValue);
    connect(ui->noiseSlider, &QSlider::sliderReleased, this, &MainWindow::CheckDifferences);
    connect(ui->noiseSlider, SIGNAL(valueChanged(int)), this, SLOT(ShowNoiseEffect()));

    //Hue
    connect(ui->hueSlider, &QSlider::valueChanged, ui->hueIntValue, &QSpinBox::setValue);
    connect(ui->hueSlider, &QSlider::sliderReleased, this, &MainWindow::CheckDifferences);
    connect(ui->hueSlider, SIGNAL(valueChanged(int)), this, SLOT(ShowHueEffect()));

    //Saturation
    connect(ui->satSlider, &QSlider::valueChanged, ui->satIntValue, &QSpinBox::setValue);
    connect(ui->satSlider, &QSlider::sliderReleased, this, &MainWindow::CheckDifferences);
    connect(ui->satSlider, SIGNAL(valueChanged(int)), this, SLOT(ShowSaturationEffect()));

    //Lightness
    connect(ui->lightSlider, &QSlider::valueChanged, ui->lightIntValue, &QSpinBox::setValue);
    connect(ui->lightSlider, &QSlider::sliderReleased, this, &MainWindow::CheckDifferences);
    connect(ui->lightSlider, SIGNAL(valueChanged(int)), this, SLOT(ShowLightnessEffect()));

    //Grayscale
    connect(ui->setGrayscaleBtn, SIGNAL(clicked(bool)), this, SLOT(ShowGrayscaleEffect()));

    //Exposure
    connect(ui->exposureSlider, &QSlider::valueChanged, ui->exposureIntValue, &QSpinBox::setValue);
    connect(ui->exposureSlider, &QSlider::sliderReleased, this, &MainWindow::CheckDifferences);
    connect(ui->exposureSlider, SIGNAL(valueChanged(int)), this, SLOT(ShowExposureEffect()));

    //Change pages
    connect(ui->FilterList, SIGNAL(currentRowChanged(int)), this, SLOT(SetPropertiesPage()));

    //Defaults
    connect(ui->setDefaultsBlur, SIGNAL(clicked(bool)), this, SLOT(SetDefaultValues()));
    connect(ui->setDefaultsCon, SIGNAL(clicked(bool)), this, SLOT(SetDefaultValues()));
    connect(ui->setDefaultsBr, SIGNAL(clicked(bool)), this, SLOT(SetDefaultValues()));
    connect(ui->setDefaultsExp, SIGNAL(clicked(bool)), this, SLOT(SetDefaultValues()));
    connect(ui->setDefaultsNoise, SIGNAL(clicked(bool)), this, SLOT(SetDefaultValues()));
    connect(ui->setDefaultsHue, SIGNAL(clicked(bool)), this, SLOT(SetDefaultValues()));
    connect(ui->setDefaultsSat, SIGNAL(clicked(bool)), this, SLOT(SetDefaultValues()));
    connect(ui->setDefaultsLight, SIGNAL(clicked(bool)), this, SLOT(SetDefaultValues()));
    connect(ui->setDefaultsGray, SIGNAL(clicked(bool)), this, SLOT(SetDefaultValues()));

    //Rotating and flipping
    connect(ui->actionRotate_90_clockwise, SIGNAL(triggered(bool)), this, SLOT(RotateClockwise()));
    connect(ui->actionRotate_90_counterclockwise, SIGNAL(triggered(bool)), this, SLOT(RotateCounterClockwise()));
    connect(ui->actionRotate_180, SIGNAL(triggered(bool)), this, SLOT(RotateUpsideDown()));
    connect(ui->actionFlip_Horizontal, SIGNAL(triggered(bool)), this, SLOT(FlipHorizontal()));
    connect(ui->actionFlip_Vertical, SIGNAL(triggered(bool)), this, SLOT(FlipVertical()));

    //Zoom
    connect(ui->actionZoomIn, SIGNAL(triggered(bool)), this, SLOT(ZoomIn()));
    connect(ui->actionZoomOut, SIGNAL(triggered(bool)), this, SLOT(ZoomOut()));

    //Undo and redo
    connect(ui->actionUndo, SIGNAL(triggered(bool)), this, SLOT(UndoAction()));
    connect(ui->actionRedo, SIGNAL(triggered(bool)), this, SLOT(RedoAction()));

    //Crop
    connect(ui->actionCrop, SIGNAL(triggered(bool)), ui->image, SLOT(IsTriggered()));
    connect(ui->image, SIGNAL(SelectionFinished(bool)), this, SLOT(Crop()));

}

void MainWindow::SetupEffectLayers(){

    void (*effect)(cv::Mat&, cv::Mat, int) = Effects::MakeBrightnessEffect;
    effectLayers.push_back(EffectLayer(EffectNames::brightness, 0, effect));

    effect = Effects::MakeContrastEffect;
    effectLayers.push_back(EffectLayer(EffectNames::contrast, 0, effect));

    effect = Effects::MakeExposureEffect;
    effectLayers.push_back(EffectLayer(EffectNames::exposure, 0, effect));

    effect = Effects::MakeHueEffect;
    effectLayers.push_back(EffectLayer(EffectNames::hue, 0, effect));

    effect = Effects::MakeSaturationEffect;
    effectLayers.push_back(EffectLayer(EffectNames::saturation, 0, effect));

    effect = Effects::MakeLightnessEffect;
    effectLayers.push_back(EffectLayer(EffectNames::lightness, 0, effect));

    effect = Effects::MakeGrayscaleEffect;
    effectLayers.push_back(EffectLayer(EffectNames::gray, 0, effect));

    effect = Effects::MakeBlurEffect;
    effectLayers.push_back(EffectLayer(EffectNames::blur, 0, effect));

    effect = Effects::MakeNoiseEffect;
    effectLayers.push_back(EffectLayer(EffectNames::noise, 0, effect));

}

void MainWindow::OpenFile(){
    const QStringList picturesLocations = QStandardPaths::standardLocations(QStandardPaths::PicturesLocation);
    QString filepath = QFileDialog::getOpenFileName(this, tr("Open File"),
        picturesLocations.isEmpty() ? QDir::currentPath() : picturesLocations.last(),
        tr("All files (*.*);;JPEG (*.jpg *.jpeg);;PNG (*.png)" ));
    if(!filepath.isEmpty()){
        image.load(filepath);
        UpdateResolutionLabel();
        UpdateScaleLabel();
        EnableInterface(true);

        effectLayers.clear();

        originalImage = ConvertImage::QImageToCVMat(image);
        imgForEditing = originalImage;
        imgForDisplay = originalImage;
        noiseMat = Effects::MakeNoiseMap(imgForDisplay);

        for(int curPage = 1; curPage < ui->FilterProperties->count(); curPage++){
            ui->FilterProperties->setCurrentIndex(curPage);
            SetDefaultValues();
        }

        Effects::MakeNoiseMap(originalImage);
        SetupEffectLayers();
        previousState = effectLayers;
        undoStack->clear();
        ui->actionCrop->setChecked(false);
        ui->FilterProperties->setCurrentIndex(0);
        ui->FilterList->setCurrentRow(-1);
        scaleValue = 1.0;
        isSaved = true;
        DisplayImage(image);
    }
}

bool MainWindow::SaveImage(){
    if(image.isNull()) return false;
    const QStringList picturesLocations = QStandardPaths::standardLocations(QStandardPaths::PicturesLocation);
    QString filepath = QFileDialog::getOpenFileName(this, tr("Open File"),
        picturesLocations.isEmpty() ? QDir::currentPath() : picturesLocations.last(),
        tr("All files (*.*);;JPEG (*.jpg *.jpeg);;PNG (*.png)" ));

    if(!filepath.isEmpty()){
        UpdateImage();
        image = ConvertImage::CVMatToQImage(imgForDisplay);
        QImageWriter writer(filepath);
        if(!writer.write(image)){
            QMessageBox msg(QMessageBox::Critical, MainWindow::windowTitle(),tr("Unable to save image!"));
            msg.exec();
            return false;
        }
        QMessageBox msg(QMessageBox::Information, MainWindow::windowTitle(), tr("Image was successfully saved!"));
        msg.exec();
        isSaved = true;
        return true;
    }
    return false;
}

void MainWindow::ExitApplication(){
    if(!isSaved){
        QMessageBox::StandardButton reply = QMessageBox::question(this, MainWindow::windowTitle(),
                                                                  tr("Image was not saved!\nDo you want to save it?"), QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
        if(reply == QMessageBox::Yes) if(!SaveImage()) return;
        if(reply == QMessageBox::Cancel) return;
    }
    close();

}

void MainWindow::closeEvent(QCloseEvent* event){
    ExitApplication();
    event->ignore();
}

void MainWindow::CheckDifferences(){
    for(size_t i = 0; i<effectLayers.size(); i++){
        if(effectLayers[i] != previousState[i]){

            undoStack->push(new AddCommand(&imgForEditing, &effectLayers, previousState, effectLayers, &noiseMat, "Effect value changed"));

            previousState = effectLayers;
            break;
        }
    }
}

void MainWindow::UpdateSliderValues(){
    for(auto it = effectLayers.begin(); it != effectLayers.end(); it++){
        if(it->GetIndex() != EffectNames::gray)
        {
            QWidget* currentPage = ui->FilterProperties->widget(it->GetIndex());
            QSlider* slider = currentPage->findChild<QSlider*>();
            slider->setValue(it->GetValue());
        }
        else ui->setGrayscaleBtn->setEnabled(it->GetValue());

    }
}

void MainWindow::UndoAction(){

    if(undoStack->count()){
        undoStack->undo();
        UpdateImage();
        UpdateSliderValues();
    }

    if(scaleValue != 1.0) ScaleImage(0.0);
    else DisplayImage(ConvertImage::CVMatToQImage(imgForDisplay));
}

void MainWindow::RedoAction(){
    if(undoStack->count()){
        undoStack->redo();
        UpdateImage();
        UpdateSliderValues();
    }
    if(scaleValue != 1.0) ScaleImage(0.0);
    else DisplayImage(ConvertImage::CVMatToQImage(imgForDisplay));
}

void MainWindow::DisplayImage(QImage newImage){

    ui->image->setPixmap(QPixmap::fromImage(newImage));
}

void MainWindow::UpdateImage(){
    cv::Mat imgAfter = imgForEditing.clone();
    for(auto it = effectLayers.begin(); it != effectLayers.end(); it++){
        if(it->GetValue()){
            if(it->GetIndex()!=EffectNames::noise){
                it->ApplyEffect(imgAfter, imgAfter, it->GetValue());
            }
            else it->ApplyEffect(imgAfter, noiseMat, it->GetValue());

        }
    }
    imgForDisplay = imgAfter;
    isSaved = false;
}

void MainWindow::ShowGrayscaleEffect(){
    ui->setGrayscaleBtn->setEnabled(false);

    for(auto it = effectLayers.begin(); it != effectLayers.end(); it++){
        if(it->GetIndex() == EffectNames::gray){
            it->SetValue(1);
            break;
        }
    }
    UpdateImage();

    if(scaleValue != 1.0) ScaleImage(0.0);
    else DisplayImage(ConvertImage::CVMatToQImage(imgForDisplay));
}

void MainWindow::ShowBrightnessEffect(){
    for(auto it = effectLayers.begin(); it != effectLayers.end(); it++){
        if(it->GetIndex() == EffectNames::brightness){
            it->SetValue(ui->brightnessIntValue->value());
            break;
        }
    }
    UpdateImage();

    if(scaleValue != 1.0) ScaleImage(0.0);
    else DisplayImage(ConvertImage::CVMatToQImage(imgForDisplay));
}

void MainWindow::ShowContrastEffect(){
    for(auto it = effectLayers.begin(); it != effectLayers.end(); it++){
        if(it->GetIndex() == EffectNames::contrast){
            it->SetValue(ui->contrastIntValue->value());
            break;
        }
    }
    UpdateImage();

    if(scaleValue != 1.0) ScaleImage(0.0);
    else DisplayImage(ConvertImage::CVMatToQImage(imgForDisplay));
}

void MainWindow::ShowExposureEffect(){
    for(auto it = effectLayers.begin(); it != effectLayers.end(); it++){
        if(it->GetIndex() == EffectNames::exposure){
            it->SetValue(ui->exposureIntValue->value());
            break;
        }
    }
    UpdateImage();

    if(scaleValue != 1.0) ScaleImage(0.0);
    else DisplayImage(ConvertImage::CVMatToQImage(imgForDisplay));
}

void MainWindow::ShowHueEffect(){
    for(auto it = effectLayers.begin(); it != effectLayers.end(); it++){
        if(it->GetIndex() == EffectNames::hue){
            it->SetValue(ui->hueIntValue->value());
            break;
        }
    }
    UpdateImage();

    if(scaleValue != 1.0) ScaleImage(0.0);
    else DisplayImage(ConvertImage::CVMatToQImage(imgForDisplay));
}

void MainWindow::ShowSaturationEffect(){
    for(auto it = effectLayers.begin(); it != effectLayers.end(); it++){
        if(it->GetIndex() == EffectNames::saturation){
            it->SetValue(ui->satIntValue->value());
            break;
        }
    }
    UpdateImage();

    if(scaleValue != 1.0) ScaleImage(0.0);
    else DisplayImage(ConvertImage::CVMatToQImage(imgForDisplay));
}

void MainWindow::ShowLightnessEffect(){
    for(auto it = effectLayers.begin(); it != effectLayers.end(); it++){
        if(it->GetIndex() == EffectNames::lightness){
            it->SetValue(ui->lightIntValue->value());
            break;
        }
    }
    UpdateImage();

    if(scaleValue != 1.0) ScaleImage(0.0);
    else DisplayImage(ConvertImage::CVMatToQImage(imgForDisplay));
}

void MainWindow::ShowBlurEffect(){
    for(auto it = effectLayers.begin(); it != effectLayers.end(); it++){
        if(it->GetIndex() == EffectNames::blur){
            it->SetValue(ui->blurIntValue->value());
            break;
        }
    }
    UpdateImage();

    if(scaleValue != 1.0) ScaleImage(0.0);
    else DisplayImage(ConvertImage::CVMatToQImage(imgForDisplay));
}

void MainWindow::ShowNoiseEffect(){
    for(auto it = effectLayers.begin(); it != effectLayers.end(); it++){
        if(it->GetIndex() == EffectNames::noise){
            it->SetValue(ui->noiseIntValue->value());
            break;
        }
    }
    UpdateImage();

    if(scaleValue != 1.0) ScaleImage(0.0);
    else DisplayImage(ConvertImage::CVMatToQImage(imgForDisplay));
}

void MainWindow::RotateClockwise(){
    cv::Mat imgBefore = imgForEditing;

    cv::rotate(imgForEditing, imgForEditing, cv::ROTATE_90_CLOCKWISE);

    undoStack->push(new AddCommand(&imgForEditing, imgBefore, imgForEditing, &effectLayers, &noiseMat, "Rotate 90"));

    UpdateImage();

    if(scaleValue != 1.0) ScaleImage(0.0);
    else DisplayImage(ConvertImage::CVMatToQImage(imgForDisplay));
}

void MainWindow::RotateCounterClockwise(){
    cv::Mat imgBefore = imgForEditing;

    cv::rotate(imgForEditing, imgForEditing, cv::ROTATE_90_COUNTERCLOCKWISE);

    undoStack->push(new AddCommand(&imgForEditing, imgBefore, imgForEditing, &effectLayers, &noiseMat, "Rotate -90"));

    UpdateImage();

    if(scaleValue != 1.0) ScaleImage(0.0);
    else DisplayImage(ConvertImage::CVMatToQImage(imgForDisplay));
}

void MainWindow::RotateUpsideDown(){
    cv::Mat imgBefore = imgForEditing;

    cv::rotate(imgForEditing, imgForEditing, cv::ROTATE_180);

    undoStack->push(new AddCommand(&imgForEditing, imgBefore, imgForEditing, &effectLayers, &noiseMat, "Rotate 180"));

    UpdateImage();

    if(scaleValue != 1.0) ScaleImage(0.0);
    else DisplayImage(ConvertImage::CVMatToQImage(imgForDisplay));
}

void MainWindow::FlipHorizontal(){
    cv::Mat imgBefore = imgForEditing;

    cv::flip(imgForEditing, imgForEditing, 1);

    undoStack->push(new AddCommand(&imgForEditing, imgBefore, imgForEditing, &effectLayers, &noiseMat, "Flip Horizontal"));

    UpdateImage();

    if(scaleValue != 1.0) ScaleImage(0.0);
    else DisplayImage(ConvertImage::CVMatToQImage(imgForDisplay));
}

void MainWindow::FlipVertical(){
    cv::Mat imgBefore = imgForEditing;

    cv::flip(imgForEditing, imgForEditing, 0);

    undoStack->push(new AddCommand(&imgForEditing, imgBefore, imgForEditing, &effectLayers, &noiseMat, "Flip Vertical"));

    UpdateImage();

    if(scaleValue != 1.0) ScaleImage(0.0);
    else DisplayImage(ConvertImage::CVMatToQImage(imgForDisplay));
}

void MainWindow::Crop(){
    ui->actionCrop->setChecked(false);
    QRect area = ui->image->SelectionRect();
    int x = area.x() / scaleValue;
    int y = area.y() / scaleValue;
    int width = area.width() / scaleValue;
    int height = area.height() / scaleValue;

    x = std::max(0, std::min(x, imgForEditing.cols-1));
    y = std::max(0, std::min(y, imgForEditing.rows-1));
    width = std::min(width, imgForEditing.cols - x);
    height = std::min(height, imgForEditing.rows - y);

    cv::Mat cropped = imgForEditing(cv::Rect(x, y, width, height)).clone();
    cv::Mat croppedNoise = noiseMat(cv::Rect(x, y, width, height)).clone();

    undoStack->push(new AddCommand(&imgForEditing, imgForEditing, cropped, &effectLayers, effectLayers, effectLayers, &noiseMat, noiseMat, croppedNoise, "Cropped" ));

    imgForEditing = cropped.clone();
    noiseMat = croppedNoise.clone();
    UpdateImage();

    if(scaleValue != 1.0) ScaleImage(0.0);
    else DisplayImage(ConvertImage::CVMatToQImage(imgForDisplay));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete undoStack;
    delete scaleLabel;
    delete resolutionLabel;
}

