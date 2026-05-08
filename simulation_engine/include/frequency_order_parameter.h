#pragma once
#include <cmath>
#include <vector>
#include <complex>
using Frequency = std::vector<double>;
using Density = std::vector<std::vector<Frequency>>;


double computeS (Density& f, Frequency& g,  
                         int thetaPoints, int omegaPoints, int frequencyPoints,
                         double dTheta, double dOmega, double dFrequency, double minimumOmega); 