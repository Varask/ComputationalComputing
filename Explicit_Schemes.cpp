// make a static class for schemes Forwards, Backwards and Central
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

class Explicit_Schemes {
public:
    static double forward_diff(double x, double dx, double (*f)(double)) {
        return (f(x + dx) - f(x)) / dx;
    }

    static double backward_diff(double x, double dx, double (*f)(double)) {
        return (f(x) - f(x - dx)) / dx;
    }

    static double central_diff(double x, double dx, double (*f)(double)) {
        return (f(x + dx) - f(x - dx)) / (2 * dx);
    }

    static double FTBS(double x, double dx, double dt,double u, double (*f)(double)) { // Forward Time Backward Space
        return f(x) - (u*dt / dx) * (f(x) - f(x - dx));
    }
};


class Implicit_Schemes {
    public:
    static double FTBS(double x, double dx, double dt,double u, double (*f)(double)) { // Forward Time Backward Space
        return 1/(1+u*dt/dx) * (f(x) + u*dt/dx * f(x - dx)); // ou u*dt/dx est le CFL
    }

    static double Lax_Wendroff(double x, double dx, double dt,double u, double (*f)(double)) {
        return f(x) - (u*dt / (2*dx)) * (f(x + dx) - f(x - dx)) + (u*u*dt*dt / (2*dx*dx)) * (f(x + dx) - 2*f(x) + f(x - dx));
    }


};

