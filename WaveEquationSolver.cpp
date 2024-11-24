#include <math.h>
#include <vector>
#include <iostream>
#include <string>
#include <fstream>
#include <regex>  
#include <cstdlib>
#ifdef _WIN32
#include <direct.h> // Pour _mkdir sous Windows
#else
#include <sys/stat.h> // Pour mkdir sous Linux/Mac
#endif

#include "Tools/Schemes.cpp"


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
    int t_max;       //  s
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



bool is_csv(const std::string& filename)
{
    std::regex csv_regex(".*\\.csv");
    return std::regex_match(filename, csv_regex);
}



class WaveEquationSolver
{
    public:
        enum Scheme {E_FTBS, I_FTBS, Lax_Wendroff, Richtmyer_MultiStep, data, All};

        double dt;
        double dx;
        std::vector<vector <double>> matrix; // 2D array 
        Input input;

        WaveEquationSolver(Input input) : input(input)
        {
            this->dx = (input.x_max - input.x_min) / input.N;
            this->dt = input.CFL * dx / input.u;
        }

        void writeMatixToCSV(string filename){
            if (is_csv(filename)){
                std::ofstream out(filename); 
                if (out.is_open())
                {
                    out << "x, t, f\n";
                    for (int i = 0; i < matrix.size(); i++) {
                        for (int j = 0; j < matrix[i].size(); j++) {
                            double x = input.x_min + j * dx;
                            out << x << ", " << i*dt << ", " << matrix[i][j] << "\n";
                        }
                    }
                }
                out.close(); // Closing csv file
            }
            else {
                std::cerr << "Erreur d'ouverture du fichier : " << filename << std::endl;
            }
        }



        void solve_E_FTBS(const std::string& filename = "")
        {
            //clear the matrix
            matrix.clear();
            
            // init the 1srt row of the matrix
            std::vector<double> row; 

            for (int i = 0; i < input.N; i++) {
                double x = input.x_min + i * dx;
                double value = input.bondary.t0_function(x);
                row.push_back(value);
            }

            matrix.push_back(row);

            // Calcul for the next rows

            for (double t = dt; t < input.t_max; t += dt){
                for (int i = 1; i < input.N; i++){
                    double x = input.x_min + i * dx;
                    double value = Explicit_Schemes::FTBS_alternative(x, dx, dt, input.u, matrix[matrix.size() - 1], i);
                    row[i] = value;
                }
                matrix.push_back(row);
            }

            writeMatixToCSV(filename);

        }

        //adding void function solve_I_FTBS adapted from solve_E_FTBS
        void solve_I_FTBS(const std::string& filename = "")
        {
            //clear the matrix
            matrix.clear();
            
            // init the 1srt row of the matrix
            std::vector<double> row; 

            for (int i = 0; i < input.N; i++) {
                double x = input.x_min + i * dx;
                double value = input.bondary.t0_function(x);
                row.push_back(value);
            }

            matrix.push_back(row);

            // Calcul for the next rows

            for (double t = dt; t < input.t_max; t += dt){
                for (int i = input.N-2; i > 1; i--){
                    double x = input.x_min + i * dx;
                    double value = Implicit_Schemes::I_FTBS_alternative(x, dx, dt, input.u, row[i-1], matrix[matrix.size() - 1], i);
                    row[i] = value;
                }
                matrix.push_back(row);
            }

            writeMatixToCSV(filename);
        };

        void solve_Lax_Wendroff(const std::string& filename = "")
        {
            //clear the matrix
            matrix.clear();
            std::vector<double> row;

            // init the 1srt row of the matrix with the initial condition
            for (int i = 0; i < input.N; i++) {
                double x = input.x_min + i * dx;
                double value = input.bondary.t0_function(x);
                row.push_back(value);
            }

            matrix.push_back(row);

            for (double t = dt; t < input.t_max; t += dt){
                for (int i = 2; i < input.N - 2; i++){
                    double x = input.x_min + i * dx;
                    double value = Explicit_Schemes::Lax_Wendroff(x, dx, dt, input.u, matrix[matrix.size() - 1], i);
                    row[i] = value;
                }
                matrix.push_back(row);
            }

            writeMatixToCSV(filename);
        };

        void solve_Richtmyer_MultiStep(const std::string& filename = "")
        {
            //clear the matrix
            matrix.clear();
            std::vector<double> row;
            std::vector<double> row_half;

            // init the 1srt row of the matrix with the initial condition
            for (int i = 0; i < input.N; i++) {
                double x = input.x_min + i * dx;
                double value = input.bondary.t0_function(x);
                row.push_back(value);
            }

            matrix.push_back(row);
            row_half = row;

            for (double t = dt; t < input.t_max; t += dt){
                for (int i = 1; i < input.N - 2; i++){
                    double x = input.x_min + i * dx;
                    double value = Step_Schemes::Ritchmyer_method_prediction(x, dx, dt, input.u, matrix[matrix.size() - 1], i);
                    row_half[i] = value;
                }

                for (int i = 1; i < input.N - 2; i++){
                    double x = input.x_min + i * dx;
                    double value = Step_Schemes::Ritchmyer_method_correction(x, dx, dt, input.u, matrix[matrix.size() - 1], row_half, i);
                    row[i] = value;
                } 
                matrix.push_back(row);          
            }



            writeMatixToCSV(filename);
        };
};