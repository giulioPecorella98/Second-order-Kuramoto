# First-order Kuramoto Model Simulator

This project is a numerical simulator for the first order, mean field Kuramoto PDE 

<p align="center">
  <img src="docs/eq.png" alt="Kuramoto PDE">
</p>

## Model

The equation describes the evolution of the density function, with:

- ρ : density function
- θ : phase variable
- t : time variable
- D : noise coefficient
- Ω : natural frequency  
- g(Ω) : natural frequency distribution
- K : coupling strength  

For more details about Kuramoto models see the introduction of [https://www.sciencedirect.com/science/article/pii/S0022247X24007595].

##  Description

The simulator allows the user to:
- compute numerical simulations of the Kuramoto dynamics
- compute phase synchronization dependence on the coupling constant K
- save simulation results for post processing and visualization
- data visualization of the results

## Numerical simulation

The user can specify a configuration (final time, noise level, coupling constant, 
initial condition, natural frequency distribution), and simulate the evolution of the density.

<table>
  <tr>
    <td align="center">
      <img src="docs/initial.png" width="450">
    </td>
    <td style="width:80px;"></td>
    <td align="center">
      <img src="docs/final.png" width="450">
    </td>
  </tr>
</table>

Warning: the numerical scheme is an explicit finite difference method. In order to have a stable method, the time discretization scales as the square of the phase discretization and the inverse of the noise level and the coupling strength. Suggested value for noise D and coupling K not greater than 100.

## Phase synchronization dependence on K

The user can specify a configuration (noise level, initial condition, natural frequency distribution), 
and analyze the dependance of the phase synchrony on the coupling strength K. The phase syncrony is given by the absolute value of the order parameter

<p align="center">
  <img src="docs/eq_r.png">
</p>

The phase synchrony can take values in the intervall [0,1]: when it is close to 0 there is low synchonization in the system, 
while when it is close to 1 the system is almost completely synchronized. In the Kuramoto model, a greater coupling constant
K usually implies greater phase synchrony

<p align="center">
  <img src="docs/eq_rK.png">
</p>

##  How to build and run

###  Requirements

- C++ compiler (e.g. `g++`, `clang++`, MSVC)
- CMake (>= 3.10)
- Python >= 3.8 with numpy and matplotlib

---

###  Build

Clone the repository:

```bash
git clone https://github.com/giulioPecorella98/First-order-Kuramoto-1.git
```

Set building directories:

```bash
cd First-order-Kuramoto-1
mkdir build
cd build
```

On Linux systems:
```bash
cmake ..
cmake --build .
```

On Windows, you need to specify a generator (e.g. MinGW):

```bash
cmake -G "MinGW Makefiles" ..
cmake --build .
```


Run the python script main.py.


---


## Notes

This is a numerical implementation for research/educational purposes. 
