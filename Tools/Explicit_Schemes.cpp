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

    static double FTBS_alternative(double x, double dx, double dt,double u, vector<double> precedentArray, int i) { // Forward Time Backward Space
        return precedentArray[i] - (u*dt / dx) * (precedentArray[i] - precedentArray[i - 1]);
    }
    
};


class Implicit_Schemes {
    public:
    static double FTBS(double x, double dx, double dt,double u, double (*f)(double)) { // Forward Time Backward Space
        return 1/(1+u*dt/dx) * (f(x) + u*dt/dx * f(x - dx)); // ou u*dt/dx est le CFL
    }

    static double I_FTBS(double x, double dx, double dt, double u, double (*f)(double)){
        return (u * dt * f(x - dx) + dx * f(x)) / (u * dt + dx);
    }
    //TODO: REFAIRE LA FORMULE
    // ATTENTION ERREUR DANS LA FORMULE ON VEUT le precedent du meme array et pas le precedent de l'array precedent 
    static double I_FTBS_alternative(double x, double dx, double dt, double u, vector<double> precedentArray, int i){
        return ( u * dt * precedentArray[i - 1] + dx * precedentArray[i]) / (u * dt + dx);
    }
    
    static double Lax_Wendroff(double x, double dx, double dt,double u, double (*f)(double)) {
        return f(x) - (u*dt / (2*dx)) * (f(x + dx) - f(x - dx)) + (u*u*dt*dt / (2*dx*dx)) * (f(x + dx) - 2*f(x) + f(x - dx));
    }


};

class Step_Schemes {
    public:
    
};


