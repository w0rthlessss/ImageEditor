QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17
#CONFIG += release

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    AddCommand.cpp \
    ConvertImage.cpp \
    EffectLayer.cpp \
    Effects.cpp \
    SelectableImage.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    AddCommand.h \
    ConvertImage.h \
    EffectLayer.h \
    Effects.h \
    SelectableImage.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

INCLUDEPATH += C:\OpenCV\opencv\build\include

LIBS += C:\OpenCV\opencv\build\bin\libopencv_core480.dll
LIBS += C:\OpenCV\opencv\build\bin\libopencv_highgui480.dll
LIBS += C:\OpenCV\opencv\build\bin\libopencv_imgcodecs480.dll
LIBS += C:\OpenCV\opencv\build\bin\libopencv_imgproc480.dll
LIBS += C:\OpenCV\opencv\build\bin\libopencv_features2d480.dll
LIBS += C:\OpenCV\opencv\build\bin\libopencv_calib3d480.dll

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
