# ComputationalComputing

## Explicit FTBS

$$
f(n+1) = f(n)-(u*\frac{dt}{dx})*(f(x)-f(x-dx))
$$

## Implicit FTBS

$$
f_{i}^{n+1} = \frac{\Delta x f_{i}^{n} - c \Delta t f_{i+1}^{n+1}}{\Delta x - c * \Delta t}
$$


## Lax-Wendrof

$$
f_{i}^{n+1} = f_{i}^{n} - \frac{c \Delta t}{2 \Delta x} (f_{i+1}^{n} - f_{i-1}^{n}) + \frac{c^2 \Delta t^2}{2 \Delta x^2} (f_{i+1}^{n} - 2 f_{i}^{n} + f_{i-1}^{n}) 
$$

## Ritchmyer multi-step

$$
f_{i}^{n+1} = f_{i}^{n} + \frac{u \Delta t}{2 \Delta x}(f_{i-1}^{n} - f_{i+1}^{n}) + \frac{u^2 \Delta t^2}{2 \Delta x^2}(f_{i+1}^{n} - 2 f_{i}^{n} + f_{i-1}^{n})
$$
