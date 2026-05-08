import numpy as np
import struct
import matplotlib.pyplot as plt
from pathlib import Path
import os



rightButtonPressed = False
def onButtonPress(event):
    global rightButtonPressed
    if event.button == 3:
        rightButtonPressed = True

def onButtonRelease(event):
    global rightButtonPressed   
    if event.button == 3:
        rightButtonPressed = False

def onScroll(event):
    ax = event.inaxes
    if ax is None:
        return
    delta = 5 if event.button == 'up' else -5 if event.button == 'down' else 0
    if delta == 0:
        return
    if rightButtonPressed:
        ax.view_init(elev = ax.elev + delta, azim = ax.azim)
    else:
        ax.view_init(elev = 30, azim = ax.azim + delta)
    event.canvas.draw_idle()



def densityEvolution():

    simulation = input("Which simulation do you wish to load? (type 's' to see available simulations, 'q' to quit to main menu) ")
    path = Path("save/density")
    path.mkdir(parents = True, exist_ok = True)

    while simulation == 's':
        print(f"The available simulations are: {', '.join(os.listdir(path))}")
        simulation = input("Which simulation do you wish to load? ")    
    if simulation == 'q':
        return
    
    continueAnalysis = True
    rho = []
    r = []
    s = []
    # Load data from the binary file   
    while continueAnalysis:
        try:
            with open(path / simulation, "rb") as f:
                frequencyPoints = int(struct.unpack('i', f.read(4))[0])
                if frequencyPoints == 1:
                    thetaPoints = int(struct.unpack('i', f.read(4))[0])
                    omegaPoints = int(struct.unpack('i', f.read(4))[0])
                    minimumOmega = struct.unpack('d', f.read(8))[0]
                    maximumOmega = struct.unpack('d', f.read(8))[0]
                else:
                    minimumFrequency = struct.unpack('d', f.read(8))[0]
                    maximumFrequency = struct.unpack('d', f.read(8))[0]
                finalTime = struct.unpack('d', f.read(8))[0]
                D = struct.unpack('d', f.read(8))[0]
                m = struct.unpack('d', f.read(8))[0]
                K = struct.unpack('d', f.read(8))[0]
                if frequencyPoints != 1:
                    g = np.fromfile(f, dtype = np.float64, count = frequencyPoints)
                    while True:
                        data = f.read(8)
                        if len(data) != 8:
                            break
                        r.append(struct.unpack('d', f.read(8))[0])
                        s.append(struct.unpack('d', f.read(8))[0])
                else:
                    size = thetaPoints * omegaPoints * frequencyPoints
                    while True:
                        data = f.read(size * 8)  
                        if len(data) != size * 8:
                            break
                        rho.append(np.frombuffer(data, dtype = np.float64))
                        r.append(struct.unpack('d', f.read(8))[0])
                        s.append(struct.unpack('d', f.read(8))[0])
                    rho = np.array(rho)
                r = np.array(r)
                s = np.array(s)
                timePoints = np.size(r)
                continueAnalysis = False
        except Exception as e:
            print(f"An error occurred while reading the file: {e}. Returning to the main menu...")
            return  
            
    # Plot the evolution of the density for identical oscillators
    if (frequencyPoints == 1):
        rho = rho.reshape((timePoints, thetaPoints, omegaPoints, frequencyPoints))
        dt = finalTime / (timePoints - 1)
        vmax = np.max(rho)
        vmin = np.min(np.min(rho), 0)  
        print("Plotting the evolution of the density...")
        theta = np.linspace(0, 2 * np.pi, thetaPoints)
        omega = np.linspace(minimumOmega, maximumOmega, omegaPoints)
        Theta, Omega = np.meshgrid(theta, omega)
        rhoInitial = rho[0, :, :, 0].T  # initial density (thetaPoints, omegaPoints)
        fig3d = plt.figure()
        ax3d = fig3d.add_subplot(111, projection='3d')
        ax3d.plot_surface(Theta, Omega, rhoInitial, cmap='viridis', vmin=vmin, vmax=vmax)
        ax3d.set_xlabel(r'$\theta$')
        ax3d.set_ylabel(r'$\omega$')
        ax3d.set_zlabel(r'$\rho$')
        ax3d.set_title('Initial density')
        fig3d.colorbar(plt.cm.ScalarMappable(cmap='viridis'), ax=ax3d)
        plt.show(block=False)
        rhoFinal = rho[-1, :, :, 0].T  # final density (thetaPoints, omegaPoints)
        fig3d = plt.figure()
        ax3d = fig3d.add_subplot(111, projection='3d')
        ax3d.plot_surface(Theta, Omega, rhoFinal, cmap='viridis', vmin=vmin, vmax=vmax)
        ax3d.set_xlabel(r'$\theta$')
        ax3d.set_ylabel(r'$\omega$')
        ax3d.set_zlabel(r'$\rho$')
        ax3d.set_title('Final density')
        fig3d.colorbar(plt.cm.ScalarMappable(cmap='viridis'), ax=ax3d)
        plt.show(block=False)
    else:
        plt.figure()
        plt.plot(np.linspace(minimumFrequency, maximumFrequency, frequencyPoints), g)
        plt.title("Natural frequency distribution")
        plt.xlabel(r"$\Omega$")
        plt.ylabel(r"$g(\Omega)$")
        plt.show(block = False) 

    
    print("Plotting the order parameter evolution")
    print(np.min(r), np.max(r))
    plt.figure()
    plt.plot(np.linspace(0, finalTime, timePoints), r)
    plt.title(f"Noise D = {D:.2f}, inertia m = {m:.2f}, coupling K = {K:.2f}")
    plt.xlim(0, finalTime)
    plt.ylim(0, 1.1)
    plt.xlabel(r"$t$")
    plt.ylabel(r"$r(t)$")
    plt.show(block = False)
    print("Plotting the frequency order parameter evolution")
    plt.figure()
    plt.plot(np.linspace(0, finalTime, timePoints), s)
    plt.title(f"Noise D = {D:.2f}, inertia m = {m:.2f}, coupling K = {K:.2f}")
    plt.xlim(0, finalTime)
    plt.ylim(0, 1.1)
    plt.xlabel(r"$t$")
    plt.ylabel(r"$S(t)$")
    plt.show(block = False)

    input("Press Enter to close the plot...")
    plt.close('all')