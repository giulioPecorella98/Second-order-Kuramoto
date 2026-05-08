#pragma once

struct Parameters {   
    double Kmax;
    int Kpoints;
    double D; 
    double m;
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
    double Tmax;
};

Parameters loadParameters();