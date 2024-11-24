# ComputationalComputing

## Explicit FTBS

$$
f(n+1) = f(n)-(u*\frac{dt}{dx})*(f(x)-f(x-dx))
$$

## Implicit FTBS

$$
f_{i}^{n+1} = \frac{\Delta x f_{i}^{n} - c \Delta t f_{i+1}^{n+1}}{\Delta x - c * \Delta t}
$$



## **Méthode de Lax-Wendroff**

$$ 
u_i^{n+1} = u_i^n - {\sigma \over 2}(u_{i+1}^n - u_{i-1}^n) + {\sigma^2 \over 2} (u_{i+1}^n - 2u_i^n + u_{i-1}^n)
$$

## **Méthode de Richtmyer**
La méthode de Richtmyer combine :
1. Une **étape de prédiction** avec Lax-Friedrichs :
   $$
   u_{i+\frac{1}{2}}^{n+\frac{1}{2}} = \frac{u_{i+1}^n + u_i^n}{2} - \frac{\sigma}{2}(u_{i+1}^n - u_i^n)
   $$
2. Une **étape de correction** avec Leapfrog :
   $$
   u_i^{n+1} = u_i^n - \sigma (u_{i+\frac{1}{2}}^{n+\frac{1}{2}} - u_{i-\frac{1}{2}}^{n+\frac{1}{2}})
   $$

