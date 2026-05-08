#pragma once

struct Parameters {
    double T;   
    double D; 
    double m;
    double K; 
    double dTheta;
    int thetaPoints;
    double dOmega;
    double minimumOmega;
    double maximumOmega;
    int omegaPoints;
    double dFrequency;
    double minimumFrequency;
    double maximumFrequency;
    int frequencyPoints;
    double dt;
    int steps;
    double frameInterval;
};

Parameters loadParameters();