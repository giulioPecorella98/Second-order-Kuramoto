#pragma once
#include "order_parameter.h"

void finiteDifference(
    Density& f, Density& fnew, Frequency& g,
    std::vector<double>& cosine, std::vector<double>& sine,
    std::vector<double>& freq, std::vector<double>& omegaValues,
    std::vector<std::vector<int>>& kLie,
    int thetaPoints, double dTheta, 
    int omegaPoints, double dOmega,
    int frequencyPoints, double dFrequency,
    double dt, double D, double m, double K);
