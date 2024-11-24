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


#include "WaveEquationSolver.cpp"

void createFolder(const std::string& folder) {
#ifdef _WIN32
    if (_mkdir(folder.c_str()) != 0) { // Crée le dossier (Windows)
        perror("Erreur lors de la création du dossier");
    }
#else
    if (mkdir(folder.c_str(), 0777) != 0) { // Crée le dossier avec les permissions (Linux/Mac)
        perror("Erreur lors de la création du dossier");
    }
#endif
}


int main()
{
    std::string folder = "Results";
    createFolder(folder);


    //Variables
    double L = 100.0;
    double u = 1.75;
    double CFL = 0.8;

    Bondary SET1_SIGN = {SET1_Function, 0, 1};
    Bondary SET2_EXP = {SET2_Function, 0, 0};

    std::vector<int> t_values; 
    t_values.push_back(5);
    t_values.push_back(10);

    std::vector<int> N_values;  
    N_values.push_back(100);
    N_values.push_back(200);
    N_values.push_back(300);

    std::vector<Input> inputs;


    Input testInput = {u, L, -L/2, L/2, 10, 100, CFL, SET1_SIGN};

    WaveEquationSolver solver(testInput);

    for (int n : N_values){
        for (int t : t_values){
            for (Bondary bondary : {SET1_SIGN, SET2_EXP}){
                Input input = {u, L, -L/2, L/2, t, n, CFL, bondary};
                inputs.push_back(input);
            }
        }
    }

    for (Input input : inputs){
        WaveEquationSolver solver(input);

        std::string bondary_name =  (input.bondary.t0_function == SET1_Function) ? "SET1_sign" : 
                                    (input.bondary.t0_function == SET2_Function) ? "SET2_exp" : 
                                    "UNKNOWN";

        //solver.solve_E_FTBS(folder+"/E_FTBS_" +bondary_name+"_"+ std::to_string(input.N) + "_" + std::to_string(input.t_max) + ".csv");
        //OK
        solver.solve_I_FTBS(folder+"/I_FTBS_" +bondary_name+"_"+ std::to_string(input.N) + "_" + std::to_string(input.t_max) + ".csv");
        //OK
        //solver.solve_Lax_Wendroff(folder+"/LW_" +bondary_name+"_"+ std::to_string(input.N) + "_" + std::to_string(input.t_max) + ".csv");
        //OK 
        //solver.solve_Richtmyer_MultiStep(folder+"/Richtmyer_" +bondary_name+"_"+ std::to_string(input.N) + "_" + std::to_string(input.t_max) + ".csv");
        //OK
    }

    return 0;
}