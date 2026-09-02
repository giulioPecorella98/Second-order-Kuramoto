/* Function for computing the solution to the mean field Kuramoto PDE with 
noise level D and coupling strength K. The solution is computed by mixing Lie 
finite difference for the diffusive term and upwind scheme for the nonlinear 
drift, by taking care of the periodic boundary conditions.
*/
#include "finite_difference.h"

void finiteDifference(
    Density& f, Density& fnew, Frequency& g,
    std::vector<double>& cosine, std::vector<double>& sine,
    std::vector<double>& freq, std::vector<double>& omegaValues,
    std::vector<std::vector<int>>& kLie, 
    int thetaPoints, double dTheta, 
    int omegaPoints, double dOmega,
    int frequencyPoints, double dFrequency, 
    double dt, double D, double m, double K) {

    const double C1 = dt / dOmega / dOmega * D / m / m ;
    const double C2 = dt / dOmega / m;
    const double C3 = 1 + dt / m - 2 * C1;

    OrderParameter r =  computeR(f, g, cosine, sine, 
                                 thetaPoints, dTheta, 
                                 omegaPoints, dOmega, 
                                 frequencyPoints, dFrequency);                  
    for (int i = 0; i < frequencyPoints; i++) {
        for (int j = 1; j < omegaPoints - 1; j++) {
            for (int k = 0; k < thetaPoints; k++) {   
                // Compute the convolution terms by employing the order parameter R and the mean phase psi
                double fConvSin = K * (r.Rsin * cosine[k] - r.Rcos * sine[k]) 
                                  + freq[i];
                if (omegaValues[j] - fConvSin > 0) {
                    fnew[i][j][k] = f[i][j + 1][kLie[j][k]] 
                                    * (C1  +  C2 * (omegaValues[j] - fConvSin)) 
                                    + f[i][j][kLie[j][k]] 
                                    * (C3 - C2 * (omegaValues[j] - fConvSin))
                                    + f[i][j - 1][kLie[j][k]] * (C1); 
                }
                else {
                    fnew[i][j][k] = f[i][j + 1][kLie[j][k]] * (C1)
                                    + f[i][j][kLie[j][k]] 
                                    * (C3 + C2 * (omegaValues[j] - fConvSin))
                                    + f[i][j - 1][kLie[j][k]] 
                                    * (C1 -  C2 * (omegaValues[j] - fConvSin));
                }
            }
        }
    }
    // Mass conservation for every natural frequency
    double sum = 0;
    for (int i = 0; i < frequencyPoints; i++) {    
        for (int j = 0; j < omegaPoints; j++) {
            for (int k = 0; k < thetaPoints; k++) {
                sum += fnew[i][j][k];
            }
        }
        sum *= dTheta * dOmega;
        for (int j = 0; j < omegaPoints; j++) {
            for (int k = 0; k < thetaPoints; k++) {
                fnew[i][j][k] /= sum;
            }
        }
        sum = 0;
    }
}