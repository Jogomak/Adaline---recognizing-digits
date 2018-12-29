#ifndef ERRORPLOT_H
#define ERRORPLOT_H

#include <QWidget>
#include <QTableWidget>
#include "qcustomplot.h"

class ErrorPlot : public QWidget
{
    Q_OBJECT

public:
    explicit ErrorPlot(QWidget *parent = nullptr);
    ~ErrorPlot();

    QTabWidget *table;
    QCustomPlot **plots;
    QVector<double> x;
    void set_plot_xrange(int);
    void set_plot_data(int, QVector<double>);
};

#endif // ERRORPLOT_H
