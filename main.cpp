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
            if (filename != "" && filename.find(".csv"))
            {
            
            std::ofstream out(filename); 
            if (out.is_open())
            {
                out << "x, t, f\n";
            }
   
            std::vector<double> f_next(input.N, 0.0);

            // Initialisation de f avec la fonction t0
            for (int i = 0; i < input.N; ++i) {
                double x = input.x_min + i * dx;
                f[i] = input.bondary.t0_function(x);
            }

            for (double t = 0; t < input.x_max; t += dt){
                for (int i = 1; i < input.N; ++i){
                    f_next[i] = f[i] - input.CFL * (f[i] - f[i - 1]);
                }
            }

            f_next[0] = input.bondary.left;
            f_next[input.N - 1] = input.bondary.right;

            f = f_next;

            // Enregistrement des résultats à chaque étape de temps dans le fichier CSVcls
            for (int i = 0; i < input.N; ++i) {
                double x = input.x_min + i * dx;
                out << x << ", " << i*dt << ", " << f[i] << "\n";
            }

            
            out.close(); // Fermeture du fichier CSV

            } else {
                std::cerr << "Erreur d'ouverture du fichier : " << filename << std::endl;
            }
        }
};

int main()
{
    //Variables
    double L = 100.0;
    double u = 1.75;
    double CFL = 0.8;

    Bondary SET1_Bondary = {SET1_Function, 0, 1};
    Bondary SET2_Bondary = {SET2_Function, 0, 0};

    std::vector<double> t_values; 
    t_values.push_back(5);
    t_values.push_back(10);

    std::vector<int> N_values;  
    N_values.push_back(100);
    N_values.push_back(200);
    N_values.push_back(300);

    std::vector<Input> inputs;


    Input testInput = {u, L, -L/2, L/2, 5, 100, CFL, SET1_Bondary};

    WaveEquationSolver solver(testInput);
    solver.solve_E_FTBS("test.csv");

    return 0;
}