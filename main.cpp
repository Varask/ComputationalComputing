#include <math.h>
#include <vector>
#include <iostream>
#include <string>
#include <fstream>

#include "./Tools/Explicit_Schemes.cpp"

struct Bondary {
    double (*t0_function)(double); // t = 0 => f(x,t) = t0_function(x)
    double left;    // x = 0 => f(-L/2,t) = left
    double right;   // x = L => f(L/2,t) = right
};

struct Input {
    double u;       //  m/s
    double L;       //  m
    double x_min;   //  m
    double x_max;   //  m
    double t_max;   //  s
    double N;       //  number of points
    double CFL;     //  Courant-Friedrichs-Lewy number
    Bondary bondary;
};

double sign(double x) {
    if (x > 0) return 1;
    if (x < 0) return -1;
    return 0;
}

double xConverters(double x, double L) {
    if (x < 0) {
        x = 0;
    } else if (x > L) {
        x = L;
    }

    return x - L / 2;
}


double SET1_Function(double x) {
    return 0.5 * (sign(x) + 1);
}

double SET2_Function(double x) {
    return 0.5 * exp(-pow(x, 2));
}

class WaveEquationSolver {
public:
    enum Scheme {E_FTBS, I_FTBS, Lax_Wendroff, Richtmyer_MultiStep, All};

    double dt;
    double dx;
    int step_x;
    int step_t;

    WaveEquationSolver(Input input) {
        this->input = input;

        this->dx = (input.x_max - input.x_min) / input.N;
        this->dt = input.CFL * dx / input.u;

        this->step_x = static_cast<int>(input.N);
        this->step_t = static_cast<int>(input.t_max / dt);
    }

    WaveEquationSolver(double u, double L, double x_min, double x_max, double t_max, double N, double CFL, Bondary bondary) {
        this->input = {u, L, x_min, x_max, t_max, N, CFL, bondary};

        this->dx = (x_max - x_min) / N;
        this->dt = CFL * dx / u;

        this->step_x = static_cast<int>(N);
        this->step_t = static_cast<int>(t_max / dt);
    }

    void changeInput(Input input) {
        this->input = input;

        this->dx = (input.x_max - input.x_min) / input.N;
        this->dt = input.CFL * dx / input.u;

        this->step_x = static_cast<int>(input.N);
        this->step_t = static_cast<int>(input.t_max / dt);
    }

    std::vector<double> InitialCondition() {
        std::vector<double> row;
        for (int i = 0; i < step_x; i++) {
            double actual_x = xConverters(i * dx, input.L);
            row.push_back(input.bondary.t0_function(actual_x));
        }
        return row;
    }

    void solve(Scheme scheme, std::string filename = "") {
        switch (scheme) {
            case E_FTBS:
                solve_E_FTBS(filename);
                break;
            case I_FTBS:
                solve_I_FTBS(filename);
                break;
            case Lax_Wendroff:
                solve_Lax_Wendroff(filename);
                break;
            case Richtmyer_MultiStep:
                solve_Richtmyer_MultiStep(filename);
                break;
            case All:
                solve_All(filename);
                break;
            default:
                break;
        }
    }


    void solve_E_FTBS(std::string filename = "") {
        std::vector<std::vector<double>> data;
        std::vector<double> initialRow = InitialCondition();
        
        std::vector<double> spatial_positions;
        for (int i = 0; i < step_x; i++) {
            spatial_positions.push_back(xConverters(i * dx, input.L));
        }


        data.push_back(initialRow);  // Add initial condition at t = 0


        for (int t = 1; t < step_t; t++) {
            std::vector<double> row;
            for (int x = 0; x < step_x; x++) {
                if (x == 0) {
                    row.push_back(input.bondary.left);
                } else if (x == step_x - 1) {
                    row.push_back(input.bondary.right);
                } else {
                    row.push_back(Explicit_Schemes::FTBS_alternative(spatial_positions[x], dx, dt, input.u, data[t - 1], x));  // Fix here
                }
            }

            data.push_back(row);
        }
        if (!filename.empty()) {
            // open file
            std::ofstream file(filename);
            if (!file.is_open()) {
                throw std::runtime_error("Unable to open file: " + filename);
            } else {
                file << "x,t,f(x,t)\n";
                for (int t = 0; t < step_t; t++) {
                    for (int x = 0; x < step_x; x++) {
                        file << spatial_positions[x] << "," << t * dt << "," << data[t][x] << "\n";
                    }
                }
            }

            
        } else {
            for (const auto& row : data) {
                for (double value : row) {
                    std::cout << value << " ";
                }
                std::cout << "\n";
            }
        }
    }

    void solve_I_FTBS(std::string filename = "") {
        vector<vector<double>> data;
        vector<double> initialRow = InitialCondition();

        vector<double> spatial_positions;
        for (int i = 0; i < step_x; i++) {
            spatial_positions.push_back(xConverters(i * dx, input.L));
        }

        data.push_back(initialRow);  // Add initial condition at t = 0
        
        for (int t = 1; t < step_t; t++) {
            vector<double> row;
            for (int x = 0; x < step_x; x++) {
                if (x == 0) {
                    row.push_back(input.bondary.left);
                } else if (x == step_x - 1) {
                    row.push_back(input.bondary.right);
                } else {
                    row.push_back(Implicit_Schemes::I_FTBS_alternative(spatial_positions[x], dx, dt, input.u, data[t - 1], x));  // Fix here
                }
            }

            data.push_back(row);
        }
        if (!filename.empty()) {
            // open file
            std::ofstream file(filename);
            if (!file.is_open()) {
                throw std::runtime_error("Unable to open file: " + filename);
            } else {
                file << "x,t,f(x,t)\n";
                for (int t = 0; t < step_t; t++) {
                    for (int x = 0; x < step_x; x++) {
                        file << spatial_positions[x] << "," << t * dt << "," << data[t][x] << "\n";
                    }
                }
            }

            
        } else {
            for (const auto& row : data) {
                for (double value : row) {
                    std::cout << value << " ";
                }
                std::cout << "\n";
            }
        }        
    }

    void solve_Lax_Wendroff(std::string filename = "") {
        // To be implemented
    }

    void solve_Richtmyer_MultiStep(std::string filename = "") {
        // To be implemented
    }

    void writeToCSV(std::string filename, std::vector<std::vector<double>> data) {
        std::ofstream file(filename);

        if (!file.is_open()) {
            throw std::runtime_error("Unable to open file: " + filename);
        }


    }

    void solve_All(std::string filename = "") {
        // To be implemented
    }



private:
    Input input;
};

int main() {
    double L = 100.0;
    double u = 1.75;
    double CFL = 0.90;

    Bondary SET1_Bondary = {SET1_Function, 0, 1};
    Bondary SET2_Bondary = {SET2_Function, 0, 0};

    std::vector<double> t = {5, 10,20};
    std::vector<double> N = {100, 200, 400};

    Input testInput = {u, L, -L / 2, L / 2, t[2], N[0], CFL, SET2_Bondary};

    WaveEquationSolver solver(testInput);

    solver.solve(WaveEquationSolver::E_FTBS,"test.csv");
    solver.solve(WaveEquationSolver::I_FTBS,"test_IFTBS.csv");
    return 0;
}
