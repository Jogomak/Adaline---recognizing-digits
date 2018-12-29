#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QImage>
#include <QPainter>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QString>
#include <QPushButton>
#include <QLabel>
#include <QRect>
#include <QBoxLayout>
#include <complex>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <unistd.h>
#include "adaline.h"
#include "errorplot.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(ErrorPlot *, QWidget *parent = nullptr);
    ~MainWindow();

    int learning_sample_size;
    int image_original_size;
    int image_pixels_original_count;

    QImage *image_in;
    int image_width_height;
    unsigned int white, black;
    unsigned int painting_color;
    unsigned int *image_in_bits;
    unsigned int mouse_button_pressed;
    int x, y;

    QBoxLayout *box_layout_vertical;
    QPushButton *button_clear_image;
    QLabel *label_result;

    std::complex<double> *weights_dft;
    double **learning_samples_dft_amplitude;
    double *learning_sample_tmp;
    double *input_data;
    double *input_data_dft_amplitude;

    std::thread threads[5];

    ErrorPlot *plot;
    AdaLine *adalines[10];

    void initialize_adaline_and_prepare_data();
    void learn_adaline();
    void put_pixel();
    void check();
    void set_plot_data(int, QVector<double>);

private slots:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void clear_image();
};

#endif // MAINWINDOW_H
