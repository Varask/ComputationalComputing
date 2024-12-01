#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

class Explicit_Schemes {
public:
    /// @brief Computes the forward finite difference of a function
    /// @param x The point at which the derivative is calculated
    /// @param dx The step size
    /// @param f The function whose derivative is being computed
    /// @return Approximate derivative using forward difference
    static double forward_diff(double x, double dx, double (*f)(double)) {
        return (f(x + dx) - f(x)) / dx;
    }

    /// @brief Computes the backward finite difference of a function
    /// @param x The point at which the derivative is calculated
    /// @param dx The step size
    /// @param f The function whose derivative is being computed
    /// @return Approximate derivative using backward difference
    static double backward_diff(double x, double dx, double (*f)(double)) {
        return (f(x) - f(x - dx)) / dx;
    }

    /// @brief Computes the central finite difference of a function
    /// @param x The point at which the derivative is calculated
    /// @param dx The step size
    /// @param f The function whose derivative is being computed
    /// @return Approximate derivative using central difference
    static double central_diff(double x, double dx, double (*f)(double)) {
        return (f(x + dx) - f(x - dx)) / (2 * dx);
    }

    /// @brief Computes the Forward Time Backward Space (FTBS) explicit scheme
    /// @param x The current point
    /// @param dx The spatial step size
    /// @param dt The time step size
    /// @param u Advection velocity
    /// @param f The function representing the previous time step
    /// @return Approximation using FTBS
    static double FTBS(double x, double dx, double dt, double u, double (*f)(double)) {
        return f(x) - (u * dt / dx) * (f(x) - f(x - dx));
    }

    /// @brief Computes the FTBS scheme using a vector of precedent values
    /// @param x The current point
    /// @param dx The spatial step size
    /// @param dt The time step size
    /// @param u Advection velocity
    /// @param precedentArray The array holding previous time step values
    /// @param i The index of the current point in the array
    /// @return Approximation using FTBS with precedent array
    static double FTBS_alternative(double x, double dx, double dt, double u, vector<double> precedentArray, int i) {
        return precedentArray[i] - (u * dt / dx) * (precedentArray[i] - precedentArray[i - 1]);
    }

    /// @brief Computes the Lax-Wendroff scheme for higher accuracy
    /// @param x The current point
    /// @param dx The spatial step size
    /// @param dt The time step size
    /// @param u Advection velocity
    /// @param precedentArray The array holding previous time step values
    /// @param i The index of the current point in the array
    /// @return Approximation using the Lax-Wendroff scheme
    static double Lax_Wendroff(double x, double dx, double dt, double u, vector<double> precedentArray, int i) {
        return precedentArray[i] - (u * dt / (2 * dx)) * (precedentArray[i + 1] - precedentArray[i - 1]) +
               (u * u * dt * dt / (dx * dx)) * 0.5 * (precedentArray[i + 1] - 2 * precedentArray[i] + precedentArray[i - 1]);
    }
};

class Implicit_Schemes {
public:
    /// @brief Computes the Forward Time Backward Space (FTBS) implicit scheme
    /// @param x The current point
    /// @param dx The spatial step size
    /// @param dt The time step size
    /// @param u Advection velocity
    /// @param f The function representing the previous time step
    /// @return Approximation using implicit FTBS
    static double FTBS(double x, double dx, double dt, double u, double (*f)(double)) {
        return 1 / (1 + u * dt / dx) * (f(x) + u * dt / dx * f(x - dx));  // CFL condition applied
    }

    /// @brief Computes the implicit FTBS scheme
    /// @param x The current point
    /// @param dx The spatial step size
    /// @param dt The time step size
    /// @param u Advection velocity
    /// @param f The function representing the previous time step
    /// @return Approximation using implicit FTBS
    static double I_FTBS(double x, double dx, double dt, double u, double (*f)(double)) {
        return (u * dt * f(x - dx) + dx * f(x)) / (u * dt + dx);
    }

    /// @brief Computes the implicit FTBS scheme using alternative logic
    /// @param x The current point
    /// @param dx The spatial step size
    /// @param dt The time step size
    /// @param u Advection velocity
    /// @param lastPoint The value at the previous point
    /// @param precedentArray The array holding values from the previous time step
    /// @param i The index of the current point in the array
    /// @return Approximation using implicit FTBS with an alternative method
    static double I_FTBS_alternative(double x, double dx, double dt, double u, double lastPoint, vector<double> precedentArray, int i) {
        return (u * dt * lastPoint + dx * precedentArray[i]) / (u * dt + dx);
    }
};

class Step_Schemes {
public:
    /// @brief Computes the prediction step of the Richtmyer method
    /// @param x The current point
    /// @param dx The spatial step size
    /// @param dt The time step size
    /// @param u Advection velocity
    /// @param precedentArray The array holding previous time step values
    /// @param i The index of the current point in the array
    /// @return Predicted value at the midpoint using Richtmyer method
    static double Ritchmyer_method_prediction(double x, double dx, double dt, double u, vector<double> precedentArray, int i) {
        return 0.5 * (precedentArray[i + 1] + precedentArray[i - 1]) -
               (u * dt / dx) * 0.25 * (precedentArray[i + 1] - precedentArray[i - 1]);
    }

    /// @brief Computes the correction step of the Richtmyer method
    /// @param x The current point
    /// @param dx The spatial step size
    /// @param dt The time step size
    /// @param u Advection velocity
    /// @param precedentArray The array holding previous time step values
    /// @param stepArray The array holding predicted values
    /// @param i The index of the current point in the array
    /// @return Corrected value at the midpoint using Richtmyer method
    static double Ritchmyer_method_correction(double x, double dx, double dt, double u, vector<double> precedentArray, vector<double> stepArray, int i) {
        return precedentArray[i] - (u * dt / dx) * 0.5 * (stepArray[i + 1] - stepArray[i - 1]);
    }
};
