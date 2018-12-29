#ifndef ADALINE_H
#define ADALINE_H

#include <QVector>
#include <random>
#include <ctime>

class AdaLine
{
public:
    AdaLine(int, double**, int, int);
    ~AdaLine();

    int recognize_number;
    int input_data_size;
    int learning_data_size;
    double **learning_data_dft_amplitude;
    double *weights;
    double *best_weights;
    double best_error;
    double best_bias;
    double bias;
    double learning_rate;

    void initialize_weights();
    double get_error();
    double get_answer(double*);
    QVector<double> learn(int, double, int = 10);
};

#endif // ADALINE_H
