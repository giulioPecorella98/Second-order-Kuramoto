#include "frequency_order_parameter.h"

double computeS (Density& f, Frequency& g, 
                 std::vector<double>& omegaValues,
                 int thetaPoints, double dTheta,
                 int omegaPoints, double dOmega,
                 int frequencyPoints, double dFrequency) {
    
    std::complex<double> S = 0.0;
    for (int i = 0; i < frequencyPoints; i++) {
        for (int j = 0; j < omegaPoints; j++) {
           // double omega = j * dOmega + minimumOmega;
            for (int k = 0; k < thetaPoints; k++) {
                S += std::exp(std::complex<double>(0.0, omegaValues[j])) 
                     * f[i][j][k] * g[i];
            }
        }
    }
    double result = std::abs(S) * dTheta * dOmega * dFrequency;
    
    return result;
}