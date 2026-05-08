#pragma once
#include <cmath>
#include <vector>
using Frequency = std::vector<double>;
using Density = std::vector<std::vector<Frequency>>;

struct OrderParameter {
    double Rcos;
    double Rsin;
    double R;
};

OrderParameter computeR (Density& f, Frequency& g,  
                         int thetaPoints, int omegaPoints, int frequencyPoints,
                         double dTheta, double dOmega, double dFrequency); 