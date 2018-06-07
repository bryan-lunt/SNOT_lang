/*
MIT License

Copyright (c) 2017 Bryan Lunt <bjlunt2@illinois.edu> <bryan.j.lunt@gmail.com>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "param_storage.h"
#include <iostream>
#include <cmath>

#include <vector>
using namespace gsparams;

class Rosenbrock {
    public:
        double a;
        double b;
        DictList myvals;
        Rosenbrock(double _a,double _b) : a(_a), b(_b),myvals(){
            myvals["x"] = DictList(0.0);
            myvals["y"] = DictList(0.078);
        }

        double check_value(std::vector<double> params){

            //create a data structure from the params list.
            DictList tmpvals = myvals; //We could actually just populate into myvals, but this shows the ability to use a prototype pattern.
            tmpvals.populate(params);

            return pow(a-tmpvals["x"],2.0) + b*pow(tmpvals["y"]-pow(tmpvals["x"],2.0),2.0);
        }
};

double f(std::vector<double> params, void *other){
    Rosenbrock *as_r = (Rosenbrock*)other;

    return as_r->check_value(params);
}

std::vector<double> df(std::vector<double> params, void *other){
    double diff = 0.000001;
    std::vector<double> return_vector(params.size());

    double f_val = f(params, other);

    for(int i = 0;i<params.size();i++){
        std::vector<double> tmpparams = params;
        tmpparams[i] += diff;
        return_vector[i] = (f(tmpparams, other) - f_val)/diff;
    }
    return return_vector;
}



int main(){

    Rosenbrock myrosen(1.0,100.0);

    //We don't even need toknow the number of parameters in advance. The structure tells us.
    std::vector<dictlist_primitive_t> running_params(0);
    myrosen.myvals.traverse(&running_params);

    int n_parameters = running_params.size();
    std::cout << "The optimization function has " << n_parameters << " free parameters." << std::endl;

    int n_iters = 10000;

    double stepsize = 0.001;

    for(int i = 0;i<n_iters;i++){

        //gradient descent
        std::vector<dictlist_primitive_t> deriv_at_x = df(running_params,&myrosen);

        for(int i = 0;i<running_params.size();i++){
            running_params[i] -= stepsize*deriv_at_x[i];
        }


        //output
        if(i%200 == 0){
            double val = f(running_params,&myrosen);
            std::cout << "iteration " << i << " f : " << val << " : at : ";
            for(int i = 0;i<running_params.size();i++){
                std::cout << running_params[i] << " ";
            }
            std::cout << std::endl;
        }
    }

}
