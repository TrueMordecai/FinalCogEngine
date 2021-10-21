#include <QApplication>
#include "mywindow.h"
#include <QPictureFormatPlugin>


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    myWindow win;
    win.show();
    return app.exec();
}
