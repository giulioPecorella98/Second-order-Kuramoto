#include "order_parameter.h"

OrderParameter computeR (Density& f, Frequency& g, 
                         int thetaPoints, int omegaPoints, int frequencyPoints, 
                         double dTheta, double dOmega, double dFrequency) {
    
    double Rcos = 0.0, Rsin = 0.0, R = 0.0;
    for (int i = 0; i < thetaPoints; i++) {
        double psi = i * dTheta;
        for (int j = 0; j < omegaPoints; j++) {
            for (int k = 0; k < frequencyPoints; k++) {
                Rcos += cos(psi) * f[i][j][k] * g[k];
                Rsin += sin(psi) * f[i][j][k] * g[k];;
            }
        }
    }
    Rcos *= dTheta * dOmega * dFrequency;
    Rsin *= dTheta * dOmega * dFrequency;
    R = sqrt(Rcos * Rcos + Rsin * Rsin);
    
    return {Rcos, Rsin, R};
}