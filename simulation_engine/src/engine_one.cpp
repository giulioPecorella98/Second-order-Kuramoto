#include "finite_difference.h"
#include "initial_conditions.h"
#include "parameters_one.h"
#include "frequency_order_parameter.h"
#include "order_parameter.h"
#include "saving_density.h"


double PI = 3.14159265358979323846;

int main() {
    // Enter the name of the binary file where the result will be saved, 
    // and save the result in a binary file in the "save/density" subfolder
    std::string filename = save();
    if (filename == "quit") {
        std::cout << "Returning to the main menu..." << std::endl;
        return 0;
    }
    std::filesystem::path fullpath = std::filesystem::path(PROJECT_ROOT) 
                                             / "save" / "density" / (filename);
    FILE* file = fopen(fullpath.string().c_str(), "wb");
    if (!file) {
        std::cerr << "Error: file not saved. "<< std::endl;
        return 1; 
    } 

    Parameters p = loadParameters();                                                            
    // Solution vector: frequency x omega x theta
    Density f(p.frequencyPoints, 
         std::vector<Frequency>(p.omegaPoints, Frequency(p.thetaPoints, 0.0)));     
    // Auxiliary vector          
    Density fnew(p.frequencyPoints, 
         std::vector<Frequency>(p.omegaPoints, Frequency(p.thetaPoints, 0.0)));     
    // Vector of natural frequencies distribution       
    Frequency g(p.frequencyPoints, 0.0);                                                       
    
    // Precompute auxiliary values for efficiency
    std::vector<double> cosine(p.thetaPoints), sine(p.thetaPoints);
    std::vector<double> freq(p.frequencyPoints), omegaValues(p.omegaPoints);
    std::vector<std::vector<int>> kLie(p.omegaPoints, 
                                       std::vector<int>(p.thetaPoints));
    for (int k = 0; k < p.thetaPoints; ++k) {
        cosine[k] = std::cos(k * p.dTheta);
        sine[k] = std::sin(k * p.dTheta);
    }
    for (int i = 0; i < p.frequencyPoints; ++i) {
        freq[i] = p.minimumFrequency + i * p.dFrequency;
    }
    int halfIntervals = static_cast<int>(p.maximumOmega / p.dOmega);
    for (int j = 0; j < p.omegaPoints; ++j) {
        omegaValues[j] = p.minimumOmega + j * p.dOmega;
        for (int k = 0; k < p.thetaPoints; ++k) {
            // Compute the index of the Lie derivative for each theta and omega
            // Take care of the periodic conditions by using modulo operation
            kLie[j][k] = static_cast<int>(k - j + halfIntervals)%p.thetaPoints;
            if (kLie[j][k] < 0) {
                kLie[j][k] += p.thetaPoints;
            }
        }
    }

    fwrite(&p.frequencyPoints, sizeof(int), 1, file);
    if (p.frequencyPoints == 1) {
        fwrite(&p.thetaPoints, sizeof(int), 1, file);
        fwrite(&p.omegaPoints, sizeof(int), 1, file);
        fwrite(&p.minimumOmega, sizeof(double), 1, file);
        fwrite(&p.maximumOmega, sizeof(double), 1, file);
    }
    else {
        fwrite(&p.minimumFrequency, sizeof(double), 1, file);
        fwrite(&p.maximumFrequency, sizeof(double), 1, file);
    }
    fwrite(&p.T, sizeof(double), 1, file);
    fwrite(&p.D, sizeof(double), 1, file);
    fwrite(&p.m, sizeof(double), 1, file);
    fwrite(&p.K, sizeof(double), 1, file);

    // Apply the initial conditions
    initialConditions(f, g, p.thetaPoints, p.dTheta, p.omegaPoints, p.dOmega, 
                      p.minimumOmega, p.maximumOmega, p.frequencyPoints, 
                      p.dFrequency, p.minimumFrequency, p.maximumFrequency);
    if (p.frequencyPoints != 1) {
        fwrite(g.data(), sizeof(double), p.frequencyPoints, file);
    }
    else {    
        for (const auto& naturalFrequency : f) {
            for (const auto& omega : naturalFrequency) {
                fwrite(omega.data(), sizeof(double), p.thetaPoints, file);
            }
        }
    }
    OrderParameter ordR = computeR(f, g, cosine, sine, p.thetaPoints, p.dTheta,
                                   p.omegaPoints, p.dOmega, 
                                   p.frequencyPoints, p.dFrequency);
    fwrite(&ordR.R, sizeof(double), 1, file);
    double S = computeS(f, g, omegaValues, p.thetaPoints, p.dTheta,
                        p.omegaPoints, p.dOmega, 
                        p.frequencyPoints, p.dFrequency);
    fwrite(&S, sizeof(double), 1, file);
    
    // Start the simulation
    double updateTime = 0.0;
    for (int t = 0; t < p.steps; t++) {

        // Compute the solution at each time step
        finiteDifference(f, fnew, g, cosine, sine, freq, omegaValues, kLie,
                         p.thetaPoints, p.dTheta, p.omegaPoints, p.dOmega,
                         p.frequencyPoints, p.dFrequency, p.dt, p.D, p.m, p.K);
        std::swap(f, fnew); 
        
        // Save the solution a the specified frame intervals
        updateTime += p.dt;
        if (static_cast<int>(updateTime / p.frameInterval) >= 1) {
            if (p.frequencyPoints == 1) {
                for (const auto& naturalFrequency : f) {
                    for (const auto& omega : naturalFrequency) {
                        fwrite(omega.data(), sizeof(double), 
                               p.thetaPoints, file);
                    }
                }   
            }
            OrderParameter ordR = computeR(f, g, cosine, sine, p.thetaPoints, 
                                           p.dTheta, p.omegaPoints, p.dOmega, 
                                           p.frequencyPoints, p.dFrequency);
            fwrite(&ordR.R, sizeof(double), 1, file);
            S = computeS(f, g, omegaValues, p.thetaPoints, p.dTheta,
                         p.omegaPoints, p.dOmega, 
                         p.frequencyPoints, p.dFrequency);
            fwrite(&S, sizeof(double), 1, file);
            updateTime = 0.0;

        }
        else if (t == p.steps - 1) {
            if (p.frequencyPoints == 1) {
                for (const auto& naturalFrequency : f) {
                    for (const auto& omega : naturalFrequency) {
                        fwrite(omega.data(), sizeof(double), 
                               p.thetaPoints, file);
                    }
                }
            }
            OrderParameter ordR = computeR(f, g, cosine, sine, p.thetaPoints, 
                                           p.dTheta, p.omegaPoints, p.dOmega,   
                                           p.frequencyPoints, p.dFrequency);
            fwrite(&ordR.R, sizeof(double), 1, file);
            S = computeS(f, g, omegaValues, p.thetaPoints, p.dTheta,
                         p.omegaPoints, p.dOmega, 
                         p.frequencyPoints, p.dFrequency);
            fwrite(&S, sizeof(double), 1, file);
        }

        // Simulation progress bar
        int progress = static_cast<int>((static_cast<double>(t)/p.steps)*100);
        if (progress >= 1) {
            std::string bar(progress, '=');
            bar.append(100 - progress, ' ');
            std::cout << "\rComputing: [" << bar << "] " << progress 
                      << "%" << std::flush;
        }
    }
    std::cout << "\rComputing: [" << std::string(100, '=') << "] 100%" 
              << std::endl;
    std::cout << "Simulation completed successfully." << std::endl;
    fclose(file);
    std::cout << "Result saved successfully in " << fullpath.generic_string() 
              << "." << std::endl;

    return 0;
}