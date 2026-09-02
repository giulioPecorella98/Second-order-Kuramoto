#include "order_parameter.h"

OrderParameter computeR (Density& f, Frequency& g,  
                         std::vector<double>& cosine, std::vector<double>& sine,
                         int thetaPoints, double dTheta,
                         int omegaPoints, double dOmega,
                         int frequencyPoints, double dFrequency) {
    
    double Rcos = 0.0, Rsin = 0.0, R = 0.0;
    for (int i = 0; i < frequencyPoints; i++) {
        for (int j = 0; j < omegaPoints; j++) {
            for (int k = 0; k < thetaPoints; k++) {
                Rcos += cosine[k] * f[i][j][k] * g[i];
                Rsin += sine[k] * f[i][j][k] * g[i];
            }
        }
    }
    Rcos *= dTheta * dOmega * dFrequency;
    Rsin *= dTheta * dOmega * dFrequency;
    R = sqrt(Rcos * Rcos + Rsin * Rsin);
    
    return {Rcos, Rsin, R};
}