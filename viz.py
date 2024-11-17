import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

# Load the CSV file
data = pd.read_csv('result.csv')

# Group data by time
grouped = data.groupby('time')

# Generate colors with varying opacity
opacities = np.linspace(0.2, 1.0, len(grouped))

# Plot each time step as a red line with varying opacity
plt.figure(figsize=(10, 6))
for (time, group), alpha in zip(grouped, opacities):
    plt.plot(group['pos'], group['value'], color='red', alpha=alpha)

# Configure the plot
plt.xlabel('Spatial Position')
plt.ylabel('Solution Value')
plt.title('Solution Comparison Across Time Steps (Red with Varying Opacity)')
plt.grid(True)

# Show the plot
plt.show()
