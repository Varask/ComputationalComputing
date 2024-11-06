#include <vector>
#include <iostream>
#include <cmath>
#include <Eigen/Dense>

class NumericalAnalysisTools {
public:
    // Constructor for initializing matrix
    NumericalAnalysisTools(const Eigen::MatrixXd& matrix)
        : A(matrix) {}

    // Function to calculate eigenvalues for stability analysis
    void calculateEigenvalues() {
        Eigen::EigenSolver<Eigen::MatrixXd> solver(A);
        eigenvalues = solver.eigenvalues();
    }

    // Function to print eigenvalues
    void printEigenvalues() {
        std::cout << "Eigenvalues:\n";
        for (int i = 0; i < eigenvalues.size(); ++i) {
            std::cout << eigenvalues[i] << std::endl;
        }
    }

    // Function to analyze convergence by checking spectral radius
    void analyzeConvergence() {
        double spectralRadius = 0.0;
        for (int i = 0; i < eigenvalues.size(); ++i) {
            double absValue = std::abs(eigenvalues[i]);
            if (absValue > spectralRadius) {
                spectralRadius = absValue;
            }
        }

        std::cout << "Spectral Radius: " << spectralRadius << std::endl;
        if (spectralRadius < 1.0) {
            std::cout << "The method is convergent." << std::endl;
        } else {
            std::cout << "The method is not convergent." << std::endl;
        }
    }

    // Function to evaluate consistency by comparing numerical and analytical solutions
    void evaluateConsistency(const std::vector<double>& numericalSolution, const std::vector<double>& analyticalSolution) {
        if (numericalSolution.size() != analyticalSolution.size()) {
            throw std::invalid_argument("The numerical and analytical solutions must have the same size.");
        }

        double errorNorm = 0.0;
        for (size_t i = 0; i < numericalSolution.size(); ++i) {
            errorNorm += std::pow(numericalSolution[i] - analyticalSolution[i], 2);
        }
        errorNorm = std::sqrt(errorNorm);

        std::cout << "Consistency Error Norm: " << errorNorm << std::endl;
    }

private:
    Eigen::MatrixXd A;
    Eigen::VectorXcd eigenvalues;
};