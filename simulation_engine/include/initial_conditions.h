#pragma once
#include <vector>
#include <cmath>
#include <iostream>
using Frequency = std::vector<double>;
using Density = std::vector<std::vector<Frequency>>;

void initialConditions(Density& f, Frequency& g, 
                       int thetaPoints, double dTheta, 
                       int omegaPoints, double dOmega, double minimumOmega, double maximumOmega,
                       int frequencyPoints, double dFrequency, double minimumFrequency, double maximumFrequency);