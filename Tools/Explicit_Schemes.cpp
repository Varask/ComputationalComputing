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

class Step_Schemes {
    public:
        static double RM_S1(double x, double dx, double t, double dt, double (*f)(double)){
            return 1/2 * ((t + dt + t) - (dt / 2 * dx) * f(t + dt) - f(t));
        }

        static double RM_S2(double x, double dx, double t, double dt, double (*f)(double)){
            return 1/2 * (t + t - dt) - (dt / 2 * dx) * f(t) - f(t - dt);
        }

        static double RM_F(double dx, double dt, double x, double t, double RMS1, double RMS2, double (*f)(double)){
            return t - (dt / dx) * (f(RMS1) - f(RMS2));
        }
};


