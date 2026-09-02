import os
import struct
from pathlib import Path

import matplotlib.pyplot as plt
import numpy as np


RIGHT_BUTTON_PRESSED = False
def on_button_press(event):
    global RIGHT_BUTTON_PRESSED
    if event.button == 3:
        RIGHT_BUTTON_PRESSED = True

def on_button_release(event):
    global RIGHT_BUTTON_PRESSED   
    if event.button == 3:
        RIGHT_BUTTON_PRESSED = False

def on_scroll(event):
    ax = event.inaxes
    if ax is None:
        return
    delta = 5 if event.button == 'up' else -5 if event.button == 'down' else 0
    if delta == 0:
        return
    if RIGHT_BUTTON_PRESSED:
        ax.view_init(elev=ax.elev + delta, azim=ax.azim)
    else:
        ax.view_init(elev=30, azim=ax.azim + delta)
    event.canvas.draw_idle()



def density_evolution():

    simulation = input("Which simulation do you wish to load? (type 's' to " \
                       "see available simulations, 'q' to quit to main menu) ")
    path = Path("save/density")
    path.mkdir(parents=True, exist_ok=True)

    while simulation == 's':
        print(f"The available simulations are: {', '.join(os.listdir(path))}")
        simulation = input("Which simulation do you wish to load? ")    
    if simulation == 'q':
        return
    
    continue_analysis = True
    rho = []
    r = []
    s = []
    # Load data from the binary file   
    while continue_analysis:
        try:
            with open(path / simulation, "rb") as f:
                frequency_points = int(struct.unpack('i', f.read(4))[0])
                if frequency_points == 1:
                    theta_points = int(struct.unpack('i', f.read(4))[0])
                    omega_points = int(struct.unpack('i', f.read(4))[0])
                    minimum_omega = struct.unpack('d', f.read(8))[0]
                    maximum_omega = struct.unpack('d', f.read(8))[0]
                else:
                    minimum_frequency = struct.unpack('d', f.read(8))[0]
                    maximum_frequency = struct.unpack('d', f.read(8))[0]
                final_time = struct.unpack('d', f.read(8))[0]
                D = struct.unpack('d', f.read(8))[0]
                m = struct.unpack('d', f.read(8))[0]
                K = struct.unpack('d', f.read(8))[0]
                if frequency_points != 1:
                    g = np.fromfile(f, dtype=np.float64, 
                                    count=frequency_points)
                    while True:
                        data = f.read(8)
                        if len(data) != 8:
                            break
                        r.append(struct.unpack('d', f.read(8))[0])
                        s.append(struct.unpack('d', f.read(8))[0])
                else:
                    size = theta_points * omega_points * frequency_points
                    while True:
                        data = f.read(size * 8)  
                        if len(data) != size * 8:
                            break
                        rho.append(np.frombuffer(data, dtype=np.float64))
                        r.append(struct.unpack('d', f.read(8))[0])
                        s.append(struct.unpack('d', f.read(8))[0])
                    rho = np.array(rho)
                r = np.array(r)
                s = np.array(s)
                time_points = np.size(r)
                continue_analysis = False
        except Exception as e:
            print(f"An error occurred while reading the file: {e}. " \
                   "Returning to the main menu...")
            return  
            
    # Plot the evolution of the density for identical oscillators
    if (frequency_points == 1):
        rho = rho.reshape((time_points, frequency_points, 
                           omega_points, theta_points))
        dt = final_time / (time_points - 1)
        vmax = np.max(rho)
        vmin = np.min(np.min(rho), 0)  
        print("Plotting the evolution of the density...")
        theta = np.linspace(0, 2 * np.pi, theta_points)
        omega = np.linspace(minimum_omega, maximum_omega, omega_points)
        theta_grid, omega_grid = np.meshgrid(theta, omega)
        rho_initial = rho[0, 0, :, :] 
        fig3d = plt.figure()
        ax3d = fig3d.add_subplot(111, projection='3d')
        ax3d.plot_surface(theta_grid, omega_grid, rho_initial, 
                          cmap='viridis', vmin=vmin, vmax=vmax)
        ax3d.set_xlabel(r'$\theta$')
        ax3d.set_ylabel(r'$\omega$')
        ax3d.set_zlabel(r'$\rho$')
        ax3d.set_title('Initial density')
        fig3d.colorbar(plt.cm.ScalarMappable(cmap='viridis'), ax=ax3d)
        plt.show(block=False)
        rho_final = rho[-1, 0, :, :]  
        fig3d = plt.figure()
        ax3d = fig3d.add_subplot(111, projection='3d')
        ax3d.plot_surface(theta_grid, omega_grid, rho_final, 
                          cmap='viridis', vmin=vmin, vmax=vmax)
        ax3d.set_xlabel(r'$\theta$')
        ax3d.set_ylabel(r'$\omega$')
        ax3d.set_zlabel(r'$\rho$')
        ax3d.set_title('Final density')
        fig3d.colorbar(plt.cm.ScalarMappable(cmap='viridis'), ax=ax3d)
        plt.show(block=False)
    else:
        plt.figure()
        plt.plot(np.linspace(minimum_frequency, maximum_frequency, 
                             frequency_points), g)
        plt.title("Natural frequency distribution")
        plt.xlabel(r"$\Omega$")
        plt.ylabel(r"$g(\Omega)$")
        plt.show(block=False) 

    
    print("Plotting the order parameter evolution")
    plt.figure()
    plt.plot(np.linspace(0, final_time, time_points), r)
    plt.title(f"Noise D = {D:.2f}, inertia m = {m:.2f}, coupling K = {K:.2f}")
    plt.xlim(0, final_time)
    plt.ylim(0, 1.1)
    plt.xlabel(r"$t$")
    plt.ylabel(r"$r(t)$")
    plt.show(block=False)
    print("Plotting the frequency order parameter evolution")
    plt.figure()
    plt.plot(np.linspace(0, final_time, time_points), s)
    plt.title(f"Noise D = {D:.2f}, inertia m = {m:.2f}, coupling K = {K:.2f}")
    plt.xlim(0, final_time)
    plt.ylim(0, 1.1)
    plt.xlabel(r"$t$")
    plt.ylabel(r"$S(t)$")
    plt.show(block=False)

    input("Press Enter to close the plot...")
    plt.close('all')