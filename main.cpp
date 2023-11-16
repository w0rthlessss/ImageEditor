#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    QIcon appIcon(":/Icons/logo.png");
    w.setWindowIcon(appIcon);
    w.show();
    return a.exec();
}
