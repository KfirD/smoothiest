
#include "trainer.h"

const int input_count = 5;
const int output_count = 5;

double feedback(vector<double> &inputs, vector<double> &outputs)
{
    double tot = 0;
    for (double i: outputs) tot += i;
    return tot;
}

Trainer::Trainer():
    pop(input_count, output_count, )
{

}
