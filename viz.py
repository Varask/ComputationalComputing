import pandas as pd
import matplotlib.pyplot as plt

# Load the CSV file
file_path = 'test_IFTBS.csv'  # Replace with your actual file path if different
data = pd.read_csv(file_path)

# Combine the split column names into one: 'f(x,t)'
data['f(x,t)'] = data['f(x']

# Drop the now redundant columns
data = data.drop(columns=['f(x', 't)'])

# Plot x vs f(x,t) for each unique value of t without displaying t in the legend
for t in data['t'].unique():
    subset = data[data['t'] == t]
    plt.plot(subset['x'], subset['f(x,t)'])

plt.xlabel('x')
plt.ylabel('f(x,t)')
plt.title('2D Plot of x vs f(x,t) for different values of t')
plt.show()
