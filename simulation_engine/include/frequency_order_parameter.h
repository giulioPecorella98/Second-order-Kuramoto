#pragma once
#include <cmath>
#include <vector>
#include <complex>
using Frequency = std::vector<double>;
using Density = std::vector<std::vector<std::vector<double>>>; 


double computeS (
    Density& f, Frequency& g,  
    std::vector<double>& omegaValues,
    int thetaPoints, double dTheta, 
    int omegaPoints, double dOmega, 
    int frequencyPoints, double dFrequency); 