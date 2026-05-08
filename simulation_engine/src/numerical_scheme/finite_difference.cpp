// Function for computing the solution to the mean field Kuramoto PDE with noise level D and coupling strength K.
#include "finite_difference.h"

void finiteDifference(Density& f, Density& fnew, Frequency& g,
                      int thetaPoints, double dTheta, 
                      int omegaPoints, double dOmega, double minimumOmega,
                      int frequencyPoints, double dFrequency, double minimumFrequency,
                      double dt, double D, double m, double K) {

    auto wrapIndex = [&](int index, int size) {
        int wrapped = index % size;
        return wrapped < 0 ? wrapped + size : wrapped;
    };

    OrderParameter r =  computeR(f, g, thetaPoints, omegaPoints, frequencyPoints, dTheta, dOmega, dFrequency);                  
    for (int k = 0; k < frequencyPoints; k++) {
        double frequency = minimumFrequency + k * dFrequency;
        for (int j = 1; j < omegaPoints - 1; j++) {
            double omega = minimumOmega + j * dOmega;
            for (int i = 0; i < thetaPoints; i++) {    
                double theta = i * dTheta;
                // Compute the convolution terms by employing the order parameter R and the mean phase psi
                double fConvSin = K * (r.Rsin * cos(theta) - r.Rcos * sin(theta)) + frequency;
                /* 
                Compute the solution by mixing Lie finite difference for the diffusive term and upwind scheme 
                for the nonlinear drift, by taking care of the periodic boundary conditions.
                */
                int iLie = wrapIndex(i - j, thetaPoints);
                if (omega - fConvSin > 0) {
                    fnew[i][j][k] = f[iLie][j + 1][k] * (dt / dOmega / dOmega * D / m / m  +  dt / dOmega / m * (omega - fConvSin)) 
                            + f[iLie][j][k] * (1 + dt / m - 2 * dt / dOmega / dOmega * D / m / m - dt / dOmega / m * (omega - fConvSin))
                            + f[iLie][j - 1][k] * (dt / dOmega / dOmega * D / m / m); 
                }
                else {
                    fnew[i][j][k] = f[iLie][j + 1][k] * (dt / dOmega / dOmega * D / m / m)
                            + f[iLie][j][k] * (1 + dt / m - 2 * dt / dOmega / dOmega * D / m / m + dt / dOmega / m * (omega - fConvSin))
                            + f[iLie][j - 1][k] * (dt / dOmega / dOmega * D / m / m  -  dt / dOmega / m * (omega - fConvSin));
                }
            }
        }
    }
    // Mass conservation for every natural frequency (the scheme should be preserving)
    double sum = 0;
    for (int k = 0; k < frequencyPoints; k++) {    
        for (int i = 0; i < thetaPoints; i++) {
            for (int j = 0; j < omegaPoints; j++) {
                sum += fnew[i][j][k];
            }
        }
        for (int i = 0; i < thetaPoints; i++) {
            for (int j = 0; j < omegaPoints; j++) {
                fnew[i][j][k] /= (sum * dTheta * dOmega);
            }
        }
        sum = 0;
    }
}