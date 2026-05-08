/*
Function to load the parameters of the simulation by asking the user to input them in the terminal. 
The function also computes some derived parameters such as the time step dt, the number of steps, 
and the number of frames to be plotted, by taking care of the stability condition for the finite difference scheme.
*/
#include "parameters_one.h"
#include <iostream>
#include <limits>
#include <algorithm>

Parameters loadParameters() {
    
    double PI = 3.14159265358979323846;
    std::cout << "\nParameters acquisitions." << std::endl;
    double T;   
    std::cout << "1) Enter the final time T: ";
    std::cin >> T;  
    while ((T <= 0) || (std::cin.fail())) {
        std::cout << "Invalid choice. The final time must be a positive number: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin >> T;
    }             
    double D; 
    std::cout << "2) Enter the noise level D: ";
    std::cin >> D;  
    while ((D < 0) || (std::cin.fail())) {
        std::cout << "Invalid choice. The noise level must be a nonnegative number: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin >> D;
    }   
    double m;
    std::cout << "3) Enter the inertia constant m: ";
    std::cin >> m;
    while ((m < 0) || (std::cin.fail())) {
        std::cout << "Invalid choice. The inertia must be a nonnegative number: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin >> m;
    }
    double K; 
    std::cout << "4) Enter the coupling constant K: ";
    std::cin >> K;
    while ((K < 0) || (std::cin.fail())) {
        std::cout << "Invalid choice. The coupling constant must be a nonnegative number: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin >> K;
    }            
    double dOmega;
    std::cout << "5) Enter the frequency discretization: ";
    std::cin >> dOmega;
    while ((dOmega <= 0) || (std::cin.fail())) {
        std::cout << "Invalid choice. The frequency discretization must be a positive number: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin >> dOmega;
    }
    std::cout << "6) The numerical scheme needs to truncate the frequency interval: " << std::endl;
    double minimumOmega;
    std::cout << "-enter the minimum frequency: ";
    std::cin >> minimumOmega;
    while (std::cin.fail()) {
        std::cout << "Invalid choice. Please try again: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin >> minimumOmega;
    }
    double maximumOmega;
    std::cout << "-enter the maximum frequency: ";
    std::cin >> maximumOmega;
    while (std::cin.fail() || (maximumOmega < minimumOmega)) {
        std::cout << "Invalid choice. Remember that the minimum frequency is at most equal to the maximum frequency: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin >> maximumOmega;
    }
    double omegaMax = std::max(std::abs(minimumOmega), std::abs(maximumOmega));
    int omegaPoints = static_cast<int>(((maximumOmega - minimumOmega) / dOmega) + 1);
    double minimumFrequency;
    std::cout << "8) Enter the minimum natural frequency of the oscillators: ";
    std::cin >> minimumFrequency; 
    while (std::cin.fail()) {
        std::cout << "Invalid choice. Please try again: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin >> minimumFrequency;
    }      
    double maximumFrequency;
    std::cout << "9) Enter the maximum natural frequency of the oscillators: ";
    std::cin >> maximumFrequency; 
    while (std::cin.fail() || (maximumFrequency < minimumFrequency)) {
        std::cout << "Invalid choice. Remember that the minimum frequency is at most equal to the maximum frequency: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin >> maximumFrequency;
    }
    int frequencyPoints;
    double dFrequency;
    if (maximumFrequency == minimumFrequency) {
        dFrequency = 1.0;
        std::cout << "You choose identical oscillators." << std::endl;
        frequencyPoints = 1;
    }
    else {
        std::cout << "Enter the natural frequency discretization: ";
        std::cin >> dFrequency; 
        while ((dFrequency <= 0) || (dFrequency > (maximumFrequency - minimumFrequency)) || (std::cin.fail())) {
            std::cout << "Invalid choice. The natural frequency discretization must be a positive number: ";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cin >> dFrequency;
        }
        frequencyPoints = static_cast<int>(((maximumFrequency - minimumFrequency) / dFrequency) + 1);
    }
    double frequencyMax = std::max(std::abs(minimumFrequency), std::abs(maximumFrequency));
    double framePerSeconds;
    std::cout << "10) Enter the number of frames per seconds: ";
    std::cin >> framePerSeconds;  
    while ((framePerSeconds <= 0) || (std::cin.fail()) || (framePerSeconds < (1 / T))) {
        std::cout << "Invalid choice. Try again: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin >> framePerSeconds;
    }
    //Stability condition for the finite difference scheme 
    double dtMax = 0.9 * (m * m * dOmega * dOmega) / (2 * D + m * dOmega + m * dOmega * (K + omegaMax + frequencyMax));  
    double frameInterval = 1.0 / framePerSeconds;
    double dt = std::min(dtMax, frameInterval);  
    int steps = static_cast<int>(T / dt) + 1;
    double dTheta = dOmega * dt;
    int thetaPoints = static_cast<int>((2 * PI / dTheta) + 1);  
    
    return {T, D, m, K, dTheta, thetaPoints, dOmega, minimumOmega, maximumOmega, omegaPoints, 
            dFrequency, minimumFrequency, maximumFrequency, frequencyPoints, dt, steps, frameInterval};
}