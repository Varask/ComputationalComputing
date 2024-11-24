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
    
    static double Lax_Wendroff(double x, double dx, double dt, double u, vector<double> precedentArray, int i) {
        return  precedentArray[i] - (u*dt / (2*dx)) * (precedentArray[i+1] - precedentArray[i-1]) + 
        (u*u*dt*dt / (dx*dx))* (1/2) * (precedentArray[i+1] - 2*precedentArray[i] + precedentArray[i-1]);
    }
};


class Implicit_Schemes {
    public:
    static double FTBS(double x, double dx, double dt,double u, double (*f)(double)) { // Forward Time Backward Space
        return 1/(1+u*dt/dx) * (f(x) + u*dt/dx * f(x - dx)); // ou u*dt/dx est le CFL
    }

    //function to calculate the next value of the array
    static double I_FTBS(double x, double dx, double dt, double u, double (*f)(double)){
        return (u * dt * f(x - dx) + dx * f(x)) / (u * dt + dx);
    }
    //adding alternative function using 'currentArray' instead of 'precedentArray' considering the calculus of [i] and [i-1] of the same array
    static double I_FTBS_alternative(double x, double dx, double dt, double u, double lastPoint, vector<double>precedentArray, int i){
        return ( u * dt * lastPoint + dx * precedentArray[i]) / (u * dt + dx);
    }

};

class Step_Schemes {
    public:
    static double Ritchmyer_method_prediction(double x, double dx, double dt, double u, vector<double>precedentArray,  int i)
    {
        return 0.5*(precedentArray[i+1] + precedentArray[i-1]) - (u*dt/(dx))*(1/4)*(precedentArray[i+1] - precedentArray[i-1]);
    }

    static double Ritchmyer_method_correction(double x, double dx, double dt, double u, vector<double>precedentArray,vector<double>stepArray, int i)
    {
        return precedentArray[i] - (u*dt/(dx))*(1/2)*(stepArray[i+1] - stepArray[i-1]);
    }
    
};


