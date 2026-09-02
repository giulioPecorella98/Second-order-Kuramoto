/*
Function to load the parameters of the simulation by asking the user to input 
them in the terminal. The function also computes some derived parameters such 
as the time step dt, the number of steps, and the number of frames to be 
plotted, by taking care of the stability condition for the numerical scheme.
*/
#include "parameters_one.h"
#include <iostream>
#include <limits>
#include <algorithm>
#include <cmath>

Parameters loadParameters() {
    
    double PI = 3.14159265358979323846;
    std::cout << "\nParameters acquisitions." << std::endl;
    double T;   
    std::cout << "1) Enter the final time T: ";
    std::cin >> T;  
    while ((std::cin.fail()) || (T <= 0)) {
        std::cout << 
                  "Invalid choice. The final time must be a positive number: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin >> T;
    }             
    double D; 
    std::cout << "2) Enter the noise level D: ";
    std::cin >> D;  
    while ((std::cin.fail()) || (D < 0)) {
        std::cout << 
              "Invalid choice. The noise level must be a nonnegative number: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin >> D;
    }   
    double m;
    std::cout << "3) Enter the inertia constant m: ";
    std::cin >> m;
    while ((std::cin.fail()) || (m < 0)) {
        std::cout << 
                  "Invalid choice. The inertia must be a nonnegative number: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin >> m;
    }
    double K; 
    std::cout << "4) Enter the coupling constant K: ";
    std::cin >> K;
    while ((std::cin.fail()) || (K < 0)) {
        std::cout << 
        "Invalid choice. The coupling constant must be a nonnegative number: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin >> K;
    }            
    double dOmega;
    std::cout << "5) Enter the frequency discretization: ";
    std::cin >> dOmega;
    while ((std::cin.fail()) || (dOmega <= 0)) {
        std::cout << "Invalid choice. The frequency discretization "
                     "must be a positive number: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin >> dOmega;
    }
    std::cout << "6) The numerical scheme needs to truncate the frequency "
                 "in an interval symmetric around zero. Enter the maximum "
                 "frequency: ";
    double maximumOmega;
    std::cin >> maximumOmega;
    while (std::cin.fail()) {
        std::cout << "Invalid choice. Please try again: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin >> maximumOmega;
    }
    double minimumOmega = -maximumOmega;
    int omegaPoints = static_cast<int>(((maximumOmega-minimumOmega)/dOmega)+1);
    const int halfIntervalsPoints = std::ceil(maximumOmega / dOmega);
    dOmega = maximumOmega / halfIntervalsPoints;
    omegaPoints = 2 * halfIntervalsPoints + 1;

    double minimumFrequency;
    std::cout << "7) Enter the minimum natural frequency of the oscillators: ";
    std::cin >> minimumFrequency; 
    while (std::cin.fail()) {
        std::cout << "Invalid choice. Please try again: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin >> minimumFrequency;
    }      
    double maximumFrequency;
    std::cout << "8) Enter the maximum natural frequency of the oscillators: ";
    std::cin >> maximumFrequency; 
    while (std::cin.fail() || (maximumFrequency < minimumFrequency)) {
        std::cout << "Invalid choice. Remember that the minimum frequency is"
                     " at most equal to the maximum frequency: ";
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
        while ((std::cin.fail()) || 
               (dFrequency <= 0) || 
               (dFrequency > (maximumFrequency - minimumFrequency))) {
            std::cout << "Invalid choice. The natural frequency discretization"
                         " must be a positive number: ";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cin >> dFrequency;
        }
        frequencyPoints = 
          static_cast<int>(((maximumFrequency-minimumFrequency)/dFrequency)+1);
    }
    double frequencyMax = 
              std::max(std::abs(minimumFrequency), std::abs(maximumFrequency));
    double framePerSeconds;
    std::cout << "9) Enter the number of frames per seconds: ";
    std::cin >> framePerSeconds;  
    while ((std::cin.fail()) || 
           (framePerSeconds <= 0) || 
           (framePerSeconds < (1 / T))) {
        std::cout << "Invalid choice. Try again: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin >> framePerSeconds;
    }
    //Stability condition for the finite difference scheme 
    double dtMax = 0.9 * (m * m * dOmega * dOmega) / (
                2 * D + m * dOmega + m * dOmega * (K + maximumOmega + frequencyMax)
                );  
    double frameInterval = 1.0 / framePerSeconds;
    double dt = std::min(dtMax, frameInterval);  
    int steps = static_cast<int>(T / dt) + 1;
    double dTheta = dOmega * dt;
    int thetaPoints = static_cast<int>((2 * PI / dTheta) + 1);  
    
    return {T, D, m, K, dTheta, thetaPoints, dOmega, 
            minimumOmega, maximumOmega, omegaPoints, 
            dFrequency, minimumFrequency, maximumFrequency, 
            frequencyPoints, dt, steps, frameInterval};
}