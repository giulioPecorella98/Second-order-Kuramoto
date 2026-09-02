// Function to set the initial distributions of the simulation
#include "initial_conditions.h"

void initialConditions(
    Density& f, Frequency& g, 
    int thetaPoints, double dTheta, 
    int omegaPoints, double dOmega, double minimumOmega, double maximumOmega,
    int frequencyPoints, double dFrequency, 
    double minimumFrequency, double maximumFrequency) {   
    
    double PI = 3.14159265358979323846;
    double choice, n;
    double mean, variance, amplitude;
    double sum = 0;
    std::vector<double> theta(thetaPoints);
    for (int k = 0; k < thetaPoints; k++) {
        theta[k] = k * dTheta;
    }
    std::vector<double> omega(omegaPoints);
    for (int j = 0; j < omegaPoints; j++) { 
        omega[j] = j * dOmega + minimumOmega;
    }

    if (frequencyPoints != 1) { 
        std::cout << "Please choose one of the following natural frequency"
                     " initial conditions:" << std::endl;
        std::cout << "1. Gaussian-type" << std::endl;
        std::cout << "2. uniform distribution" << std::endl;
        std::cout << "3. to be implemented" << std::endl; 
        std::cout << "Enter your choice (1, 2 or 3): ";
        std::cin >> choice;
        while ((std::cin.fail()) || 
        (choice < 1) || (choice > 3) || (static_cast<int>(choice) != choice)) {
            std::cout << "Invalid choice. Please enter 1, 2 or 3: ";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cin >> choice;
        }
        switch (static_cast<int>(choice)) {
            case 1:
                std::cout << "Enter the mean of the distribution: ";
                std::cin >> mean;
                while ((mean < minimumFrequency) || (mean > maximumFrequency) || (std::cin.fail())) {
                    std::cout << "Invalid choice. The mean must be in the interval [" << minimumFrequency << ", " << maximumFrequency << "]: ";
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cin >> mean;
                }
                std::cout << "Enter the variance of the distribution: ";
                std::cin >> variance;
                while ((variance <= 0) || (std::cin.fail())) {
                    std::cout << "Invalid choice. The variance must be a positive number: ";
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cin >> variance;
                }
                for (int i = 0; i < frequencyPoints; i++) {
                    double frequency = minimumFrequency + i * dFrequency;
                    double diff = frequency - mean;
                    g[i] = std::exp(- diff * diff / variance);    
                }
                break;
            case 2:
                for (int i = 0; i < frequencyPoints; i++) {
                    g[i] = 1.0; 
                }
                break;
            case 3:
                std::cout << "This initial condition is not implemented yet, the uniform distribution will be used instead. " << std::endl;
                for (int i = 0; i < frequencyPoints; i++) {
                    g[i] = 1.0; 
                }
                break;
            default:
                break;
        }
        // Normalization of g
        for (int i = 0; i < frequencyPoints; i++) { sum += g[i]; }
        sum *= dFrequency;
        for (int i = 0; i < frequencyPoints; i++) { g[i] /= sum; }
    }
    else { g[0] = 1.0; }


    std::cout << "Please choose one of the following initial conditions for the density:" << std::endl;
    std::cout << "1. Gaussian in frequency, n-modal Gaussian-type in phase, uniform in natural frequency" << std::endl;
    std::cout << "2. Gaussian in frequency, uniform in phase and natural frequency" << std::endl;
    std::cout << "3. to be implemented" << std::endl;
    std::cout << "Enter your choice (1, 2 or 3): ";
    std::cin >> choice;
    while ((std::cin.fail()) || 
           (choice < 1) || 
           (choice > 3) || 
           (static_cast<int>(choice) != choice)) {
        std::cout << "Invalid choice. Please enter 1, 2 or 3: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin >> choice;
    }
    switch (static_cast<int>(choice)) {
        case 1: {
            std::cout << "Please choose the number of modes for phase: ";
            std::cin >> n;
            while ((n < 1) || (std::cin.fail())) {
                std::cout << 
                          "Invalid choice. Please enter the number of modes: ";
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), 
                                '\n');
                std::cin >> n;
            }
            for (int mode = 0; mode < n; mode++) {
                std::cout << "Enter the mean of the distribution for mode " 
                          << mode + 1 << ": ";
                std::cin >> mean;
                while ((std::cin.fail()) || 
                       (mean < 0) || 
                       (mean > 2 * PI)) {
                    std::cout << "Invalid choice. The mean must be in the"
                                 " interval [0,2pi]: ";
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), 
                                    '\n');
                    std::cin >> mean;
                }
                std::cout << "Enter the variance of the distribution for mode "
                          << mode + 1 << ": ";
                std::cin >> variance;
                while ((std::cin.fail()) || (variance <= 0)) {
                    std::cout << "Invalid choice. The variance must be a "
                                 "positive number: ";
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), 
                                    '\n');
                    std::cin >> variance;
                }
                std::cout << "Enter the amplitude of the distribution for mode " 
                          << mode + 1 << ": ";
                std::cin >> amplitude;  
                while ((amplitude < 0) || (std::cin.fail())) {
                    std::cout << "Invalid choice. The amplitude must be a "
                                 "positive number: ";
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), 
                                    '\n');
                    std::cin >> amplitude;
                }
                std::vector<double> diff(thetaPoints);
                for (int k = 0; k < thetaPoints; k++) {
                    diff[k] = theta[k] - mean;
                    if (diff[k] > PI) {diff[k] -= 2 * PI;}
                    else if (diff[k] < -PI) {diff[k] += 2 * PI;}
                }
                for (int i = 0; i < frequencyPoints; i++) {
                    for (int j = 0; j < omegaPoints; j++) {
                        for (int k = 0; k < thetaPoints; k++) {
                            f[i][j][k] += amplitude 
                              * std::exp(- diff[k] * diff[k] / (2 * variance));
                        }    
                    }
                }
            }
            std::cout << "Enter the mean frequency of the distribution: ";
            std::cin >> mean;
            while ((std::cin.fail()) || 
                   (mean < minimumOmega) || 
                   (mean > maximumOmega)) {
                std::cout << 
                           "Invalid choice. The mean must be in the interval [" 
                          << minimumOmega << "," << maximumOmega << "]: ";
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), 
                                '\n');
                std::cin >> mean;
            }
            std::cout << "Enter the variance of the distribution: ";
            std::cin >> variance;
            while ((std::cin.fail()) || (variance <= 0)) {
                std::cout << 
                    "Invalid choice. The variance must be a positive number: ";
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), 
                                '\n');
                std::cin >> variance;
            }
            std::vector<double> diffOmega(omegaPoints);
            for (int j = 0; j < omegaPoints; j++) {
                diffOmega[j] = omega[j] - mean;
            }
            for (int i = 0; i < frequencyPoints; i++) {
                for (int j = 0; j < omegaPoints; j++) {
                    for (int k = 0; k < thetaPoints; k++) {
                        f[i][j][k] *= amplitude 
                        * std::exp(- diffOmega[j] * diffOmega[j] / (2 * variance));
                        }    
                    }
                }      
            break;
        }
        case 2: {
            for (int i = 0; i < frequencyPoints; i++) {
                 for (int j = 0; j < omegaPoints; j++) {
                    for (int k = 0; k < thetaPoints; k++) {
                        f[i][j][k] = 1.0;
                    }
                }
            }
            std::cout << "Enter the mean frequency of the distribution: ";
            std::cin >> mean;
                while ((std::cin.fail()) || 
                       (mean < minimumOmega) || 
                       (mean > maximumOmega)) {
                    std::cout << 
                    "Invalid choice. The mean must be in the interval [" << minimumOmega << "," << maximumOmega << "]: ";
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cin >> mean;
                }
                std::cout << "Enter the variance of the distribution: ";
                std::cin >> variance;
                while ((std::cin.fail()) || (variance <= 0)) {
                    std::cout << "Invalid choice. The variance must be a positive number: ";
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cin >> variance;
                }
                std::vector<double> diffOmega(omegaPoints);
                for (int j = 0; j < omegaPoints; j++) {
                    diffOmega[j] = omega[j] - mean;
                }
                for (int i = 0; i < frequencyPoints; i++) {
                    for (int j = 0; j < omegaPoints; j++) {
                        for (int k = 0; k < thetaPoints; k++) {
                            f[i][j][k] *= amplitude * std::exp(
                                        - diffOmega[j] * diffOmega[j] 
                                        / (2 * variance)
                                        );
                        }
                    }
                }
            break;
        }
        case 3:
            std::cout << "This initial condition is not implemented yet, " 
                         "the uniform distribution will be used instead. " 
                      << std::endl;
            for (int i = 0; i < frequencyPoints; i++) {
                 for (int j = 0; j < omegaPoints; j++) {
                    for (int k = 0; k < thetaPoints; k++) {
                        f[i][j][k] = 1.0;
                    }
                }
            }
            break;
        default:
            break;
    }
    // Normalization for every natural frequency
    for (int i = 0; i < frequencyPoints; i++) {
        sum = 0;
        for (int j = 0; j < omegaPoints; j++) { 
            for (int k = 0; k < thetaPoints; k++) {
                sum += f[i][j][k];
            }
        }
        sum *= dTheta * dOmega;
        for (int j = 0; j < omegaPoints; j++) {
            for (int k = 0; k < thetaPoints; k++) {
                f[i][j][k] /= sum;
            }
        }
    }
}