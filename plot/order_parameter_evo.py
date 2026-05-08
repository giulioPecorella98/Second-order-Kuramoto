import numpy as np
import struct
import matplotlib.pyplot as plt
from pathlib import Path
import os



def orderParameterEvolution():

    simulation =  input("Which simulation do you wish to load? (type 's' to see available simulations, 'q' to quit to main menu) ")
    path = Path("save/order_parameter")
    path.mkdir(parents=True, exist_ok=True)
    while simulation == 's':
        print(f"The available simulations are: {', '.join(os.listdir(path))}")
        simulation = input("Which simulation do you wish to load? ")
    if simulation == 'q':
            return
    
    continueAnalysis = True
    while continueAnalysis:
        try:
            with open(path / simulation, "rb") as file:            
                KPoints = struct.unpack('i', file.read(4))[0]
                Kmax = struct.unpack('d', file.read(8))[0]
                K = np.linspace(0, Kmax, KPoints)
                r = np.fromfile(file, dtype = np.float64, count = KPoints)
                minimumFrequency = struct.unpack('d', file.read(8))[0]
                maximumFrequency = struct.unpack('d', file.read(8))[0]
                g = np.fromfile(file, dtype = np.float64, count = -1)
                continueAnalysis = False
        except Exception as e:
            print(f"An error occurred while reading the file: {e}. Returning to the main menu...")
            return  

    print("Plotting the order parameter evolution...")
    plt.figure()
    plt.plot(K, r)
    plt.title(f"Order parameter evolution")
    plt.xlim(0, K[-1])
    plt.ylim(0, 1.1)
    plt.xlabel(r"$K$")
    plt.ylabel(r"$r(K)$")
    plt.show(block = False)

    if len(g) > 1:
        print("Plotting the natural frequency distribution...")
        plt.figure()
        frequency = np.linspace(minimumFrequency, maximumFrequency, len(g))
        plt.plot(frequency, g)
        plt.title(f"Natural frequency distribution")
        plt.xlabel(r"$\Omega$")
        plt.ylabel(r"$g(\Omega)$")
        plt.xlim(minimumFrequency, maximumFrequency)
        plt.ylim(0, np.max(g) * 1.1)
        plt.show(block = False)

    input("Press Enter to close the plot...")
    plt.close('all')