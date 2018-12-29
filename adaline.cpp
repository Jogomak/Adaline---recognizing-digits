#include "adaline.h"
#include <iostream>

AdaLine::AdaLine(int input_data_size, double **learning_data_dft_amplitude, int learning_data_size, int recognize_number){
    this->input_data_size = input_data_size;
    this->learning_data_dft_amplitude = learning_data_dft_amplitude;
    this->learning_data_size = learning_data_size;
    this->recognize_number = recognize_number;

    srand(static_cast<unsigned int>(time(nullptr)));
    weights = new double[input_data_size];
    best_weights = new double[input_data_size];
    best_error = 100;
    learning_rate = 0.0005;
    initialize_weights();
}

AdaLine::~AdaLine(){
    delete[] weights;
}

void AdaLine::initialize_weights(){
    for(int i = 0; i < input_data_size; i++){
        weights[i] = static_cast<double>(rand()) / RAND_MAX * 2 - 1;
    }

    bias = static_cast<double>(rand()) / RAND_MAX * 2 - 1;
}

double AdaLine::get_error(){
    double error = 0;
    int correct_answer;

    for(int i = 0; i < input_data_size; i++){
        if(i / 10 == recognize_number){
            correct_answer = 1;
        }else{
            correct_answer = -1;
        }
        error += pow(get_answer(learning_data_dft_amplitude[i]) - correct_answer, 2);
    }

    return error;
}

double AdaLine::get_answer(double *input_data_dft){
    double answer = 0;

    for(int i = 0; i < input_data_size; i++){
        answer += input_data_dft[i] * weights[i];
    }

    return answer + bias;
}

QVector<double> AdaLine::learn(int max_steps, double acceptable_error, int max_repeats){
    int sample_number;
    double correct_answer;
    double answer;
    QVector<double> errors(max_steps);

    while(get_error() > acceptable_error && max_repeats > 0){
        for(int i = 0; i < max_steps; i++){
            if(i % 4){
                sample_number = rand() % 100;
            }else{
                sample_number = rand() % 10 + recognize_number * 10;
            }

            if(sample_number / 10 == recognize_number){
                correct_answer = 1;
            }else{
                correct_answer = -1;
            }

            answer = get_answer(learning_data_dft_amplitude[sample_number]);

            for(int j = 0; j < input_data_size; j++){
                weights[j] += learning_rate * (correct_answer - answer) * learning_data_dft_amplitude[sample_number][j];
            }
            bias += learning_rate * (correct_answer - answer);
            if(max_repeats == 3){
                errors[i] = get_error();
            }
        }
        max_repeats--;
        //std::cout << recognize_number  << ' ' << max_repeats << ' ' << get_error() << '\n';
        if(get_error() < best_error){
            memcpy(best_weights, weights, static_cast<unsigned int>(input_data_size) * sizeof(double));
            best_bias = bias;
            best_error = get_error();
        }

        if(!(max_repeats % 3)){
            initialize_weights();
        }
    }

    memcpy(weights, best_weights, static_cast<unsigned int>(input_data_size) * sizeof(double));
    bias = best_bias;
    //std::cout << recognize_number  << ' ' << max_repeats << ' ' << get_error() << '\n';

    return errors;
}
