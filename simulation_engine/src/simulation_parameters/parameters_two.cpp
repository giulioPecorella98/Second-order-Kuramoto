/*
Function to load the parameters of the simulation by asking the user to input 
them in the terminal, computing also some derived parameters
*/
#include "parameters_two.h"
#include <iostream>
#include <limits>       

Parameters loadParameters() {
    
    double PI = 3.14159265358979323846;
    std::cout << "\nParameters acquisitions." << std::endl;
    double Kmax;
    std::cout << "1) Enter the maximum coupling strength: ";
    std::cin >> Kmax;
    while (std::cin.fail() || Kmax < 0) {
        std::cout << "Invalid choice. The maximum coupling strength must be a"
                     " nonnegative number: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin >> Kmax;
    }
    int Kpoints;
    std::cout << "2) Enter the number of coupling strength points: ";
    std::cin >> Kpoints;
    while ( (std::cin.fail()) || (Kpoints <= 1) || ) {
        std::cout << "Invalid choice. The number of coupling strength points"
                     " must be an integer greater than 1: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin >> Kpoints;
    }
    double D; 
    std::cout << "3) Enter the noise level D: ";
    std::cin >> D;  
    while ((std::cin.fail()) || (D < 0) || ) {
        std::cout << 
              "Invalid choice. The noise level must be a nonnegative number: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin >> D;
    }  
    double m;
    std::cout << "4) Enter the inertia constant m: ";
    std::cin >> m;
    while ((std::cin.fail()) || (m <= 0)) {
        std::cout << "Invalid choice. The inertia must be a positive number: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin >> m;
    }
    std::cout << "5) The numerical scheme needs to truncate the frequency "
                 "in an interval symmetric around zero. Enter the maximum "
                 "frequency: " << std::endl;
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
    std::cout << "6) Enter the minimum natural frequency of the oscillators: ";
    std::cin >> minimumFrequency; 
    while (std::cin.fail()) {
        std::cout << "Invalid choice. Please try again: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin >> minimumFrequency;
    }      
    double maximumFrequency;
    std::cout << "7) Enter the maximum natural frequency of the oscillators: ";
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
        dFrequency = 1;
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
    double Tmax;
    std::cout << "8) Enter the maximum simulation time: ";
    std::cin >> Tmax;
    while ((Tmax <= 0) || (std::cin.fail())) {
        std::cout << "Invalid choice. The maximum simulation time must be a "
                     "positive number: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin >> Tmax;
    }
    double dTheta;
    int thetaPoints = static_cast<int>((2 * PI / dTheta) + 1);
    dTheta = 2 * PI / (thetaPoints - 1);
    
    return {Kmax, Kpoints, D, m, dTheta, thetaPoints, dOmega,
            minimumOmega, maximumOmega, omegaPoints, dFrequency, 
            minimumFrequency, maximumFrequency, frequencyPoints, Tmax};
}