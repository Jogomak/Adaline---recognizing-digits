#include "errorplot.h"
#include "ui_errorplot.h"

ErrorPlot::ErrorPlot(QWidget *parent) : QWidget(parent)
{
    this->setMinimumSize(1500, 800);

    table = new QTabWidget(this);
    table->setGeometry(10, 10, 1480, 780);
    plots = new QCustomPlot*[10];

    for(int i = 0; i < 10; i++){
        plots[i] = new QCustomPlot(this);
        plots[i]->yAxis->setLabel("Error");
        table->addTab(plots[i], "Adaline " + QString(char(i + 48)));
    }
}

ErrorPlot::~ErrorPlot()
{
    for(int i = 0; i < 10; i++){
        delete plots[i];
    }
    delete []plots;
    delete table;
}

void ErrorPlot::set_plot_xrange(int x_range){
    for(int i = 0; i < 10; i++){
        plots[i]->yAxis->setRange(0, 100);
        plots[i]->xAxis->setRange(0, x_range);
    }

    x = QVector<double>(x_range);
    for(int i = 0; i < x_range; i++){
        x[i] = i;
    }
}

void ErrorPlot::set_plot_data(int sheet_number, QVector<double> y){
    plots[sheet_number]->addGraph();
    plots[sheet_number]->graph(0)->setPen(QPen(Qt::GlobalColor(static_cast<unsigned int>(sheet_number) + 5)));
    plots[sheet_number]->graph(0)->setData(x, y);
    plots[sheet_number]->replot();
}
