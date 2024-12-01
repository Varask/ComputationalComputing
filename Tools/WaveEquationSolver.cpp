#include <math.h>
#include <vector>
#include <iostream>
#include <string>
#include <fstream>
#include <regex>
#include <cstdlib>
#ifdef _WIN32
#include <direct.h> // For _mkdir on Windows
#else
#include <sys/stat.h> // For mkdir on Linux/Mac
#endif

#include "Schemes.cpp"

/// @struct Bondary
/// @brief Represents boundary conditions and initial function for the wave equation
struct Bondary {
    double (*t0_function)(double); ///< Initial condition function: f(x, t=0)
    double left;    ///< Boundary condition at x = -L/2
    double right;   ///< Boundary condition at x = L/2
};

/// @struct Input
/// @brief Represents input parameters for solving the wave equation
struct Input {
    double u;       ///< Advection velocity in m/s
    double L;       ///< Length of the domain in meters
    double x_min;   ///< Minimum x value of the domain
    double x_max;   ///< Maximum x value of the domain
    int t_max;      ///< Maximum simulation time in seconds
    int N;          ///< Number of spatial points
    double CFL;     ///< Courant-Friedrichs-Lewy number
    Bondary bondary; ///< Boundary conditions and initial function
};

/// @brief Returns the sign of a number
/// @param x The number to evaluate
/// @return -1 for negative, 1 for positive, 0 for zero
double sign(double x) {
    if (x > 0) return 1;
    if (x < 0) return -1;
    return 0;
}

/// @brief Represents a step function
/// @param x The input value
/// @return 0.5 for x >= 0, 0 otherwise
double SET1_Function(double x) {
    return 0.5 * (sign(x) + 1);
}

/// @brief Represents a Gaussian-like function
/// @param x The input value
/// @return Value of the Gaussian function at x
double SET2_Function(double x) {
    return 0.5 * (exp(-x * x));
}

/// @brief Checks if a file is a CSV file
/// @param filename The file name to check
/// @return True if the file is a CSV, false otherwise
bool is_csv(const std::string& filename) {
    std::regex csv_regex(".*\\.csv");
    return std::regex_match(filename, csv_regex);
}

/// @class WaveEquationSolver
/// @brief A solver for the 1D wave equation using various numerical schemes
class WaveEquationSolver {
public:
    /// @enum Scheme
    /// @brief Enumeration of available numerical schemes
    enum Scheme { E_FTBS, I_FTBS, Lax_Wendroff, Richtmyer_MultiStep, data, All };

    double dt; ///< Time step size
    double dx; ///< Spatial step size
    std::vector<std::vector<double>> matrix; ///< Matrix to store solution over time
    Input input; ///< Input parameters

    /// @brief Constructor to initialize the solver with input parameters
    /// @param input Input parameters for the simulation
    WaveEquationSolver(Input input) : input(input) {
        this->dx = (input.x_max - input.x_min) / input.N;
        this->dt = input.CFL * dx / input.u;
    }

    /// @brief Writes the solution matrix to a CSV file
    /// @param filename The name of the output CSV file
    void writeMatixToCSV(std::string filename) {
        if (is_csv(filename)) {
            std::ofstream out(filename);
            if (out.is_open()) {
                out << "x, t, f\n";
                for (int i = 0; i < matrix.size(); i++) {
                    for (int j = 0; j < matrix[i].size(); j++) {
                        double x = input.x_min + j * dx;
                        out << x << ", " << i * dt << ", " << matrix[i][j] << "\n";
                    }
                }
            }
            out.close();
        } else {
            std::cerr << "Error opening file: " << filename << std::endl;
        }
    }

    /// @brief Solves the wave equation using the explicit FTBS scheme
    /// @param filename The name of the output CSV file
    void solve_E_FTBS(const std::string& filename = "") {
        matrix.clear();
        std::vector<double> row;

        for (int i = 0; i < input.N; i++) {
            double x = input.x_min + i * dx;
            row.push_back(input.bondary.t0_function(x));
        }
        matrix.push_back(row);

        for (double t = dt; t < input.t_max+dt; t += dt) {
            for (int i = 1; i < input.N; i++) {
                double x = input.x_min + i * dx;
                row[i] = Explicit_Schemes::FTBS_alternative(x, dx, dt, input.u, matrix.back(), i);
            }
            matrix.push_back(row);
        }

        writeMatixToCSV(filename);
    }

    /// @brief Solves the wave equation using the implicit FTBS scheme
    /// @param filename The name of the output CSV file
    void solve_I_FTBS(const std::string& filename = "") {
        matrix.clear();
        std::vector<double> row;

        for (int i = 0; i < input.N; i++) {
            double x = input.x_min + i * dx;
            row.push_back(input.bondary.t0_function(x));
        }
        matrix.push_back(row);

        for (double t = dt; t < input.t_max; t += dt) {
            for (int i = input.N - 2; i > 1; i--) {
                double x = input.x_min + i * dx;
                row[i] = Implicit_Schemes::I_FTBS_alternative(x, dx, dt, input.u, row[i - 1], matrix.back(), i);
            }
            matrix.push_back(row);
        }

        writeMatixToCSV(filename);
    }

    /// @brief Solves the wave equation using the Lax-Wendroff scheme
    /// @param filename The name of the output CSV file
    void solve_Lax_Wendroff(const std::string& filename = "") {
        matrix.clear();
        std::vector<double> row;

        for (int i = 0; i < input.N; i++) {
            double x = input.x_min + i * dx;
            row.push_back(input.bondary.t0_function(x));
        }
        matrix.push_back(row);

        for (double t = dt; t < input.t_max; t += dt) {
            for (int i = 2; i < input.N - 2; i++) {
                double x = input.x_min + i * dx;
                row[i] = Explicit_Schemes::Lax_Wendroff(x, dx, dt, input.u, matrix.back(), i);
            }
            matrix.push_back(row);
        }

        writeMatixToCSV(filename);
    }

    /// @brief Solves the wave equation using the Richtmyer multistep scheme
    /// @param filename The name of the output CSV file
    void solve_Richtmyer_MultiStep(const std::string& filename = "") {
        matrix.clear();
        std::vector<double> row, row_half;

        for (int i = 0; i < input.N; i++) {
            double x = input.x_min + i * dx;
            row.push_back(input.bondary.t0_function(x));
        }
        matrix.push_back(row);
        row_half = row;

        for (double t = dt; t < input.t_max; t += dt) {
            for (int i = 1; i < input.N - 2; i++) {
                double x = input.x_min + i * dx;
                row_half[i] = Step_Schemes::Ritchmyer_method_prediction(x, dx, dt, input.u, matrix.back(), i);
            }
            for (int i = 1; i < input.N - 2; i++) {
                double x = input.x_min + i * dx;
                row[i] = Step_Schemes::Ritchmyer_method_correction(x, dx, dt, input.u, matrix.back(), row_half, i);
            }
            matrix.push_back(row);
        }

        writeMatixToCSV(filename);
    }
};
