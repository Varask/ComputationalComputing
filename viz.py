# x, t, f
# -50, 0, 0
# -49, 0.457143, 0
# -48, 0.914286, 0
# -47, 1.37143, 0
# -46, 1.82857, 0
# -45, 2.28571, 0

import matplotlib.pyplot as plt
import numpy as np

# read test.csv
data = np.genfromtxt('test.csv', delimiter=',', names=['x', 't', 'f'])

# plot
plt.plot(data['x'], data['f'], label='f')
plt.legend()

plt.show()
