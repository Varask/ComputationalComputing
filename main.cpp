#include <math.h>
#include <vector>
#include <iostream>
#include <string>
#include <fstream>

#include "./Tools/Explicit_Schemes.cpp"

struct Bondary
{
    double (*t0_function)(double); // t = 0 => f(x,t) = t0_function(x)
    double left;    // x = 0 => f(-L/2,t) = left
    double right;   // x = L => f(L/2,t) = right
};

struct Input
{
    double u;       //  m/s
    double L;       //  m
    double x_min;   //  m
    double x_max;   //  m
    double t_max;       //  s
    int N;       //  number of points, should be integer
    double CFL;     //  Courant-Friedrichs-Lewy number
    Bondary bondary;
};

double sign(double x){
    if (x > 0) return 1;
    if (x < 0) return -1;
    return 0;
}

double SET1_Function(double x){  
    return 0.5 * (sign(x) + 1);
}

double SET2_Function(double x){
    return 0.5 * (exp(-x * x));
}

class WaveEquationSolver
{
    public:
        enum Scheme {E_FTBS, I_FTBS, Lax_Wendroff, Richtmyer_MultiStep, data, All};

        double dt;
        double dx;
        std::vector<double> f;
        Input input;

        WaveEquationSolver(Input input) : input(input)
        {
            this->dx = (input.x_max - input.x_min) / input.N;
            this->dt = input.CFL * dx / input.u;
            f.resize(input.N, 0.0);
        }

        void solve_E_FTBS(const std::string& filename = "")
        {
            std::vector<double> f_next(input.N, 0.0);

            for (int i = 0; i < input.N; ++i) {
                double x = input.x_min + i * dx;
                f[i] = input.bondary.t0_function(x);
            }

            for (double t = 0; t < input.x_max; t += dt) {
                for (int i = 1; i < input.N; ++i) {
                    f_next[i] = f[i] - input.CFL * (f[i] - f[i - 1]);
                }
            }

            f_next[0] = input.bondary.left;
            f_next[input.N - 1] = input.bondary.right;
            f = f_next;

            if (!filename.empty()) {
                std::ofstream out(filename.c_str());
                if (out.is_open()) {
                    for (size_t i = 0; i < f.size(); ++i) {
                        out << f[i] << ",";
                    }
                    out << "\n";
                    out.close();
                } else {
                    std::cerr << "Erreur d'ouverture du fichier : " << filename << std::endl;
                }
            }
        }
};


int main()
{
    double L = 100.0;
    double u = 1.75;

    Bondary SET1_Bondary = {SET1_Function, 0, 1};
    Bondary SET2_Bondary = {SET2_Function, 0, 0};

    std::vector<double> t_values = {5, 10};
    std::vector<int> N_values = {100, 200, 300};  // Changed N to int for clarity

    std::vector<Input> inputs;

    for (size_t i = 0; i < t_values.size(); ++i) {
        for (size_t j = 0; j < N_values.size(); ++j) {
            inputs.push_back({u, L, -L/2, L/2, t_values[i], N_values[j], 0.5, SET1_Bondary});
            inputs.push_back({u, L, -L/2, L/2, t_values[i], N_values[j], 0.5, SET2_Bondary});
        }
    }

    return 0;
}