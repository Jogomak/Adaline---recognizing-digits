#include "mainwindow.h"
#include "errorplot.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ErrorPlot plot;
    plot.show();

    MainWindow w(&plot);
    w.show();

    return a.exec();
}
