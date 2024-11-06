#include <math.h>
#include <vector>
#include <iostream>
#include <string>

#include "./BaseTools/Explicit_Schemes.cpp"

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
    double N;       //  number of points
    double CFL;     //  Courant-Friedrichs-Lewy number
    Bondary bondary;
};

double sign(double x){
    if (x > 0) return 1;
    if (x < 0) return -1;
    return 0;
}

double SET1_Function(double x){  
    return 1/2 * (sign(x)+1);
}

double SET2_Function(double x){
    return 1/2 * (exp(pow(-x,2)));
}

class WaveEquationSolver
{
    public:
        enum Scheme {E_FTBS, I_FTBS, Lax_Wendroff, Richtmyer_MultiStep, All};

        double dt;
        double dx;

        WaveEquationSolver(Input input)
        {
            this->input = input;

            this->dx = (input.x_max - input.x_min) / input.N;
            this->dt = input.CFL * dx / input.u;
        }

        WaveEquationSolver(double u, double L, double x_min, double x_max, double t_max, double N,double CFL, Bondary bondary)
        {
            this->input = {u, L, x_min, x_max, t_max, N, CFL, bondary};

            this->dx = (x_max - x_min) / N;
            this->dt = CFL * dx / u;
        }

        void changeInput(Input input)
        {
            this->input = input;

            this->dx = (input.x_max - input.x_min) / input.N;
            this->dt = input.CFL * dx / input.u;
        }

        void solve(Scheme scheme, std::string filename = "")
        {
        }

        void solve_E_FTBS(std::string filename = "")
        {
            
        }

        void solve_I_FTBS(std::string filename = "")
        {
        }

        void solve_Lax_Wendroff(std::string filename = "")
        {
        }

        void solve_Richtmyer_MultiStep(std::string filename = "")
        {
        }

        void solve_All(std::string filename = "")
        {
        }


    private:
        Input input;
};




int main()
{
    double L = 100.0;
    double u = 1.75;

    Bondary SET1_Bondary = {SET1_Function, 0, 1};
    Bondary SET2_Bondary = {SET2_Function, 0, 0};

    std::vector<double> t = {5,10};
    std::vector<double> N = {100,200,400};
    
    std::vector<Input> inputs;

    Input testInput = {u, L, -L/2, L/2, 5, 100, SET1_Bondary};
    return 0;
}

