#include "finite_difference.h"
#include "initial_conditions.h"
#include "parameters_two.h"
#include "order_parameter.h"
#include "saving_r.h"



int main() {
    // Enter the name of the binary file where the result will be saved, and save the result in a binary file in the "save/order_parameter" subfolder
    std::string filename = save();
    if (filename == "quit") {
        std::cout << "Returning to the main menu..." << std::endl;
        return 0;
    }
    std::filesystem::path fullpath = std::filesystem::path(PROJECT_ROOT) / "save" / "order_parameter" / (filename);
    FILE* file = fopen(fullpath.string().c_str(), "wb");
    if (!file) {
        std::cerr << "Error: file not saved. "<< std::endl;
        return 1; 
    } 

    Parameters p = loadParameters();                                        // Load parameters 
    fwrite(&p.Kpoints, sizeof(int), 1, file);
    fwrite(&p.Kmax, sizeof(double), 1, file);

    Density f(p.thetaPoints, Frequency(p.frequencyPoints, 0.0));         // Solution vector
    Density fnew(p.thetaPoints,  Frequency(p.frequencyPoints, 0.0));     // Auxiliary vector
    Frequency g(p.frequencyPoints, 0.0);                                 // Vector of natural frequencies

    // Apply the initial conditions and run the simulation for different values of K, saving the order parameter R for each value of K in the binary file
    initialConditions(f, g, p.thetaPoints, p.dTheta, p.frequencyPoints, p.dFrequency, p.minimumFrequency, p.maximumFrequency); 
    OrderParameter ordR =  computeR(f, g, p.thetaPoints, p.frequencyPoints, p.dTheta, p.dFrequency);              
    double multiplyFactor = p.Kmax / (p.Kpoints - 1);       
    double maxFrequency = std::max(std::abs(p.minimumFrequency), std::abs(p.maximumFrequency));
    double R = ordR.R;
    double Rnew;
    double Rold;
    int progress;
    int steps;
    int asymptotic;
    double K;
    double dt; 
    Density fInitial = f;
    Density fnewInitial = fnew;

    for (int i = 0; i < p.Kpoints; i++) {
        f = fInitial;
        fnew = fnewInitial;
        Rold = R;
        K = i * multiplyFactor;
        dt = 0.9 * (p.dTheta * p.dTheta) / (2 * p.D + (K + maxFrequency) * p.dTheta + K * p.dTheta * p.dTheta);
        steps = static_cast<int>(p.Tmax / dt) + 1;
        asymptotic = 0;
        for (int t = 0; t < steps; t++) {

            // Compute the solution at each time step
            finiteDifference(f, fnew, g, p.thetaPoints, p.dTheta, p.frequencyPoints, p.dFrequency, p.minimumFrequency,  dt, p.D, K);
            std::swap(f, fnew); 
            OrderParameter ordRnew =  computeR(f, g, p.thetaPoints, p.frequencyPoints, p.dTheta, p.dFrequency); 
            Rnew = ordRnew.R;

            // Check if the order parameter has reached an asymptotic value, in which case we can stop the simulation and save the result
            if (std::abs(Rnew - Rold) < 0.001) { asymptotic ++; }
            else { asymptotic = 0; }
            if (asymptotic == static_cast<int>(steps / 4)) { 
                fwrite(&Rnew, sizeof(double), 1, file); 
                break; 
            }  
            else if (t == steps - 1) { 
                fwrite(&Rnew, sizeof(double), 1, file); 
                std::cout << "\nWarning: the order parameter did not reach an asymptotic value for K = " << K << " ." << std::endl;
                std::cout << "Consider increasing the maximum simulation time." << std::endl;
            }
            std::swap(Rold, Rnew); 
        }

        // Simulation progress bar
        progress = static_cast<int>((K / p.Kmax) * 100);
        if (progress >= 1) {
            std::string bar(progress, '=');
            bar.append(100 - progress, ' ');
            std::cout << "\rComputing: [" << bar << "] " << progress << "%" << std::flush;
        }
    }
    std::cout << "\rComputing: [" << std::string(100, '=') << "] 100%" << std::endl;
    std::cout << "Simulation completed successfully." << std::endl;
    fwrite(&p.minimumFrequency, sizeof(double), 1, file);
    fwrite(&p.maximumFrequency, sizeof(double), 1, file);
    fwrite(g.data(), sizeof(double), p.frequencyPoints, file);
    fclose(file);
    std::cout << "Result saved successfully in " << fullpath.generic_string() << "." << std::endl;

    return 0;
}
