#include "frequency_order_parameter.h"

double computeS (Density& f, Frequency& g, 
                         int thetaPoints, int omegaPoints, int frequencyPoints, 
                         double dTheta, double dOmega, double dFrequency, double minimumOmega) {
    
    std::complex<double> S = 0.0;
    for (int i = 0; i < thetaPoints; i++) {
        for (int j = 0; j < omegaPoints; j++) {
            double omega = j * dOmega + minimumOmega;
            for (int k = 0; k < frequencyPoints; k++) {
                S += std::exp(std::complex<double>(0.0, omega)) * f[i][j][k] * g[k];
            }
        }
    }
    double result = std::abs(S) * dTheta * dOmega * dFrequency;
    return result;
}