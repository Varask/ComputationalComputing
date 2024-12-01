#include <vector>
#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
#ifdef _WIN32
#include <direct.h> // For _mkdir on Windows
#else
#include <sys/stat.h> // For mkdir on Linux/Mac
#endif

#include "./Tools/WaveEquationSolver.cpp" // Include the WaveEquationSolver implementation

/// @brief Creates a folder in the file system
/// @param folder Name of the folder to be created
void createFolder(const std::string& folder) {
#ifdef _WIN32
    if (_mkdir(folder.c_str()) != 0) { // Create the folder (Windows)
        perror("Error creating folder");
    }
#else
    if (mkdir(folder.c_str(), 0777) != 0) { // Create the folder with permissions (Linux/Mac)
        perror("Error creating folder");
    }
#endif
}

int main() {
    // Create a folder to store results
    std::string folder = "Results";
    createFolder(folder);

    // Simulation variables
    double L = 100.0;    // Domain length
    double u = 1.75;     // Advection velocity
    double CFL = 0.5;     // Courant-Friedrichs-Lewy number

    // Define boundary conditions
    Bondary SET1_SIGN = {SET1_Function, 0, 1}; // SET1_SIGN: Step function with boundaries 0 and 1
    Bondary SET2_EXP = {SET2_Function, 0, 0}; // SET2_EXP: Exponential function with boundaries 0 and 0

    // Define values for time and spatial resolution
    std::vector<int> t_values = {5,10};      // Different time durations
    std::vector<int> N_values = {100, 200, 400}; // Different number of spatial points

    // Store different input configurations
    std::vector<Input> inputs;


    for  (int n: N_values){
        for (Bondary bondary : {SET1_SIGN, SET2_EXP}) {
            Input input = {u, L, -L / 2, L / 2, t_values[1], n, CFL, bondary};
            inputs.push_back(input); // Add input configuration to the list
        }
    }
        

    // Solve the wave equation for each input configuration
    for (Input input : inputs) {
        WaveEquationSolver solver(input);

        // Determine the name of the boundary function for file naming
        std::string bondary_name = (input.bondary.t0_function == SET1_Function) ? "SET1_sign" :
                                   (input.bondary.t0_function == SET2_Function) ? "SET2_exp" :
                                   "UNKNOWN";

        // Solve using Explicit FTBS and write results to CSV
        solver.solve_E_FTBS(folder + "/E_FTBS_" + bondary_name + "_" + 
                            std::to_string(input.N) + "_" + std::to_string(input.t_max) + ".csv");

        // Solve using Implicit FTBS and write results to CSV
        solver.solve_I_FTBS(folder + "/I_FTBS_" + bondary_name + "_" + 
                            std::to_string(input.N) + "_" + std::to_string(input.t_max) + ".csv");

        // Solve using Lax-Wendroff and write results to CSV
        solver.solve_Lax_Wendroff(folder + "/LW_" + bondary_name + "_" + 
                                  std::to_string(input.N) + "_" + std::to_string(input.t_max) + ".csv");

        // Solve using Richtmyer Multistep and write results to CSV
        solver.solve_Richtmyer_MultiStep(folder + "/Richtmyer_" + bondary_name + "_" + 
                                         std::to_string(input.N) + "_" + std::to_string(input.t_max) + ".csv");
    }

    return 0; // Exit program
}
