
#include <cmath>

#include "activation.h"

/* Activation math */
double sum_ints(std::vector<double> &v)
{
    double value = 0;
    for (double num : v) {
        value += num;
    }
    return value;
}

double mult_ints(std::vector<double> &v)
{
    double value = 1;
    for (double num : v) {
        value *= num;
    }
    return value;
}

double sigmoid(double num)
{
    return 1 / (1 + std::exp(-num));
}

// uses leaky ReLU algorithm
double relu(double num)
{
    const double leak_coefficient = 0.01;
    if (num < 0)
        return leak_coefficient * num;
    else
        return num;
}

double gaussian(double num)
{
    return std::exp(-std::pow(num, 2));
}

/* Activation functions */
double sigmoid_activation(std::vector<double> &inputs)    { return sigmoid(sum_ints(inputs)); }
double tanh_activation(std::vector<double> &inputs)       { return std::tanh(sum_ints(inputs)); }
double relu_activation(std::vector<double> &inputs)       { return relu(sum_ints(inputs)); }
double gaussian_activation(std::vector<double> &inputs)   { return gaussian(sum_ints(inputs)); }
double sine_activation(std::vector<double> &inputs)       { return std::sin(sum_ints(inputs)); }
double abs_activation(std::vector<double> &inputs)        { return std::abs(sum_ints(inputs)); }
double mult_activation(std::vector<double> &inputs)       { return mult_ints(inputs); }
double add_activation(std::vector<double> &inputs)        { return sum_ints(inputs); }
double square_activation(std::vector<double> &inputs)     { return std::pow(sum_ints(inputs), 2); }
