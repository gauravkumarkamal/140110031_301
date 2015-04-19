#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    w.setStyleSheet("background-image: url(:/myresource/background.jpg);");;

    return a.exec();
}
