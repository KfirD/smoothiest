
#ifndef MATH_H
#define MATH_H

#include <vector>

/* Activation math */
double sum_ints(std::vector<double> &v);
double mult_ints(std::vector<double> &v);

double sigmoid(double num);
double relu(double num);
double gaussian(double num);

/* Activation functions */
double sigmoid_activation(std::vector<double> &inputs);
double tanh_activation(std::vector<double> &inputs);
double relu_activation(std::vector<double> &inputs);
double gaussian_activation(std::vector<double> &inputs);
double sine_activation(std::vector<double> &inputs);
double abs_activation(std::vector<double> &inputs);
double mult_activation(std::vector<double> &inputs);
double add_activation(std::vector<double> &inputs);
double square_activation(std::vector<double> &inputs);

/* Array of activation functions for efficient access */
const int activation_function_count = 9;
double (*const activation_functions[])(std::vector<double> &) = {
    sigmoid_activation,
    tanh_activation,
    relu_activation,
    gaussian_activation,
    sine_activation,
    abs_activation,
    mult_activation,
    add_activation,
    square_activation,
};

#endif
